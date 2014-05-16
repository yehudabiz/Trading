// Stoch indicator

#pragma once
#include "Global.h"
#include "DataSeries.h"
#include "MA.h"

using namespace std;

class RSI
{
public:
	void Init(int symbol, vector<double>* data, int period, int max_size);
	void Update(bool is_new_bar);
	double GetValue(int index) { return m_result[index]; };
	vector<double>* GetRsiSeries() { return &m_result; };

private:
	int m_symbol;
	vector<double>* m_data;
	int m_period;
	int m_max_size;
	vector<double> m_result;
	vector<double> m_pos;
	vector<double> m_neg;

	MA m_pos_ma;
	MA m_neg_ma;
};