#pragma once
#include "DualSvm.h"
#include "SeriesLogger.h"

TraderDualSvm::TraderDualSvm (int symbol, int period, double pip, double sl, double tp, int train_days)
{
	m_max_size = 1000;
	m_stoch_k = 100;
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

	m_source_data.Init(symbol, period, m_max_size);

	m_ma13.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 13, m_max_size);
	m_ma34.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 34, m_max_size);
	m_ma100.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 100, m_max_size);
	m_stoch.Init(m_symbol, m_source_data.GetHighSeries(), m_source_data.GetLowSeries(), m_source_data.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
}

void TraderDualSvm::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;

	switch (m_mode)
	{
		case SVM_TRAINING:
			if (m_long_short)
			{
				advise = BUY + TRAINING_ORDER;
				stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
			}
			else
			{
				advise = SELL + TRAINING_ORDER;
				stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
			}
			m_long_short = !m_long_short;
			break;
		case SVM_ADVISING:
			svm_node* node = VectorToNodeArray(CreateVector());
			double llabel = svm_predict(m_svm_lmodel, node);
			double slabel = svm_predict(m_svm_smodel, node);
			delete node;

			if (llabel > 0 && slabel < 0)
			{
				advise = BUY;
				stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
			}

			if (llabel < 0 && slabel > 0)
			{
				advise = SELL;
				stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
			}

			break;
	}
}

void TraderDualSvm::UpdatePrice(Bar bar)
{
	int is_new = m_source_data.Update(bar);
	m_ma13.Update(is_new == UPDATE_NEW);
	m_ma34.Update(is_new == UPDATE_NEW);
	m_ma100.Update(is_new == UPDATE_NEW);
	m_stoch.Update(is_new == UPDATE_NEW);
	
	if (m_ma100.GetValue(0) > 0 && m_stoch.GetK(0) > 0 && m_mode == SVM_DISABLED)
	{
		m_mode = SVM_TRAINING;
	}

	if (m_start_time == 0) m_start_time = bar.m_time;
	if (m_start_time > 0 && bar.m_time - m_start_time > m_train_days*24*60*60 && m_mode == SVM_TRAINING) InitSvm();
}

void TraderDualSvm::UpdateOrder(Order ord)
{
	if (m_mode != SVM_TRAINING) return;

	if (ord.m_state == ORDER_STATE_OPEN)
	{
		m_waiting_vectors.insert(m_waiting_vectors.end(), CreateVector(ord));
	}

	if (ord.m_state == ORDER_STATE_CLOSED)
	{
		for (unsigned int i = 0; i < m_waiting_vectors.size(); i++)
		{
			if ((*m_waiting_vectors[i])[0] == ord.m_id)
			{
				// Delete order id befor converting to node
				m_waiting_vectors[i]->erase(m_waiting_vectors[i]->begin());
				UpdateProblem(ord.m_order_type == ORDER_TYPE_LONG ? m_lproblem : m_sproblem, 
								VectorToNodeArray(m_waiting_vectors[i]), ord.m_pl > 0 ? 1 : -1);
				m_waiting_vectors.erase(m_waiting_vectors.begin() + i);
				break;
			}
		}
	}
}

void TraderDualSvm::UpdateProblem(svm_problem* problem, svm_node* node, int result)
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

vector<long>* TraderDualSvm::CreateVector(Order ord)
{
	vector<long>* vec = CreateVector();
	vec->insert(vec->begin(), ord.m_id);
	return vec;
}

vector<long>* TraderDualSvm::CreateVector()
{
	vector<long>* vec = new vector<long>();
	vec->insert(vec->end(), (long)((m_ma13.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)((m_ma34.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)((m_ma100.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)(m_stoch.GetK(0)));

	return vec;
}

svm_node* TraderDualSvm::VectorToNodeArray(vector<long>* vector)
{
	svm_node* res = new svm_node[vector->size() + 1]();
	for (unsigned i = 0; i < vector->size(); i++)
	{
		res[i].index = i + 1;
		res[i].value = (*vector)[i];
	}
	res[vector->size()].index = -1;
	delete vector;
	return res;
}

void TraderDualSvm::Deinit()
{
	svm_free_model_content(m_svm_lmodel);
	svm_free_model_content(m_svm_smodel);
}

void TraderDualSvm::InitSvm()
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
	delete par;

	m_mode = SVM_ADVISING;
}
