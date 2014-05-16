#pragma once

class Bar
{
public:
	Bar(long time, double high, double low, double open, double close);

	long m_time;
	double m_high;
	double m_low;
	double m_open; 
	double m_close;
};