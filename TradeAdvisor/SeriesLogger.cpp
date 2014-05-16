#pragma once
#include "SeriesLogger.h"
#include <fstream>
#include <sstream>
#include <ctime>

SeriesLogger::SeriesLogger(string file_name, vector<long>* time_series, vector<double>* d1, vector<double>* d2, vector<double>* d3, vector<double>* d4)
{
	m_file_name = file_name;
	time_t time;
	string str_time = "";
	string str_d1 = "";
	string str_d2 = "";
	string str_d3 = "";
	string str_d4 = "";
	stringstream buf;
	m_data = new vector<string>();

	for(int i = 0; i < (int)((*time_series).size()); i++)
	{
		time = (*time_series)[i];
		tm* t = gmtime(&time);	
		str_time = asctime(t);

		if (d1 != 0 && i < (int)((*d1).size()))
		{
			buf << (*d1)[i];
			str_d1 = buf.str();
			buf.str(string());
		}
		if (d2 != 0 && i < (int)((*d2).size()))		
		{
			buf << (*d2)[i];
			str_d2 = buf.str();
			buf.str(string());
		}
		if (d3 != 0 && i < (int)((*d3).size()))		
		{
			buf << (*d3)[i];
			str_d3 = buf.str();
			buf.str(string());
		}
		if (d4 != 0 && i < (int)((*d4).size()))		
		{
			buf << (*d4)[i];
			str_d4 = buf.str();
			buf.str(string());
		}

		m_data->insert(m_data->begin(), str_time + " " + str_d1 + " " + str_d2 + " " + str_d3 + " " + str_d4);
		m_delete_data = true;
	}

}

SeriesLogger::SeriesLogger(string file_name, vector<string>* data)
{
	m_file_name = file_name;
	m_data = data;
	m_delete_data = false;
}

void SeriesLogger::SaveSeries()
{
	fstream fs(m_file_name, ios::out, ios::binary);
	stringstream buffer;

	for(int i = 0; i < (int)(m_data->size()); i++)
	{
		buffer << (*m_data)[i] << endl;
	}
	
	fs << buffer.str();
	fs.close();
}

SeriesLogger::~SeriesLogger()
{
	if (m_delete_data) delete m_data;
}
