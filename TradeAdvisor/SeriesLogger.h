// Output series data to file

#pragma once
#include "Global.h"

using namespace std;

class SeriesLogger
{
public:
	SeriesLogger(string file_name, vector<long>* time_series, vector<double>* d1 = 0, vector<double>* d2 = 0, vector<double>* d3 = 0, vector<double>* d4 = 0);
	SeriesLogger(string file_name, vector<string>* data);
	~SeriesLogger();
	void SaveSeries();

private:
	vector<string>* m_data;
	string m_file_name;
	bool m_delete_data;
};