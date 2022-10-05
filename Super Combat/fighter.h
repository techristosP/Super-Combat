#pragma once
#include <array>
#include <list>
#include <string>
#include "sgg/graphics.h"
#include "card.h"
#include "card_pool.h"
#include "special_ability.h"
#include "status_effect.h"
#include "event.h"
#include "defines.h"


class SpecialAbility;
class StatusEffect;

class Fighter
{
	Fighter* m_opponent;
	const std::string m_fighter_name;
	const std::string m_fighter_image;

	bool m_alive = true;
	bool m_active = false;

	// Soar effect (active fighter).
	float soarFighter();

protected:
	Fighter(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2);

	float m_pos[2] = { CANVAS_WIDTH * (1.0f / 4.0f), CANVAS_HEIGHT / 4 };

	const float m_init_posx = CANVAS_WIDTH * (1.0f / 4.0f);

	std::list<Event*> m_events;
	std::list<StatusEffect*> m_status_effects;

	// STATSBAR
	class FighterStatsBar* m_statsBar;

	// HEALTH
	const int m_max_hp;
	int m_rem_hp;

	// SHIELD
	bool m_shield_active = false;	
	const int m_max_shield = MAX_SHIELD;
	int m_rem_shield = 0;

	// SPECIAL ABILITIES
	SpecialAbility* const m_special1;
	SpecialAbility* const m_special2;

	// CARDS
	Card* m_hand[HAND_SIZE];
	Card* currentCard;

	// Dynamically draws cards based on fighter's position.
	virtual void drawHand(int firstCardPos, bool leftPositioning);

public:

	void draw();

	// Returns true if fighter's turn is completed (card played or ablility activated).
	bool update(const graphics::MouseState& ms, float mx, float my);

	inline float getPosX() const { return m_pos[0]; }
	inline float getPosY() const { return m_pos[1]; }
	inline std::string getName() const { return m_fighter_name; }

	inline int get_max_HP() const { return m_max_hp; }
	inline int get_rem_HP() const { return m_rem_hp; }

	// Used to draw shield bar if fighter has active shield.
	inline bool get_SHIELD_ACTIVE() const { return m_shield_active;  }
	inline int get_max_SHIELD() const { return m_max_shield; }
	inline int get_rem_SHIELD() const { return m_rem_shield; }

	inline void toggleActive() { m_active = !m_active; }
	inline bool getLifeState() const { return m_alive; }
	inline std::list<Event*>& getEventList() { return m_events; }
	inline Fighter* getOpponent() const { return m_opponent; }

	// GAME MECHANICS
	void useCard(const Card* card);

	void decreaseHPandSHIELD(int physDMG, int magicDMG);
	void increaseHP(int lifePoints);
	void increaseSHIELD(int shield);

	// If empty card slot in hand adds new card from CardPool.
	void fillHand();

	inline void setOpponent(Fighter* opponent) { m_opponent = opponent; }

	void updateAbilityCooldowns();
	
	void processEvents(); 
	// Loops through status effects list, activates effects and deletes any effect turned inactive.
	void updateStatusEffects(); 
	void addStatusEffect(StatusEffect* status_effect);
	
	inline void setPosX(float x) { m_pos[0] = x; }
	inline void setPosY(float y) { m_pos[1] = y; }

	virtual ~Fighter();

	virtual float getInitialPosX() = 0;
};


class Villain : public Fighter {
public:

	Villain(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2);

	virtual ~Villain() {};

	virtual inline float getInitialPosX() override { return m_init_posx * 1; }

private:
	virtual inline void drawHand(int firstCardPos, bool leftPositioning) override
	{
		Fighter::drawHand(this->m_firstCardPos, true);
	}

	int m_firstCardPos = 1;
};


class Hero : public Fighter {
public:

	Hero(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2);

	virtual ~Hero() {};

	virtual inline float getInitialPosX() override { return m_init_posx * 3; }

private:
	virtual inline void drawHand(int firstCardPos, bool leftPositioning) override
	{
		Fighter::drawHand(this->m_firstCardPos, false);
	}

	int m_firstCardPos = 6;
};



