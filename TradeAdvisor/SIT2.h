#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include "MA.h"
#include "Stoch.h"

using namespace std;

class TraderSit2 : public TraderBase
{
public:
	TraderSit2(int symbol, int period);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit);
	void UpdatePrice(Bar bar);
	void UpdateOrder(Order ord) { };
	void Deinit();

private:
	DataSeries m_source_data;
	DataSeries m_daily_data;
	MA m_ma13;
	MA m_daily_ma13;
	Stoch m_stoch;
	int m_symbol;

	int m_max_size;
	int m_stoch_k;
	int m_stoch_d;
	int m_stoch_slow;
	int m_thresh;
	double m_sl;
	double m_tp;
	int m_market_type;
	double m_min_daily_ma_tan;
	double m_min_ma_entry_tan;
	double m_min_ma_exit_tan;
	bool m_long_allowed;
	bool m_short_allowed;
};