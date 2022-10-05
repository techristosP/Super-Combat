#pragma once
#include <string>
#include "defines.h"
#include "status_effect.h"
#include "sgg/graphics.h"
#include "fighter.h"


class Fighter;
class StatusEffect;

class SpecialAbility
{
protected:
	StatusEffect* m_effect;

private:
	const std::string m_ability_name;
	const std::string m_abiliy_image;
	const std::string m_ability_desc;
	bool m_active;	// ability is hovered.
	bool m_usable;	// ability can be used.
	float m_ability_pos[2] = { CANVAS_WIDTH * (1.0f / 4.0f), CANVAS_HEIGHT / 4 };
	int m_cooldown = 0; 
	const int m_cooldown_duration;

public:
	virtual void activate(Fighter* f) = 0;

	void draw();
	inline void setActive(bool active) { m_active = active; }
	bool contains(float mx, float my) const;
	
	inline bool getUsable() const { return m_usable; }

	void setPosX(float pos_x);
	void setPosY(float pos_y);
	inline void setUsable(bool usable) { m_usable = usable; }

	inline void resetCooldown() { m_cooldown = m_cooldown_duration; } // CHANGE IF ABILITIES HAVE DIFFERENT COOLDOWNS
	inline void decreaseCooldown() { m_cooldown -= 1; }

	void updateCooldown();
													
	SpecialAbility(StatusEffect* ef, const std::string& ability_name, int cooldown_duration);
	virtual ~SpecialAbility();

};


class DelayedAbility : public SpecialAbility
{
public:
	DelayedAbility(Fighter* self, StatusEffect* ef, const std::string& name, int cooldown_duration);
	virtual ~DelayedAbility() {};

	// A delayed ability takes effect on the next turn of the same player that activated the ability.
	virtual void activate(Fighter* self) override;
};


class ImmediateAbility : public SpecialAbility
{
public:
	ImmediateAbility(Fighter* self, StatusEffect* ef, const std::string& name, int cooldown_duration);
	virtual ~ImmediateAbility() {};

	// An immediate ability takes effect at the following turn of the opponent, that is, it happens instantly.
	virtual void activate(Fighter* self) override;
};



