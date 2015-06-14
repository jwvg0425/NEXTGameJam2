#pragma once
#include "Unit.h"

class Thorn : public Unit
{
public:
	Thorn() = default;
	~Thorn() override = default;

	CREATE_FUNC(Thorn);

	bool init() override;

	void update(float dTime);

	void changeOnOff();

private:
	bool m_IsOn = false;
};