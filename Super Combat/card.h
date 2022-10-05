#pragma once
#include <sgg/graphics.h>
#include <string>
#include <array>
#include "defines.h"


class Card
{
	const std::string m_card_name;
	const std::string m_card_image;
	bool m_active = false;

	float m_card_pos[2] = { CANVAS_WIDTH / 9.0f, CARD_HEIGHT_CENTER };

protected:
	Card(const std::string& name);
	Card(const Card& proto);

public:
	// If left = true then draw cardBox displaying card stats to the left (villain), else draw right (hero).
	void draw(bool left);
	virtual void drawCardStats(float x, graphics::Brush& br) = 0;

	bool contains(float mx, float my) const;

	// Determines the drawing position of the card dynamically.
	void adjustPosX(int i);
	inline void setActive(bool active) { m_active = active; }

	// Get card stats as array.
	virtual stats_array_t getStats() const = 0;

	virtual ~Card() {};

};

class AttackCard : public Card
{
	int m_physical_damage;
	int m_magic_damage;

public:
	AttackCard(const std::string& name, const int (&dmg)[2]);
	AttackCard(const AttackCard& proto);

	virtual void drawCardStats(float x, graphics::Brush& br) override;
	virtual stats_array_t getStats() const override;

	virtual ~AttackCard() {};
};

class ShieldCard : public Card
{
	int m_shield_amt;

public:
	ShieldCard(const std::string& name, int shield_amt);
	ShieldCard(const ShieldCard& proto);

	virtual void drawCardStats(float x, graphics::Brush& br) override;
	virtual stats_array_t getStats() const override;

	virtual ~ShieldCard() {};
};

class HealCard : public Card
{
	int m_heal_amt;

public:
	HealCard(const std::string& name, int heal_amt);
	HealCard(const HealCard& proto);

	virtual void drawCardStats(float x, graphics::Brush& br) override;
	virtual stats_array_t getStats() const override;

	virtual ~HealCard() {};
};

