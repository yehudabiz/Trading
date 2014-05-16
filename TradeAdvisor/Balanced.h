#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"

using namespace std;

class Balanced : public TraderBase
{
public:
	Balanced(int symbol, int period, double pip, double sl, double tp, double friquency);
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
	double m_pip;
	bool m_trade_allowed;
	vector<Order> m_orders;
};