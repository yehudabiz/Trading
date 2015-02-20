#pragma once
#include "ExportFunctions.h"

TraderBase* m_trader;

void __stdcall Init(int trader_id, int symbol, int period, double pip, double p1, double p2, double p3, double p4)
{
	switch (trader_id)
	{
		case RSI_STOCH:
			m_trader = new RsiStochSvm(symbol, period, pip, p1, p2);
			break;
	}
}

void __stdcall Deinit()
{
	m_trader->Deinit();
	delete m_trader;
}

void __stdcall UpdatePrice(long time, double high, double low, double open, double close)
{
	Bar b(time, high, low, open, close);
	m_trader->UpdatePrice(b);
}

void __stdcall AskAdvise(int& advise, double& stop_loss, double& take_profit, long& timeout)
{
	m_trader->AskAdvise(advise, stop_loss, take_profit, timeout);
}

void __stdcall UpdateOrder(long id, int symbol, long open_time, long close_time, int order_type, int order_state, 
						   double entry_price, double close_price, double pl)
{
	Order ord(id, symbol, open_time, close_time, order_type, order_state, close_price, entry_price, pl);
	m_trader->UpdateOrder(ord);
}



	
