// Stoch indicator

#pragma once
#include "Global.h"
#include "DataSeries.h"
#include "MA.h"

using namespace std;

class Stoch
{
public:
	void Init(int symbol, vector<double>* high_data, vector<double>* low_data, 
		vector<double>* close_data, int k_period, int d_period, int slowing, int ma_method, int max_size);
	void Update(bool is_new_bar);
	double GetK(int index) { return m_k[index]; };
	double GetDValue(int index) { return m_d.GetValue(index); };
	vector<double>* GetKSeries() { return &m_k; };
	vector<double>* GetDSeries() { return m_d.GetMASeries(); };

private:
	int m_symbol;
	vector<double>* m_high_data;
	vector<double>* m_low_data;
	vector<double>* m_close_data;
	int m_ma_method;
	int m_k_period;
	int m_d_period;
	int m_slowing;
	int m_max_size;
	vector<double> m_k;
	MA m_d;

	void CalculateK(int oldest_recalculated_bar_index);
};