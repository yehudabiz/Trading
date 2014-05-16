#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include <random>
#include <chrono>

using namespace std;

class Random : public TraderBase
{
public:
	Random(int symbol, int period, double pip, double sl, double tp, double friquency);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit);
	void UpdatePrice(Bar bar);
	void UpdateOrder(Order ord) { };
	void Deinit();

private:
	DataSeries m_source_data;
	int m_symbol;
	int m_max_size;
	int m_sl;
	int m_tp;
	int m_friquency;
	double m_pip;
	bool m_trade_allowed;
	minstd_rand0 m_rand;
};