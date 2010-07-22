#include "StdAfx.h"
#include "cyBill.h"

cyBill::cyBill(float amount) {
	this->_amount = amount;
}
void cyBill::setMoney(float amount) {
	this->_amount = amount;
}
void cyBill::addMoney(float amount) {
	if (amount <= 0) {
		return;
	}
	this->_amount += amount;
}
float cyBill::burnMoney(float amount) {
	if (amount > this->_amount) {
		float _t = this->_amount;
		this->_amount = 0;
		return amount - _t;
	}
	this->_amount -= amount;
	return 0;
}
float cyBill::getAvailable(void) {
	return this->_amount;
}
