#include "stdafx.h"

#include "cyUsedTime.h"
using namespace cySession;

void cyUsedTime::initializeObj(cyTariff ^tariff, DateTime timeStart) {
	this->_tariff = tariff;
	this->_timeStart = this->timeToTicks(timeStart);
	this->_isRunning = true;
	this->_timeEnd = 0;
	this->_usedTime = 0;
	this->_usedMoney = 0;
}
cyUsedTime::cyUsedTime(cyTariff ^tariff, DateTime timeStart) {
	this->initializeObj(tariff, timeStart);
}
cyUsedTime::cyUsedTime(cyTariff ^tariff, DateTime timeStart, DateTime timeEnd) {
	this->initializeObj(tariff, timeStart);
	this->close(timeEnd);
}
cyUsedTime::~cyUsedTime(void) {

}
void cyUsedTime::close(DateTime time) {
	if (!this->_isRunning) {
		throw gcnew Exception("Can't close interval: Interval is NOT running.");
	}
	this->_timeEnd = this->timeToTicks(time);

	this->_usedTime = this->getUsedTime();
	this->_usedMoney = this->getUsedMoney();
	this->_isRunning = false;
}
int  cyUsedTime::timeToTicks(DateTime time) {
	int dif = Convert::ToInt32(((TimeSpan)DateTime::Now.Subtract(time)).TotalSeconds);
	return (Environment::TickCount / 1000) - dif;
}
DateTime ^cyUsedTime::ticksToTime(int ticks){
	int df = (Environment::TickCount / 1000) - ticks;
	return DateTime::Now.Subtract(TimeSpan::FromSeconds(df));
}
int cyUsedTime::getUsedTime() {
	if (this->_isRunning) {
		if (this->_timeEnd > 0) {
			return this->_timeEnd - this->_timeStart;
		}
		return (Environment::TickCount / 1000) - this->_timeStart;
	}
	return this->_usedTime;
}
float cyUsedTime::getUsedMoney() {
	if (this->_isRunning) {
		return this->_tariff->timeToMoney(this->getUsedTime());
	}
	return this->_usedMoney;
}
