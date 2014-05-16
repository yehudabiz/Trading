#pragma once
#include "SIT2.h"
#include "SeriesLogger.h"

TraderSit2::TraderSit2 (int symbol, int period)
{
	m_max_size = 200;
	m_stoch_k = 100;
	m_stoch_d = 5;
	m_stoch_slow = 3;
	m_thresh = 85;
	m_min_daily_ma_tan = 0.00200F;
	m_market_type = NO_TREND;
	m_min_ma_entry_tan = 0.00005F;
	m_min_ma_exit_tan = 0;
	m_sl = 0.00300F;
	m_tp = 0.00300F;

	m_source_data.Init(symbol, period, m_max_size);
	m_daily_data.Init(symbol, PERIOD_D1, m_max_size);
	m_long_allowed = true;
	m_short_allowed = true;

	m_ma13.Init(m_symbol, m_source_data.GetCloseSeries(), MODE_EMA, 13, m_max_size);
	m_daily_ma13.Init(m_symbol, m_daily_data.GetCloseSeries(), MODE_EMA, 13, m_max_size);
	m_stoch.Init(m_symbol, m_source_data.GetHighSeries(), m_source_data.GetLowSeries(), m_source_data.GetCloseSeries(), 
		m_stoch_k, m_stoch_d, m_stoch_slow, MODE_EMA, m_max_size);
}

void TraderSit2::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;

	if ((int)((*m_stoch.GetKSeries()).size()) < 2 || (int)((*m_ma13.GetMASeries()).size()) < 2 || 
				(int)((*m_source_data.GetCloseSeries()).size()) < 2) return;

	double d_stoch = m_stoch.GetK(0) - m_stoch.GetK(1);
	double d_ma = m_ma13.GetValue(0) - m_ma13.GetValue(1);

	if (m_stoch.GetK(0) > m_thresh && d_stoch > 0 && d_ma > m_min_ma_entry_tan && m_market_type == TREND_UP &&
		m_source_data.GetBar(0).m_close - m_source_data.GetBar(0).m_open > 0 && m_long_allowed)
	{
		advise += BUY;
		stop_loss = m_source_data.GetBar(0).m_close - m_sl;
		take_profit = m_source_data.GetBar(0).m_close + m_tp;
		m_long_allowed = false;
	}

	if (m_stoch.GetK(0) < 100 - m_thresh && d_stoch < 0 && d_ma < -m_min_ma_entry_tan && m_market_type == TREND_DOWN && 
		m_source_data.GetBar(0).m_close - m_source_data.GetBar(0).m_open < 0 && m_short_allowed)
	{
		advise += SELL;
		stop_loss = m_source_data.GetBar(0).m_close + m_sl;
		take_profit = m_source_data.GetBar(0).m_close - m_tp;
		m_short_allowed = false;
	}
}

void TraderSit2::UpdatePrice(Bar bar)
{
	int is_new_day = m_daily_data.Update(bar);
	m_daily_ma13.Update(is_new_day == UPDATE_NEW);

	if (m_daily_ma13.GetValue(0) > 0 && m_daily_ma13.GetValue(1) > 0 && 
			m_daily_ma13.GetValue(0) - m_daily_ma13.GetValue(1) > m_min_daily_ma_tan) m_market_type = TREND_UP;

	if (m_daily_ma13.GetValue(0) > 0 && m_daily_ma13.GetValue(1) > 0 && 
			m_daily_ma13.GetValue(1) - m_daily_ma13.GetValue(0) > m_min_daily_ma_tan) m_market_type = TREND_DOWN;

	double abs_tan = abs(m_daily_ma13.GetValue(0) - m_daily_ma13.GetValue(1));
	if (abs_tan < m_min_daily_ma_tan)
	{
		m_market_type = NO_TREND;
	}

	int is_new = m_source_data.Update(bar);
	m_ma13.Update(is_new == UPDATE_NEW);
	m_stoch.Update(is_new == UPDATE_NEW);

	if (m_stoch.GetK(0) < 50) m_long_allowed = true;
	if (m_stoch.GetK(0) > 50) m_short_allowed = true;
}

void TraderSit2::Deinit()
{
}
