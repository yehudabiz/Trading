#pragma once
#include "Stoch.h"

void Stoch::Init(int symbol, vector<double>* high_data, vector<double>* low_data, 
	vector<double>* close_data, int k_period, int d_period, int slowing, int ma_method, int max_size)
{
	m_high_data = high_data;
	m_low_data = low_data;
	m_close_data = close_data;
	m_ma_method = ma_method;
	m_k_period = k_period;
	m_d_period = d_period;
	m_slowing = slowing;
	m_symbol = symbol;
	m_max_size = max_size;

	int data_size =  (int)((*m_close_data).size());
	m_k.insert(m_k.begin(), max_size, 0);

	if (data_size >= k_period) CalculateK(data_size - k_period);

	m_d.Init(symbol, &m_k, MODE_EMA, d_period, max_size);
}

void Stoch::Update(bool is_new_bar)
{
	if (is_new_bar)
	{
		m_k.insert(m_k.begin(), 0);
		if ((int)(m_k.size()) > m_max_size) m_k.pop_back();
	}

	if ((int)((*m_close_data).size()) < m_k_period + m_slowing) return;

	CalculateK(1);
	m_d.Update(is_new_bar);
}

void Stoch::CalculateK(int oldest_recalculated_bar_index)
{
	int position = oldest_recalculated_bar_index + m_slowing - 1;

	vector<double> highest, lowest;
	highest.insert(highest.begin(), position + 1, 0);
	lowest.insert(lowest.begin(), position + 1, 0);

	while (position >= 0)
	{
		for (int i = position + m_k_period - 1; i >= position; i--)
		{
			if (highest[position] == 0 || highest[position] < (*m_high_data)[i]) highest[position] = (*m_high_data)[i];
			if (lowest[position] == 0 || lowest[position] > (*m_low_data)[i]) lowest[position] = (*m_low_data)[i];
		}
		position--;
	}

	position = oldest_recalculated_bar_index;
	while (position >= 0)
	{
		double high_sum = 0;
		double low_sum = 0;

		for (int i = position + m_slowing - 1; i >= position; i--)
		{
			low_sum += (*m_close_data)[i] - lowest[i];
			high_sum += highest[i] - lowest[i];
		}

		if (high_sum == 0) 
		{
			m_k[position] = 100;
		}
		else
		{
			m_k[position] = (low_sum/high_sum) * 100;
		}
		position--;
	}
}