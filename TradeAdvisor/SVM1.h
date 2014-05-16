#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include "MA.h"
#include "Stoch.h"
#include "svm.h"

using namespace std;

class TraderSvm1 : public TraderBase
{
public:
	TraderSvm1(int symbol, int period, double pip, double sl, double tp, int mode, int direction);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit);
	void UpdatePrice(Bar bar);
	void UpdateOrder(Order ord);
	void Deinit();

private:
	DataSeries m_source_data;
	MA m_ma13;
	MA m_ma34;
	MA m_ma100;
	Stoch m_stoch;
	int m_symbol;

	int m_max_size;
	int m_stoch_k;
	int m_stoch_d;
	int m_stoch_slow;
	double m_pip;
	int m_sl;
	int m_tp;
	int m_mode;
	int m_direction;
	vector<vector<long>*> m_waiting_vectors;
	vector<string> m_training_data;
	svm_model* m_svm_model;

	vector<long>* CreateVector(Order ord);
	vector<long>* CreateVector();
	svm_node* VectorToNodeArray(vector<long>* vector);
};