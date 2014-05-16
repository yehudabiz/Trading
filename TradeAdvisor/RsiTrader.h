#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include "RSI.h"
#include "SeriesLogger.h"

using namespace std;

class RsiTrader : public TraderBase
{
public:
	RsiTrader(int symbol, int period, double pip, double sl, double tp, int rsi_period);
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
	int m_rsi_period;
	double m_pip;

	RSI m_rsi;
};