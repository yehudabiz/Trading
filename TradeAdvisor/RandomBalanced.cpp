#pragma once
#include "RandomBalanced.h"

RandomBalanced::RandomBalanced (int symbol, int period, double pip, double sl, double tp, double friquency, int max_orders_count)
{
	m_max_size = 200;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_friquency = (int)friquency;
	m_max_orders_count = max_orders_count;
	m_source_data.Init(symbol, period, m_max_size);

	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
	m_rand = minstd_rand0(seed);
	m_trade_allowed = true;
	m_long_count = 0;
	m_short_count = 0;
}

void RandomBalanced::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	
	bool coin = m_rand() > (m_rand.min() + m_rand.max()) / 2;
	
	int hours = (m_source_data.GetBar(0).m_time) / 3600;
	if (hours % m_friquency != 0 && m_long_count + m_short_count < m_max_orders_count) m_trade_allowed = true;

	if (m_trade_allowed && hours % m_friquency == 0)
	{
		if (m_long_count > m_short_count) 
		{
			advise = SELL;
		}
		if (m_long_count < m_short_count) 
		{
			advise = BUY;
		}
		if (m_long_count == m_short_count) 
		{
			advise = coin ? BUY : SELL;
		}

		if (advise == SELL)
		{
			stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
			take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
		}
		else
		{
			stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
			take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
		}
		m_trade_allowed = false;
	}
}

void RandomBalanced::UpdatePrice(Bar bar)
{
	bool is_new = (m_source_data.Update(bar)) == UPDATE_NEW;
}

void RandomBalanced::UpdateOrder(Order ord)
{
	if (ord.m_order_type == ORDER_TYPE_LONG)
	{
		if (ord.m_state == ORDER_STATE_OPEN) m_long_count++;
		if (ord.m_state == ORDER_STATE_CLOSED) m_long_count--;
	}
	if (ord.m_order_type == ORDER_TYPE_SHORT)
	{
		if (ord.m_state == ORDER_STATE_OPEN) m_short_count++;
		if (ord.m_state == ORDER_STATE_CLOSED) m_short_count--;
	}
}

void RandomBalanced::Deinit()
{
}
