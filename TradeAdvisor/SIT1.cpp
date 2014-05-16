#pragma once
#include "SIT1.h"
#include "SeriesLogger.h"

TraderSit1::TraderSit1 (int symbol, int period)
{
	m_max_size = 200;
	m_stoch_k = 100;
	m_stoch_d = 5;
	m_stoch_slow = 3;
	m_thresh = 85;
	m_min_ma_entry_tan = 0.00005F;
	m_min_ma_exit_tan = 0;
	m_sl = 0.0015F;

	m_source_data.Init(symbol, period, m_max_size);
	m_long_allowed = true;
	m_short_allowed = true;

	m_ma13.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 13, m_max_size);
	m_stoch.Init(m_symbol, m_source_data.GetHighSeries(), m_source_data.GetLowSeries(), m_source_data.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
}

void TraderSit1::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;

	if ((int)((*m_stoch.GetKSeries()).size()) < 2 || (int)((*m_ma13.GetMASeries()).size()) < 2 || 
				(int)((*m_source_data.GetCloseSeries()).size()) < 2) return;

	double d_stoch = m_stoch.GetK(0) - m_stoch.GetK(1);
	double d_ma = m_ma13.GetValue(0) - m_ma13.GetValue(1);
	double d_ma_exit = m_ma13.GetValue(0) - m_ma13.GetValue(2);

	if (m_stoch.GetK(0) > m_thresh && d_stoch > 0 && d_ma > m_min_ma_entry_tan && 
		m_source_data.GetBar(0).m_close - m_source_data.GetBar(0).m_open > 0 && m_long_allowed)
	{
		advise += BUY;
		stop_loss = m_source_data.GetBar(0).m_close - m_sl;
		m_long_allowed = false;
	}

	if (m_stoch.GetK(0) < 100 - m_thresh && d_stoch < 0 && d_ma < -m_min_ma_entry_tan && 
		m_source_data.GetBar(0).m_close - m_source_data.GetBar(0).m_open < 0 && m_short_allowed)
	{
		advise += SELL;
		stop_loss = m_source_data.GetBar(0).m_close + m_sl;
		m_short_allowed = false;
	}

	if (d_ma_exit < -m_min_ma_exit_tan) advise += CLOSE_LONG;
	if (d_ma_exit > m_min_ma_exit_tan) advise += CLOSE_SHORT;
}

void TraderSit1::UpdatePrice(Bar bar)
{
	int is_new = m_source_data.Update(bar);
	m_ma13.Update(is_new == UPDATE_NEW);
	m_stoch.Update(is_new == UPDATE_NEW);

	if (m_stoch.GetK(0) < 50) m_long_allowed = true;
	if (m_stoch.GetK(0) > 50) m_short_allowed = true;
}

void TraderSit1::Deinit()
{
	SeriesLogger source_data_log("source_data.dat", m_source_data.GetTimeSeries(), m_source_data.GetHighSeries(), 
						m_source_data.GetLowSeries(), m_source_data.GetOpenSeries(), m_source_data.GetCloseSeries());
	SeriesLogger ma13_log("m13.dat", m_source_data.GetTimeSeries(), m_ma13.GetMASeries());
	SeriesLogger stoch_log("stoch.dat", m_source_data.GetTimeSeries(), m_stoch.GetKSeries());

	source_data_log.SaveSeries();
	ma13_log.SaveSeries();
	stoch_log.SaveSeries();
}
