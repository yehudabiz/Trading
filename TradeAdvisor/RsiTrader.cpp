#pragma once
#include "RsiTrader.h"

RsiTrader::RsiTrader (int symbol, int period, double pip, double sl, double tp, int rsi_period)
{
	m_max_size = 200;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_rsi_period = rsi_period;
	m_source_data.Init(symbol, period, m_max_size);

	m_rsi.Init(symbol, m_source_data.GetCloseSeries(), m_rsi_period, m_max_size);
}

void RsiTrader::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	if ((*(m_rsi.GetRsiSeries())).size() == 0 || m_rsi.GetValue(0) == 0) return;

	if (m_rsi.GetValue(0) > 80)
	{
		advise = SELL;
		stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
		take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
	}
	if (m_rsi.GetValue(0) < 20)
	{
		advise = BUY;
		stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
		take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
	}
}

void RsiTrader::UpdatePrice(Bar bar)
{
	bool is_new = (m_source_data.Update(bar)) == UPDATE_NEW;
	m_rsi.Update(is_new);
}

void RsiTrader::Deinit()
{
}
