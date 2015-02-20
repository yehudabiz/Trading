#pragma once
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "ZLOG.H"
#include <ctime>

using namespace std;

// Timeframe periods
#define PERIOD_TICKS	0
#define PERIOD_M1		1
#define PERIOD_M5		5
#define PERIOD_M15		15
#define PERIOD_M30		30
#define PERIOD_H1		60
#define PERIOD_H4		240
#define PERIOD_D1		1440
#define PERIOD_W1		10080
#define PERIOD_MN1		43200

//  Moving Average methods
#define MODE_SMA	0	// Simple moving average
#define MODE_EMA	1	// Exponential moving average
#define MODE_SMMA	2	// Smoothed moving average
#define MODE_LWMA	3	// Linear weighted moving average

// Applied price constants
#define PRICE_CLOSE		0
#define PRICE_OPEN		1
#define PRICE_HIGH		2
#define PRICE_LOW		3
#define PRICE_MEDIAN	4	// (high+low)/2
#define PRICE_TYPICAL	5	// (high+low+close)/3
#define PRICE_WEIGHTED	6	// (high+low+close+close)/4

//  Data update results
#define UPDATE_LAST		0	// The last bar was updated
#define UPDATE_NEW		1	// New bar was added
#define UPDATE_ERROR	-1	// Update failed

// Advise enumerator
#define DO_NOTHING		0
#define SELL			1
#define BUY				2
#define CLOSE_LONG		4
#define CLOSE_SHORT		8
#define SELL_CONTINUE	16
#define BUY_CONTINUE	32
#define TRAINING_ORDER	64

// Order states
#define ORDER_STATE_OPEN	1
#define ORDER_STATE_CLOSED	2

// Order types
#define ORDER_TYPE_LONG		1
#define ORDER_TYPE_SHORT	-1

// Symbols
#define EURUSD			1
#define USDCHF			2
#define USDJPY			3

// Market types
#define TREND_UP		1
#define TREND_DOWN		2
#define NO_TREND		3

// SVM modes
#define SVM_DISABLED	0
#define SVM_TRAINING	1
#define SVM_ADVISING	2
#define SVM_PROCESSING	3

// Trader id
#define RSI_STOCH		12


// Common functions

void WriteToLog(ZLOG_CONTEXT* log, string msg);
string ConvertToString(double x);
string ConvertTimeToString(long longtm);
