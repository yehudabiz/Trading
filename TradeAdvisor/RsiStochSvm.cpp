#pragma once
#include "SeriesLogger.h"
#include "RsiStochSvm.h"

RsiStochSvm::RsiStochSvm (int symbol, int period, double pip, double sl, double tp)
{
	m_max_size = 200;
	m_stoch_k = 14;
	m_stoch_d = 5;
	m_stoch_slow = 3;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_to = 2*60*60;
	m_svm_ready = SVM_DISABLED;
	m_init_finished - false;
	m_problem = new svm_problem();
	m_svm_model = 0;
	m_tick_counter = 0;
	m_iteration_counter = 1;
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

void RsiStochSvm::AskAdvise(int& advise, double& stop_loss, double& take_profit, long& timeout)
{
	//WriteToLog(&m_log,"AskAdvise invoked. Last bar time is " + ConvertTimeToString(m_source_data_5.GetBar(0).m_time));
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	timeout = 0;
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
			timeout = m_to;
		}
	}
	//WriteToLog(&m_log,"advise = " + cadv);
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

	if (m_stoch_da.GetK(0) > 0 && m_rsi_da.GetValue(0) > 0 && !m_svm_ready) 
	{
		m_svm_ready = SVM_TRAINING;
		WriteToLog(&m_log, "Initial data collection finished. Traning started");
	}

	if (m_svm_ready == SVM_TRAINING)
 	{
		CheckOrders(bar);
		CreateTrainingOrder(bar);
		if (m_tick_counter > m_training_count)
		{
			SeriesLogger training_data_log(ConvertToString(m_iteration_counter) + "_training_data.dat", &m_logged_data);
			training_data_log.SaveSeries();
			m_logged_data.clear();

			m_tick_counter = 0;
			m_svm_ready = SVM_TESTING;
			WriteToLog(&m_log, "Traning finished");
			WriteToLog(&m_log, "Testing started: " + ConvertTimeToString(bar.m_time));
		}
	}

	if (m_svm_ready == SVM_TESTING)
 	{
		CheckOrders(bar);
		CreateTrainingOrder(bar);
		if (m_tick_counter > m_testing_count)
		{
			SeriesLogger training_data_log(ConvertToString(m_iteration_counter) + "_testing_data.dat", &m_logged_data);
			training_data_log.SaveSeries();
			m_logged_data.clear();

			m_tick_counter = 0;
			InitSvm();
			m_svm_ready = SVM_ADVISING;
			WriteToLog(&m_log, "Testing finished");
			WriteToLog(&m_log, "Trading started: " + ConvertTimeToString(bar.m_time));
		}
	}

	if (m_svm_ready == SVM_ADVISING && m_tick_counter > m_traiding_count)
 	{
		SeriesLogger training_data_log(ConvertToString(m_iteration_counter) + "_traiding_data.dat", &m_logged_data);
		training_data_log.SaveSeries();
		m_logged_data.clear();

		m_tick_counter = 0;
		delete m_problem;
		m_problem = new svm_problem();
		m_orders.clear();
		m_svm_ready = SVM_TRAINING;
		m_iteration_counter++;
		WriteToLog(&m_log, "Trading finished");
		WriteToLog(&m_log, "Traning started: " + ConvertTimeToString(bar.m_time));
	}

	if (m_svm_ready != SVM_DISABLED) m_tick_counter++;
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
				m_logged_data.insert(m_logged_data.end(), str_vector);

				// Delete order id befor converting to node
				m_waiting_vectors[i]->erase(m_waiting_vectors[i]->begin());

				if (m_svm_ready == SVM_TRAINING)
				{
					UpdateProblem(m_problem, VectorToNodeArray(m_waiting_vectors[i]), ord.m_pl > 0 ? 1 : -1);
				}
				else if (m_svm_ready == SVM_TESTING)
				{
					m_testing_nodes.insert(m_testing_nodes.end(), VectorToNodeArray(m_waiting_vectors[i]));
				}

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

	delete m_svm_params;
	ZLOG_Close(m_log);
}

void RsiStochSvm::InitSvm()
{
	ClearSvm();
	m_svm_model = svm_train(m_problem, m_svm_params);
}

void RsiStochSvm::ClearSvm()
{
	svm_free_and_destroy_model(&m_svm_model);
}

void RsiStochSvm::CreateTrainingOrder(Bar bar)
{
	m_order_counter++;
	if (m_order_counter == 2147483646) m_order_counter = 1;
	Order ord(m_order_counter, EURUSD, 0, 0, ORDER_TYPE_LONG, ORDER_STATE_OPEN, bar.m_close, 0, 0);
	ord.m_sl = bar.m_close - m_sl*m_pip;
	ord.m_tp = bar.m_close + m_tp*m_pip;
	ord.m_open_time = bar.m_time;
	m_orders.insert(m_orders.end(), ord);
	UpdateOrder(ord);
}

void RsiStochSvm::CheckOrders(Bar bar)
{
	for (int i = 0; i < m_orders.size(); i++)
	{
		if (i >= m_orders.size()) break;
		if (bar.m_time - m_orders[i].m_open_time > m_to) 
		{
			m_orders.erase(m_orders.begin() + i); 
		}
		else
		{
			if (m_orders[i].m_tp <= bar.m_close || m_orders[i].m_sl >= bar.m_close)
			{
				m_orders[i].m_pl = bar.m_close - m_orders[i].m_entry_price;
				m_orders[i].m_state = ORDER_STATE_CLOSED;
				UpdateOrder(m_orders[i]);
				m_orders.erase(m_orders.begin() + i);
			}
		}
	}
}
