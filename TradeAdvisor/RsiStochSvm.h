#pragma once
#include "Global.h"
#include "TraderBase.h"
#include "DataSeries.h"
#include "MA.h"
#include "Stoch.h"
#include "RSI.h"
#include "svm.h"

using namespace std;

class RsiStochSvm : public TraderBase
{
public:
	RsiStochSvm(int symbol, int period, double pip, double sl, double tp);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit, long& timeout);
	void UpdatePrice(Bar bar);
	void UpdateOrder(Order ord);
	void Deinit();

private:
	DataSeries m_source_data_5;
	DataSeries m_source_data_15;
	DataSeries m_source_data_h;
	DataSeries m_source_data_da;

	Stoch m_stoch_5;
	Stoch m_stoch_15;
	Stoch m_stoch_h;
	Stoch m_stoch_da;

	RSI m_rsi_5;
	RSI m_rsi_15;
	RSI m_rsi_h;
	RSI m_rsi_da;

	int m_symbol;
	bool m_init_finished;
	int m_svm_ready;
	int m_max_size;
	int m_stoch_k;
	int m_stoch_d;
	int m_stoch_slow;
	double m_pip;
	int m_sl;
	int m_tp;
	int m_to;
	int m_counter;
	long m_order_counter;
	vector<vector<double>*> m_waiting_vectors;
	vector<Order> m_orders;
	svm_model* m_svm_model;
	svm_problem* m_problem;
	svm_parameter* m_svm_params;
	vector<string> m_training_data;
	vector<string> m_testing_data;
	ZLOG_CONTEXT m_log;
	vector<double>* CreateVector(Order ord);
	vector<double>* CreateVector();
	svm_node* VectorToNodeArray(vector<double>* vector);
	void UpdateProblem(svm_problem* problem, svm_node* node, int result);
	void CreateTrainingOrder(Bar bar);
	void CheckOrders(Bar bar);
	void InitSvm();
	void ClearSvm();
};