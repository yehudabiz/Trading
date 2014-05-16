#pragma once
#include "Global.h"
#include "Order.h"
#include "SIT1.h"
#include "SIT2.h"
#include "SIT3.h"
#include "Random.h"
#include "RandomBalanced.h"
#include "RandomLong.h"
#include "RandomShort.h"
#include "Balanced.h"
#include "SVM1.h"
#include "SVM2.h"
#include "DualSvm.h"
#include "RsiTrader.h"
#include "RsiStochSvm.h"


// Export functions
extern "C"
{
	void __stdcall Init(int trader_id, int symbol, int period, double pip, double p1, double p2, double p3, double p4);

	void __stdcall Deinit();

	void __stdcall UpdatePrice(long time, double high, double low, double open, double close);

	void __stdcall AskAdvise(int& advise, double& stop_loss, double& take_profit);

	void __stdcall UpdateOrder(long id, int symbol, long open_time, long close_time, int order_type, int order_state, 
						   double entry_price, double close_price, double pl);
}


