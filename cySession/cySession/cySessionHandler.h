#pragma once
#include "cyUsedTime.h"

using namespace System::Collections::Generic;
using namespace cyPaym;

namespace cySession {

	public ref class cySessionHandler {
	private:
		void tariff_changingTariffCost(cyTariff ^tariff);
		void tariff_changedTariffCost(cyTariff ^tariff);

	protected:
		List<cyUsedTime^> ^_timeList;
		int _cacheUsedTime;
		float _cacheUsedMoney;
		bool _activeIsSet;
		bool _activeWasSet;
		cyUsedTime ^_active;
		cyTariff ^_tariff;
		bool _isPrepaid;
		cyBill ^_bill;

		void initializeObj(cyTariff ^tariff);
	public:
		cySessionHandler(cyTariff ^tariff);
		cySessionHandler(cyTariff ^tariff, cyBill ^bill);
		void startNewTime(void);
		void addTime(cyUsedTime ^time);
		void closeActive(void);
		void closeActive(DateTime time);

		int getTotalTime(void);
		float getTotalMoney(void);
		int getAvailableTime(void);
		bool timeIsOver(void);

		virtual ~cySessionHandler(void);

		property bool isPrepaid {
			bool get () {
				return this->_isPrepaid;
			}
		}
	};
}
