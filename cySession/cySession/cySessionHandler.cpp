#include "StdAfx.h"

#include "cySessionHandler.h"
using namespace cySession;
using namespace cyPaym;

void cySessionHandler::tariff_changingTariffCost(cyTariff ^tariff) {
	if (this->_activeIsSet) {
		this->_activeWasSet = true;
		this->closeActive();
	}
}
void cySessionHandler::tariff_changedTariffCost(cyTariff ^tariff) {
	if (this->_activeWasSet) {
		this->_activeWasSet = false;
		this->startNewTime();
	}
}
void cySessionHandler::initializeObj(cyTariff ^tariff) {
	this->_timeList = gcnew List<cyUsedTime^>();
	this->_activeIsSet = false;
	this->_activeWasSet = false;
	this->_tariff = tariff;
	this->_isPrepaid = false;
	this->_tariff->changingTariffCost::add(gcnew TariffEvent(this, &cySessionHandler::tariff_changingTariffCost));
	this->_tariff->changedTariffCost::add(gcnew TariffEvent(this, &cySessionHandler::tariff_changedTariffCost));
}
cySessionHandler::cySessionHandler(cyTariff ^tariff) {
	this->initializeObj(tariff);
}
cySessionHandler::cySessionHandler(cyTariff ^tariff, cyBill ^bill) {
	this->initializeObj(tariff);
	this->_isPrepaid = true;
	this->_bill = bill;
}
void cySessionHandler::startNewTime(void) {
	if (this->_activeIsSet) {
		this->closeActive();
	}
	this->addTime(gcnew cyUsedTime(this->_tariff, DateTime::Now));
}
void cySessionHandler::closeActive(DateTime time) {
	this->_active->close(time);
	this->addTime(this->_active);
	this->_activeIsSet = false;
	delete this->_active; // ??
}
void cySessionHandler::closeActive() {
	this->closeActive(DateTime::Now);
}
void cySessionHandler::addTime(cyUsedTime ^time) {
	if (time->isRunning()) {
		if (this->_activeIsSet) {
			throw gcnew Exception("Just one interval could be active at any time for a single session.");
		}
		this->_active = time;
		this->_activeIsSet = true;
	} else {
		this->_timeList->Add(time);
		this->_cacheUsedTime += time->getUsedTime();
		this->_cacheUsedMoney += time->getUsedMoney();
	}
}
int cySessionHandler::getTotalTime(void) {
	return this->_cacheUsedTime + (this->_activeIsSet ? _active->getUsedTime() : 0);
}
int cySessionHandler::getAvailableTime(void) {
	if (!this->_isPrepaid) {
		return 0;
	}
	return this->_tariff->moneyToTime(this->_bill->getAvailable()) - this->getTotalTime();
}
float cySessionHandler::getTotalMoney(void) {
	return this->_cacheUsedMoney + (this->_activeIsSet ? _active->getUsedMoney() : 0);
}
cySessionHandler::~cySessionHandler(void)
{
}
