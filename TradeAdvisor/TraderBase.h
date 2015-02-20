#pragma once
#include "Bar.h"
#include "Order.h"

class TraderBase
{
public:
	virtual void UpdatePrice(Bar bar) = 0;
	virtual void UpdateOrder(Order bar) = 0;
	virtual void AskAdvise(int& advise, double& stop_loss, double& take_profit, long& timeout) = 0;
	virtual void Deinit() = 0;
};