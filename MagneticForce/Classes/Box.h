#pragma once
#include "Unit.h"

class Box : public Unit
{
public:
	Box() = default;
	~Box() override = default;

	bool init() override;

	CREATE_FUNC(Box);

	void update(float dTime);

private:
};