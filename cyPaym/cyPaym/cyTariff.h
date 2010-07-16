#pragma once

using namespace System;

namespace cyPaym {

	ref class cyTariff;
	public delegate void TariffEvent(cyTariff ^tariff);

	public ref class cyTariff
	{
	protected:
		String ^_name;
		float _cost;

	public:
		virtual event TariffEvent^ changingTariffCost;
		virtual event TariffEvent^ changedTariffCost;

		cyTariff(String ^name, float cost);
		float timeToMoney(int seconds);
		int moneyToTime(float money);

		property String ^name {
			String ^get() {
				return this->_name;
			}
			void set(String ^value) {
				this->_name = value;
			}
		}
		property float cost {
			float get() {
				return this->_cost;
			}
			void set(float value) {
				this->changingTariffCost(this);
				this->_cost = value;
				this->changedTariffCost(this);
			}
		}
	};
}
