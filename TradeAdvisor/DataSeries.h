#pragma once
#include "Global.h"
#include "Bar.h"

using namespace std;

class DataSeries
{
public:
	void Init(int symbol, int period, int size);
	int Update(Bar bar);  // Returns update result
	int GetSymbol() { return m_symbol; };
	Bar GetBar(int index);
	vector<long>* GetTimeSeries() { return &m_time_series; };
	vector<double>* GetOpenSeries() { return &m_open_series; };
	vector<double>* GetCloseSeries() { return &m_close_series; };
	vector<double>* GetHighSeries() { return &m_high_series; };
	vector<double>* GetLowSeries() { return &m_low_series; };

private:
	int m_symbol;
	int m_bar_period;
	int m_max_size;
	long m_prev_period_remainder;
	vector<long> m_time_series;
	vector<double> m_open_series;
	vector<double> m_close_series;
	vector<double> m_high_series;
	vector<double> m_low_series;
};

