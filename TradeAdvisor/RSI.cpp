#pragma once
#include "RSI.h"

void RSI::Init(int symbol, vector<double>* data, int period, int max_size)
{
	m_data = data;
	m_period = period;
	m_symbol = symbol;
	m_max_size = max_size;

	m_pos_ma.Init(symbol, &m_pos, MODE_EMA, m_period, max_size);
	m_neg_ma.Init(symbol, &m_neg, MODE_EMA, m_period, max_size);
}

void RSI::Update(bool is_new_bar)
{
	if ((int)((*m_data).size()) < 2) return;

	if (is_new_bar)
	{
		m_pos.insert(m_pos.begin(), 0);
		m_neg.insert(m_neg.begin(), 0);
		if ((int)(m_pos.size()) > m_max_size) m_pos.pop_back();
		if ((int)(m_neg.size()) > m_max_size) m_neg.pop_back();
	}

	double dif = (*m_data)[0] - (*m_data)[1];
	if (dif > 0)
	{
		m_pos[0] = dif;
		m_neg[0] = 0;
	}
	if (dif < 0)
	{
		m_pos[0] = 0;
		m_neg[0] = -dif;
	}
	if (dif == 0)
	{
		m_pos[0] = 0;
		m_neg[0] = 0;
	}

	if (m_pos.size() < m_period) return;

	if (is_new_bar)
	{
		m_result.insert(m_result.begin(), 0);
		if ((int)(m_result.size()) > m_max_size) m_result.pop_back();
	}

	m_pos_ma.Update(is_new_bar);
	m_neg_ma.Update(is_new_bar);

	if (m_neg_ma.GetValue(0) == 0)
	{
		m_result[0] = 100;
	}
	else
	{
		m_result[0] = 100.0 - (100.0 /(1 + m_pos_ma.GetValue(0)/m_neg_ma.GetValue(0)));
	}
}

