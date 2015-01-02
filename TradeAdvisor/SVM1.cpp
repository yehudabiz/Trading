#pragma once
#include "SVM1.h"
#include "SeriesLogger.h"

TraderSvm1::TraderSvm1 (int symbol, int period, double pip, double sl, double tp, int mode, int direction)
{
	m_max_size = 1000;
	m_stoch_k = 100;
	m_stoch_d = 5;
	m_stoch_slow = 3;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_mode = mode;
	m_direction = direction;
	m_source_data.Init(symbol, period, m_max_size);

	m_ma13.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 13, m_max_size);
	m_ma34.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 34, m_max_size);
	m_ma100.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 100, m_max_size);

	m_stoch.Init(m_symbol, m_source_data.GetHighSeries(), m_source_data.GetLowSeries(), m_source_data.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);

	if (m_mode == SVM_ADVISING) m_svm_model = svm_load_model("svm1.mod");
}

void TraderSvm1::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;

	switch (m_mode)
	{
		case SVM_TRAINING:
			if (m_direction == ORDER_TYPE_LONG)
			{
				advise = BUY;
				stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
			}
			if (m_direction == ORDER_TYPE_SHORT)
			{
				advise = SELL;
				stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
				take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
			}
			break;
		case SVM_ADVISING:
			svm_node* node = VectorToNodeArray(CreateVector());
			double label = svm_predict(m_svm_model, node);
			delete node;

			if (label > 0)
			{
				if (m_direction == ORDER_TYPE_LONG)
				{
					advise = BUY;
					stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
					take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
				}
				if (m_direction == ORDER_TYPE_SHORT)
				{
					advise = SELL;
					stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
					take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
				}
			}

			break;
	}
}

void TraderSvm1::UpdatePrice(Bar bar)
{
	int is_new = m_source_data.Update(bar);
	m_ma13.Update(is_new == UPDATE_NEW);
	m_ma34.Update(is_new == UPDATE_NEW);
	m_ma100.Update(is_new == UPDATE_NEW);
	m_stoch.Update(is_new == UPDATE_NEW);
}

void TraderSvm1::UpdateOrder(Order ord)
{
	if (m_mode == SVM_ADVISING) return;

	if (ord.m_state == ORDER_STATE_OPEN)
	{
		m_waiting_vectors.insert(m_waiting_vectors.end(), CreateVector(ord));
	}

	if (ord.m_state == ORDER_STATE_CLOSED)
	{
		for (unsigned i = 0; i < m_waiting_vectors.size(); i++)
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

				delete m_waiting_vectors[i];
				m_waiting_vectors.erase(m_waiting_vectors.begin() + i);
				m_training_data.insert(m_training_data.end(), str_vector);
				break;
			}
		}
	}
}

vector<long>* TraderSvm1::CreateVector(Order ord)
{
	vector<long>* vec = CreateVector();
	vec->insert(vec->begin(), ord.m_id);
	return vec;
}

vector<long>* TraderSvm1::CreateVector()
{
	vector<long>* vec = new vector<long>();
	vec->insert(vec->end(), (long)((m_ma13.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)((m_ma34.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)((m_ma100.GetValue(0) - m_source_data.GetBar(0).m_close) / m_pip));
	vec->insert(vec->end(), (long)(m_stoch.GetK(0)));

	return vec;
}

svm_node* TraderSvm1::VectorToNodeArray(vector<long>* vector)
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

void TraderSvm1::Deinit()
{
	if (m_mode == SVM_TRAINING)
	{
		SeriesLogger training_data_log("training_data.dat", &m_training_data);
		training_data_log.SaveSeries();
	}
}
