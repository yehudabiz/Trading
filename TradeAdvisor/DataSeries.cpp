#pragma once
#include "DataSeries.h"

void DataSeries::Init(int symbol, int period, int size)
{
	m_symbol = symbol;
	m_bar_period = period;
	m_max_size = size;
	m_prev_period_remainder = 0;
}

int DataSeries::Update(Bar bar)
{
	int res = UPDATE_ERROR;
	int current_period_remainder = bar.m_time % (m_bar_period * 60);

	try
	{
		if (m_bar_period == PERIOD_TICKS || current_period_remainder < m_prev_period_remainder || (int)(m_close_series.size()) == 0)
		{
			m_time_series.insert(m_time_series.begin(), bar.m_time);
			m_open_series.insert(m_open_series.begin(), bar.m_open);
			m_close_series.insert(m_close_series.begin(), bar.m_close);
			m_high_series.insert(m_high_series.begin(), bar.m_high);
			m_low_series.insert(m_low_series.begin(), bar.m_low);

			if ((int)m_time_series.size() > m_max_size) 
			{
				m_time_series.pop_back();
				m_open_series.pop_back();
				m_close_series.pop_back();
				m_high_series.pop_back();
				m_low_series.pop_back();
			}
			res = UPDATE_NEW;
		}
		else
		{
			m_close_series[0] = bar.m_close;
			if (m_high_series[0] < bar.m_high) m_high_series[0] = bar.m_high;
			if (m_low_series[0] > bar.m_low) m_low_series[0] = bar.m_low;
			res = UPDATE_LAST;
		}
	}
	catch (...) {};

	m_prev_period_remainder = current_period_remainder;
	return res;
}

Bar DataSeries::GetBar(int index)
{
	Bar b = Bar(m_time_series[index], m_high_series[index], m_low_series[index], m_open_series[index], m_close_series[index]);
	return b;
}