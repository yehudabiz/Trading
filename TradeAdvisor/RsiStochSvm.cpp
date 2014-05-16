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
	m_train_days = train_days;
	m_mode = SVM_DISABLED;
	m_start_time = 0;
	m_lproblem = new svm_problem();
	m_sproblem = new svm_problem();
	m_svm_lmodel = 0;
	m_svm_smodel = 0;
	m_long_short = true;

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

	if (m_train_days == 0)
	{
		m_svm_lmodel = svm_load_model("lsvm.mod");
		m_svm_smodel = svm_load_model("ssvm.mod");
	}

}

void RsiStochSvm::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	WriteToLog(&m_log,"AskAdvise invoked. Last bar time is " + ConvertTimeToString(m_source_data_5.GetBar(0).m_time));
	advise = 0;
	stop_loss = 0;
	take_profit = 0;

	switch (m_mode)
	{
		case SVM_TRAINING:
			WriteToLog(&m_log,"Mode is SVM_TRAINING");
			if (m_long_short)
			{
				advise = BUY + TRAINING_ORDER;
				stop_loss = m_source_data_5.GetBar(0).m_close - m_sl*m_pip;
				take_profit = m_source_data_5.GetBar(0).m_close + m_tp*m_pip;
			}
			else
			{
				advise = SELL + TRAINING_ORDER;
				stop_loss = m_source_data_5.GetBar(0).m_close + m_sl*m_pip;
				take_profit = m_source_data_5.GetBar(0).m_close - m_tp*m_pip;
			}
			m_long_short = !m_long_short;
			break;
		case SVM_TESTING:
			WriteToLog(&m_log,"Mode is SVM_TESTING");

			vector<double>* vect = CreateVector();
			svm_node* node = VectorToNodeArray(vect);

			// Log testing data
			stringstream buf;
			string str_vector = "";
			for (unsigned j = 0; j < (*vect).size(); j++)
			{
				buf << j + 1 << ":" << (*vect)[j];
				str_vector += buf.str() + " ";
				buf.str(string());
			}
			m_testing_data.insert(m_testing_data.end(), str_vector);
			delete vect;

			double llabel = svm_predict(m_svm_lmodel, node);
			double slabel = svm_predict(m_svm_smodel, node);
			delete node;

			WriteToLog(&m_log,"llabel = " + ConvertToString(llabel));
			WriteToLog(&m_log,"slabel = " + ConvertToString(slabel));
			string cadv = "DO_NOTHING";
			if (llabel > 0 && slabel < 0)
			{
				advise = BUY;
				cadv = "BUY";
				stop_loss = m_source_data_5.GetBar(0).m_close - m_sl*m_pip;
				take_profit = m_source_data_5.GetBar(0).m_close + m_tp*m_pip;
			}

			if (llabel < 0 && slabel > 0)
			{
				advise = SELL;
				cadv = "SELL";
				stop_loss = m_source_data_5.GetBar(0).m_close + m_sl*m_pip;
				take_profit = m_source_data_5.GetBar(0).m_close - m_tp*m_pip;
			}
			WriteToLog(&m_log,"advise = " + cadv);

			break;

	}
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

	if (m_stoch_da.GetK(0) > 0 && m_rsi_da.GetValue(0) > 0 && m_mode == SVM_DISABLED)
	{
		if (m_train_days > 0)
		{
			m_mode = SVM_TRAINING;
		}
		else
		{
			m_mode = SVM_TESTING;
		}
	}

	if (m_start_time == 0) m_start_time = bar.m_time;
	if (m_start_time > 0 && bar.m_time - m_start_time > m_train_days*24*60*60 && m_mode == SVM_TRAINING) InitSvm();
}

void RsiStochSvm::UpdateOrder(Order ord)
{
	if (m_mode != SVM_TRAINING) return;

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
				UpdateProblem(ord.m_order_type == ORDER_TYPE_LONG ? m_lproblem : m_sproblem, 
								VectorToNodeArray(m_waiting_vectors[i]), ord.m_pl > 0 ? 1 : -1);
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
	svm_free_model_content(m_svm_lmodel);
	svm_free_model_content(m_svm_smodel);

	SeriesLogger training_data_log("training_data.dat", &m_training_data);
	training_data_log.SaveSeries();

	SeriesLogger testing_data_log("testing_data.dat", &m_testing_data);
	testing_data_log.SaveSeries();

	ZLOG_Close(m_log);
}

void RsiStochSvm::InitSvm()
{
	svm_parameter* par = new svm_parameter();
	par->svm_type = C_SVC;
	par->kernel_type = RBF;
	par->cache_size = 100;
	par->gamma = 0.25;
	par->nr_weight = 0;
	par->eps = 0.001;
	par->shrinking = 1;
	par->C = 1;

	m_svm_lmodel = svm_train(m_lproblem, par);
	m_svm_smodel = svm_train(m_sproblem, par);

	svm_save_model("lsvm.mod", m_svm_lmodel);
	svm_save_model("ssvm.mod", m_svm_smodel);

	delete par;

	m_mode = SVM_TESTING;
}
