#pragma once

using namespace System;
using namespace cyPaym;

namespace cySession {

	public ref class cyUsedTime
	{
	protected:
		cyTariff ^_tariff;
		int _timeStart;
		int _timeEnd;

		bool _isRunning;
		int _usedTime;
		float _usedMoney;

	public:
		cyUsedTime(cyTariff ^tariff, DateTime timeStart);
		cyUsedTime(cyTariff ^tariff, DateTime timeStart, DateTime timeEnd);
		virtual ~cyUsedTime(void);
		bool isRunning(void) { return this->_isRunning; };
		void close(DateTime time);
		int timeToTicks(DateTime time);
		DateTime ^ticksToTime(int ticks);
		int getUsedTime(void);
		float getUsedMoney();
	};
}
