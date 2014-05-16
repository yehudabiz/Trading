#pragma once
#include "ExportFunctions.h"

TraderBase* m_trader;

void __stdcall Init(int trader_id, int symbol, int period, double pip, double p1, double p2, double p3, double p4)
{
	switch (trader_id)
	{
		case RANDOM:
			m_trader = new Random(symbol, period, pip, p1, p2, p3);
			break;
		case RANDOM_BALANCED:
			m_trader = new RandomBalanced(symbol, period, pip, p1, p2, p3, (int)p4);
			break;
		case RANDOM_LONG:
			m_trader = new RandomLong(symbol, period, pip, p1, p2, p3);
			break;
		case RANDOM_SHORT:
			m_trader = new RandomShort(symbol, period, pip, p1, p2, p3);
			break;
		case SIT1:
			m_trader = new TraderSit1(symbol, period);
			break;
		case SIT2:
			m_trader = new TraderSit2(symbol, period);
			break;
		case SIT3:
			m_trader = new TraderSit3(symbol, period, p1, p2);
			break;
		case BALANCED:
			m_trader = new Balanced(symbol, period, pip, p1, p2, p3);
			break;
		case SVM1:
			m_trader = new TraderSvm1(symbol, period, pip, p1, p2, (int)p3,  (int)p4);
			break;
		case SVM2:
			m_trader = new TraderSvm2(symbol, period, pip, p1, p2, (int)p3);
			break;
		case DUAL_SVM:
			m_trader = new TraderDualSvm(symbol, period, pip, p1, p2, (int)p3);
			break;
		case RSI_TRADER:
			m_trader = new RsiTrader(symbol, period, pip, p1, p2, (int)p3);
			break;
		case RSI_STOCH:
			m_trader = new RsiStochSvm(symbol, period, pip, p1, p2, (int)p3);
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

void __stdcall AskAdvise(int& advise, double& stop_loss, double& take_profit)
{
	m_trader->AskAdvise(advise, stop_loss, take_profit);
}

void __stdcall UpdateOrder(long id, int symbol, long open_time, long close_time, int order_type, int order_state, 
						   double entry_price, double close_price, double pl)
{
	Order ord(id, symbol, open_time, close_time, order_type, order_state, close_price, entry_price, pl);
	m_trader->UpdateOrder(ord);
}



	
