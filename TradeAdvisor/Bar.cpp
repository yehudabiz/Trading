#pragma once
#include "Bar.h"

Bar::Bar(long time, double high, double low, double open, double close)
{
	m_time = time;
	m_high = high;
	m_low = low;
	m_open = open;
	m_close = close;
}
