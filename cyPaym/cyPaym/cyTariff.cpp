#include "stdafx.h"

#include "cyTariff.h"
using namespace cyPaym;

cyTariff::cyTariff(String ^name, float cost) {
	this->_name = name;
	this->_cost = cost;
}

float cyTariff::timeToMoney(int seconds) {
	return (this->_cost / 3600) * seconds;
}
int cyTariff::moneyToTime(float amount) {
	return (this->_cost <= 0) ? 0 :
		(int)(amount / (this->_cost / 3600));
}
