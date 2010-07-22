#pragma once

public ref class cyBill
{
protected:
	float _amount;
	
public:
	cyBill(float amount);
	void setMoney(float amount);
	void addMoney(float amount);
	float burnMoney(float amount);
	float getAvailable(void);
};
