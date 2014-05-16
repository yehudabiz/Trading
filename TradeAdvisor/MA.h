// Moving avarage indicator

#pragma once
#include "Global.h"
#include "DataSeries.h"

using namespace std;

class MA
{
public:
	void Init(int symbol, vector<double>* source_data, int method, int period, int max_size);
	void Update(bool is_new_bar);
	double GetValue(int index) { return m_ma[index]; };
	vector<double>* GetMASeries() { return &m_ma; };

private:
	int m_symbol;
	vector<double>* m_source_data;
	int m_method;
	int m_period;
	int m_max_size;
	vector<double> m_ma;

	void SMA(int oldest_recalculated_bar_index);
	void EMA(int oldest_recalculated_bar_index);
	void LWMA(int oldest_recalculated_bar_index);
	void SMMA(int oldest_recalculated_bar_index);
};