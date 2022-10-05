#pragma once
#include "defines.h"
#include "sgg/graphics.h"


class FighterStatsBar
{
	class Fighter* m_fighter;

protected:

	float m_HP_bar_pos[2]{};
	float m_rem_HP_bar_pos[2]{};

	float m_SHIELD_bar_pos[2]{};
	float m_rem_SHIELD_bar_pos[2]{};

	virtual float rem_HP_bar_length();
	virtual float rem_SHIELD_bar_length();


public:
	FighterStatsBar(Fighter* f);
	virtual ~FighterStatsBar() {}

	// Draws fighter's hp bar and shield bar.
	void draw();
};

class VillainStatsBar : public FighterStatsBar
{
public:
	VillainStatsBar(Fighter* f);

	virtual ~VillainStatsBar() {};

	virtual float rem_HP_bar_length() override ;
	virtual float rem_SHIELD_bar_length() override;
};

class HeroStatsBar : public FighterStatsBar
{
public:
	HeroStatsBar(Fighter* f);
	virtual ~HeroStatsBar(){};

	virtual float rem_HP_bar_length() override;
	virtual float rem_SHIELD_bar_length() override;

};
