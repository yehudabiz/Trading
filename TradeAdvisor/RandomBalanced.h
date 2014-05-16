#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include <random>
#include <chrono>

using namespace std;

class RandomBalanced : public TraderBase
{
public:
	RandomBalanced(int symbol, int period, double pip, double sl, double tp, double friquency, int max_orders_count);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit);
	void UpdatePrice(Bar bar);
	void UpdateOrder(Order ord);
	void Deinit();

private:
	DataSeries m_source_data;
	int m_symbol;
	int m_max_size;
	int m_sl;
	int m_tp;
	int m_friquency;
	int m_max_orders_count;
	double m_pip;
	bool m_trade_allowed;
	minstd_rand0 m_rand;
	int m_short_count;
	int m_long_count;
};