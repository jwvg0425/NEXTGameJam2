#pragma once
#include "Unit.h"

class Portal : public Unit
{
public:
	Portal() = default;
	~Portal() override = default;

	bool init(const std::string& nextStage);

	static Portal* create(const std::string& nextStage);

	void update(float dTime);
	
private:
	std::string m_NextStage;
};