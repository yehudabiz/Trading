#pragma once
#include "MA.h"

void MA::Init(int symbol, vector<double>* source_data, int method, int period, int max_size)
{
	m_source_data = source_data;
	m_method = method;
	m_period = period;
	m_symbol = symbol;
	m_max_size = max_size;

	int data_size = (*m_source_data).size();
	m_ma.insert(m_ma.begin(), max_size, 0);

	if (data_size - m_period >= 0)
	{
		switch (m_method)
		{
			case MODE_SMA:
				SMA(data_size - m_period);
				break;
			case MODE_EMA:
				EMA(data_size - m_period);
				break;
			case MODE_LWMA:
				LWMA(data_size - m_period);
				break;
			case MODE_SMMA:
				SMMA(data_size - m_period);
				break;
		}
	}
}

void MA::Update(bool is_new_bar)
{
	if (is_new_bar)
	{
		m_ma.insert(m_ma.begin(), 0);
		if ((int)(m_ma.size()) > m_max_size) m_ma.pop_back();
	}

	if ((int)((*m_source_data).size()) < m_period) return;

	switch (m_method)
	{
		case MODE_SMA:
			SMA(1);
			break;
		case MODE_EMA:
			EMA(1);
			break;
		case MODE_LWMA:
			LWMA(1);
			break;
		case MODE_SMMA:
			SMMA(1);
			break;
	}
}

// private

void MA::SMA(int oldest_recalculated_bar_index)
{
	int position = oldest_recalculated_bar_index;

	while (position >= 0)
	{
		double sum = 0;
		for (int i = position; i < position + m_period; i++)
		{
			sum += (*m_source_data)[i];
		}
		m_ma[position] = sum / m_period;
		position--;
	}
}

void MA::EMA(int oldest_recalculated_bar_index)
{
   double alfa = 2.0F / (m_period + 1);
   int position = oldest_recalculated_bar_index;

   while (position >= 0)
   {
	   if (m_ma[position + 1] == 0) m_ma[position + 1] = (*m_source_data)[position + 1];

	   m_ma[position] = alfa * (*m_source_data)[position] + (1 - alfa) * m_ma[position + 1];
 	   position--;
   }
}

void MA::LWMA(int oldest_recalculated_bar_index)
{

}

void MA::SMMA(int oldest_recalculated_bar_index)
{

}
