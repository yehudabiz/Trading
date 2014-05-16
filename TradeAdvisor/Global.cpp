#include "Global.h"
// Common functions

void WriteToLog(ZLOG_CONTEXT* log, string msg)
{
	ZLOG_Put(*log, ZLOG_FAC_DEFAULT,ZLOG_LEVEL_DEBUG,0,0,&msg[0],0,0,0);
}

string ConvertToString(double x)
{
	stringstream str;
	str << x;
	return str.str();
}

string ConvertTimeToString(long longtm)
{
	time_t time = longtm;
	tm* t = gmtime(&time);	
	return asctime(t);
}
