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
	RsiStochSvm(int symbol, int period, double pip, double sl, double tp, int train_days);
 	void AskAdvise(int& advise, double& stop_loss, double& take_profit);
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
	int m_counter;
	long m_order_counter;
	vector<vector<double>*> m_waiting_vectors;
	vector<Order> m_orders;
	svm_model* m_svm_model;
	svm_problem* m_problem_1;
	svm_problem* m_problem_2;
	svm_problem* m_current_problem;
	svm_parameter* m_svm_params;
	vector<string> m_training_data;
	vector<string> m_testing_data;
	ZLOG_CONTEXT m_log;
	vector<double>* CreateVector(Order ord);
	vector<double>* CreateVector();
	svm_node* VectorToNodeArray(vector<double>* vector);
	void UpdateProblem(svm_problem* problem, svm_node* node, int result);
	void CreateTrainingOrder(double curr_price);
	void CheckOrders(double curr_price);
	void InitSvm();
	void ClearSvm();
};