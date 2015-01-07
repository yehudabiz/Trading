#pragma once

class Order
{
public:
	Order(long id, int symbol, long open_time, long close_time, int order_type, int state, double entry_price, double close_price, double pl) 
	{
		m_id = id;
		m_symbol = symbol;
		m_open_time = open_time;
		m_close_time = close_time;
		m_order_type = order_type;
		m_state = state;
		m_entry_price = entry_price;
		m_close_price = close_price;
		m_pl = pl;
	};

	long m_id;
	int m_symbol;
	long m_open_time;
	long m_close_time;
	int m_order_type;
	int m_state;
	double m_entry_price; 
	double m_close_price; 
	double m_sl; 
	double m_tp; 
	double m_pl; 

	 
};