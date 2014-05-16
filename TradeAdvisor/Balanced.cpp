#pragma once
#include "Balanced.h"

Balanced::Balanced (int symbol, int period, double pip, double sl, double tp, double friquency)
{
	m_max_size = 200;
	m_pip = pip;
	m_sl = (int)sl;
	m_tp = (int)tp;
	m_friquency = (int)friquency;
	m_source_data.Init(symbol, period, m_max_size);

	m_trade_allowed = true;
}

void Balanced::AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	advise = 0;
	stop_loss = 0;
	take_profit = 0;
	
	int hours = (m_source_data.GetBar(0).m_time) / 3600;
	if (hours % m_friquency != 0 && m_orders.size() < 3) m_trade_allowed = true;

	if (m_orders.size() == 0) advise = SELL;

	if (m_orders.size() == 1)
	{
		if (m_orders[0].m_order_type == ORDER_TYPE_LONG) advise = SELL;
		if (m_orders[0].m_order_type == ORDER_TYPE_SHORT) advise = BUY;
	}

	if (m_orders.size() == 2)
	{
		double price = m_source_data.GetBar(0).m_close;
		double profit0 = (price - m_orders[0].m_entry_price) * m_orders[0].m_order_type;
		double profit1 = (price - m_orders[1].m_entry_price) * m_orders[1].m_order_type;

		if (m_orders[0].m_order_type == m_orders[1].m_order_type && profit0 < 0 && profit1 < 0)
		{
			if (m_orders[0].m_order_type == ORDER_TYPE_LONG)
			{
				advise = SELL;
			}
			else
			{
				advise = BUY;
			}
		}
		else
		{
			if (m_trade_allowed && hours % m_friquency == 0)
			{

				if (profit0 > profit1) 
				{
					if (m_orders[0].m_order_type == ORDER_TYPE_LONG)
					{
						advise = BUY;
					}
					else
					{
						advise = SELL;
					}
				}
				else
				{
					if (m_orders[1].m_order_type == ORDER_TYPE_LONG)
					{
						advise = BUY;
					}
					else
					{
						advise = SELL;
					}
				}

				m_trade_allowed = false;
			}
		}
	}

	if (advise == SELL)
	{
		stop_loss = m_source_data.GetBar(0).m_close + m_sl*m_pip;
		take_profit = m_source_data.GetBar(0).m_close - m_tp*m_pip;
	}
	if (advise == BUY)
	{
		stop_loss = m_source_data.GetBar(0).m_close - m_sl*m_pip;
		take_profit = m_source_data.GetBar(0).m_close + m_tp*m_pip;
	}
}

void Balanced::UpdatePrice(Bar bar)
{
	bool is_new = (m_source_data.Update(bar)) == UPDATE_NEW;
}

void Balanced::UpdateOrder(Order ord)
{
	if (ord.m_state == ORDER_STATE_OPEN) m_orders.insert(m_orders.end(), ord);

	if (ord.m_state == ORDER_STATE_CLOSED) 
	{
		for (unsigned i = 0; i < m_orders.size(); i++)
		{
			if (m_orders[i].m_id == ord.m_id) m_orders.erase(m_orders.begin() + i);
		}
	}
}

void Balanced::Deinit()
{
	m_orders.clear();
}
