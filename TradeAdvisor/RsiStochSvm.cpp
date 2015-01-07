#pragma once
#include "DualSvm.h"
#include "SeriesLogger.h"
#include "RsiStochSvm.h"

RsiStochSvm::RsiStochSvm (int symbol, int period, double pip, double sl, double tp, int train_days)
{
	m_max_size = 200;
	m_stoch_k = 14;
	m_stoch_d = 5;
	m_stoch_slow = 3;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_svm_ready = SVM_DISABLED;
	m_init_finished - false;
	m_current_problem = m_problem_1;
	m_current_problem = new svm_problem();
	m_svm_model = 0;
	m_counter = 0;
	m_order_counter = 0;

	m_svm_params = new svm_parameter();
	m_svm_params->svm_type = C_SVC;
	m_svm_params->kernel_type = RBF;
	m_svm_params->cache_size = 100;
	m_svm_params->gamma = 0.25;
	m_svm_params->nr_weight = 0;
	m_svm_params->eps = 0.001;
	m_svm_params->shrinking = 1;
	m_svm_params->C = 1;


	ZLOG_Open(&m_log, "Logs", "", 0, ZLOG_FAC_DEFAULT, "", "RS", "1.0", true, 1);

	m_source_data_5.Init(symbol, 5, m_max_size);
	m_source_data_15.Init(symbol, 15, m_max_size);
	m_source_data_h.Init(symbol, 60, m_max_size);
	m_source_data_da.Init(symbol, 1440, m_max_size);

	m_stoch_5.Init(m_symbol, m_source_data_5.GetHighSeries(), m_source_data_5.GetLowSeries(), m_source_data_5.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
	m_stoch_15.Init(m_symbol, m_source_data_15.GetHighSeries(), m_source_data_15.GetLowSeries(), m_source_data_15.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
	m_stoch_h.Init(m_symbol, m_source_data_h.GetHighSeries(), m_source_data_h.GetLowSeries(), m_source_data_h.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
	m_stoch_da.Init(m_symbol, m_source_data_da.GetHighSeries(), m_source_data_da.GetLowSeries(), m_source_data_da.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);

	m_rsi_5.Init(symbol, m_source_data_5.GetCloseSeries(), m_stoch_k, m_max_size);
	m_rsi_15.Init(symbol, m_source_data_15.GetCloseSeries(), m_stoch_k, m_max_size);
	m_rsi_h.Init(symbol, m_source_data_h.GetCloseSeries(), m_stoch_k, m_max_size);
	m_rsi_da.Init(symbol, m_source_data_da.GetCloseSeries(), m_stoch_k, m_max_size);
}

void RsiStochSvm::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	WriteToLog(&m_log,"AskAdvise invoked. Last bar time is " + ConvertTimeToString(m_source_data_5.GetBar(0).m_time));
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	string cadv = "DO_NOTHING";

	if (m_svm_ready == SVM_ADVISING)
	{
		vector<double>* vect = CreateVector();
		svm_node* node = VectorToNodeArray(vect);

		double label = svm_predict(m_svm_model, node);
		delete node;

		WriteToLog(&m_log,"label = " + ConvertToString(label));
		if (label > 0)
		{
			advise = BUY;
			cadv = "BUY";
			stop_loss = m_source_data_5.GetBar(0).m_close - m_sl*m_pip;
			take_profit = m_source_data_5.GetBar(0).m_close + m_tp*m_pip;
		}
	}
	WriteToLog(&m_log,"advise = " + cadv);
}

void RsiStochSvm::UpdatePrice(Bar bar)
{
	bool is_new_5 = m_source_data_5.Update(bar) == UPDATE_NEW;
	bool is_new_15 = m_source_data_15.Update(bar) == UPDATE_NEW;
	bool is_new_h = m_source_data_h.Update(bar) == UPDATE_NEW;
	bool is_new_da = m_source_data_da.Update(bar) == UPDATE_NEW;

	m_stoch_5.Update(is_new_5);
	m_stoch_15.Update(is_new_15);
	m_stoch_h.Update(is_new_h);
	m_stoch_da.Update(is_new_da);

	m_rsi_5.Update(is_new_5);
	m_rsi_15.Update(is_new_15);
	m_rsi_h.Update(is_new_h);
	m_rsi_da.Update(is_new_da);

	if (m_stoch_da.GetK(0) > 0 && m_rsi_da.GetValue(0) > 0 && !m_svm_ready) m_svm_ready = SVM_TRAINING;

	if (m_counter > 1000)
	{
		m_counter = 0;
		InitSvm();
		m_svm_ready = SVM_ADVISING;
	}

	if (m_svm_ready != SVM_DISABLED)
	{
		m_counter++;
		CheckOrders(bar.m_close);
		CreateTrainingOrder(bar.m_close);
	}
}

void RsiStochSvm::UpdateOrder(Order ord)
{
	if (!m_svm_ready) return;

	if (ord.m_state == ORDER_STATE_OPEN)
	{
		WriteToLog(&m_log, "Order " + ConvertToString(ord.m_id) + " opened");
		m_waiting_vectors.insert(m_waiting_vectors.end(), CreateVector(ord));
	}

	if (ord.m_state == ORDER_STATE_CLOSED)
	{
		WriteToLog(&m_log, "Order " + ConvertToString(ord.m_id) + " closed. PL = " + ConvertToString(ord.m_pl));
		for (unsigned int i = 0; i < m_waiting_vectors.size(); i++)
		{
			if ((*m_waiting_vectors[i])[0] == ord.m_id)
			{
				stringstream buf;
				string str_vector = ord.m_pl > 0 ? "+1 " : "-1 ";

				for (unsigned j = 1; j < (*m_waiting_vectors[i]).size(); j++)
				{
					buf << j << ":" << (*m_waiting_vectors[i])[j];
					str_vector += buf.str() + " ";
					buf.str(string());
				}
				m_training_data.insert(m_training_data.end(), str_vector);

				// Delete order id befor converting to node
				m_waiting_vectors[i]->erase(m_waiting_vectors[i]->begin());
				UpdateProblem(m_current_problem, VectorToNodeArray(m_waiting_vectors[i]), ord.m_pl > 0 ? 1 : -1);
				delete m_waiting_vectors[i];
				m_waiting_vectors.erase(m_waiting_vectors.begin() + i);
				break;
			}
		}
	}
}

void RsiStochSvm::UpdateProblem(svm_problem* problem, svm_node* node, int result)
{
	int old_size = problem->l;
	double* tempy = problem->y;
	svm_node** tempx = problem->x;
	problem->y = new double[old_size + 1];
	problem->x = new svm_node*[old_size + 1];
	problem->l = old_size + 1;
	if (old_size > 0)
	{
		copy(tempy, tempy + old_size, problem->y);
		copy(tempx, tempx + old_size, problem->x);
		delete[] tempy;
		delete[] tempx;
	}

	problem->y[old_size] = result;
	problem->x[old_size] = node;

}

vector<double>* RsiStochSvm::CreateVector(Order ord)
{
	vector<double>* vec = CreateVector();
	vec->insert(vec->begin(), ord.m_id);
	return vec;
}

vector<double>* RsiStochSvm::CreateVector()
{
	vector<double>* vec = new vector<double>();
	vec->insert(vec->end(), m_stoch_5.GetK(0));
	vec->insert(vec->end(), m_stoch_15.GetK(0));
	vec->insert(vec->end(), m_stoch_h.GetK(0));
	vec->insert(vec->end(), m_stoch_da.GetK(0));
	vec->insert(vec->end(), m_rsi_5.GetValue(0));
	vec->insert(vec->end(), m_rsi_15.GetValue(0));
	vec->insert(vec->end(), m_rsi_h.GetValue(0));
	vec->insert(vec->end(), m_rsi_da.GetValue(0));

	return vec;
}

svm_node* RsiStochSvm::VectorToNodeArray(vector<double>* vector)
{
	svm_node* res = new svm_node[vector->size() + 1]();
	for (unsigned i = 0; i < vector->size(); i++)
	{
		res[i].index = i + 1;
		res[i].value = (*vector)[i];
	}
	res[vector->size()].index = -1;
	return res;
}

void RsiStochSvm::Deinit()
{
	svm_free_and_destroy_model(&m_svm_model);

	SeriesLogger training_data_log("training_data.dat", &m_training_data);
	training_data_log.SaveSeries();

	delete m_svm_params;
	ZLOG_Close(m_log);
}

void RsiStochSvm::InitSvm()
{
	ClearSvm();
	if (m_current_problem == m_problem_1)
	{
		m_svm_model = svm_train(m_problem_1, m_svm_params);
		if (m_problem_2 != nullptr) delete m_problem_2;
		m_problem_2 = new svm_problem();
		m_current_problem = m_problem_2;
	}
	else if (m_current_problem == m_problem_2)
	{
		m_svm_model = svm_train(m_problem_2, m_svm_params);
		if (m_problem_1 != nullptr) delete m_problem_1;
		m_problem_1 = new svm_problem();
		m_current_problem = m_problem_1;
	}
}

void RsiStochSvm::ClearSvm()
{
	svm_free_and_destroy_model(&m_svm_model);
}

void RsiStochSvm::CreateTrainingOrder(double curr_price)
{
	m_order_counter++;
	if (m_order_counter == 2147483646) m_order_counter = 1;
	Order ord(m_order_counter, EURUSD, 0, 0, ORDER_TYPE_LONG, ORDER_STATE_OPEN, curr_price, 0, 0);
	ord.m_sl = curr_price - m_sl*m_pip;
	ord.m_tp = curr_price + m_tp*m_pip;
	m_orders.insert(m_orders.end(), ord);
	UpdateOrder(ord);
}

void RsiStochSvm::CheckOrders(double curr_price)
{
	for (int i = 0; i < m_orders.size(); i++)
	{
		if (i >= m_orders.size()) break;
		if (m_orders[i].m_tp <= curr_price || m_orders[i].m_sl >= curr_price)
		{
			m_orders[i].m_pl = curr_price - m_orders[i].m_entry_price;
			m_orders[i].m_state = ORDER_STATE_CLOSED;
			UpdateOrder(m_orders[i]);
			m_orders.erase(m_orders.begin() + i);
		}
	}
}
