#pragma once
#include "RandomShort.h"

RandomShort::RandomShort (int symbol, int period, double pip, double sl, double tp, double friquency)
{
	m_max_size = 200;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_friquency = (int)friquency;
	m_source_data.Init(symbol, period, m_max_size);

	m_trade_allowed = true;
}

void RandomShort::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	
	int hours = (m_source_data.GetBar(0).m_time) / 3600;
	if (hours % m_friquency != 0) m_trade_allowed = true;

	if (m_trade_allowed && hours % m_friquency == 0)
	{
		advise = SELL;
		stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
		take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
		m_trade_allowed = false;
	}
}

void RandomShort::UpdatePrice(Bar bar)
{
	bool is_new = (m_source_data.Update(bar)) == UPDATE_NEW;
}

void RandomShort::Deinit()
{
}
