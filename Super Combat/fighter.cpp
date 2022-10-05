#include "fighter.h"
#include "fighter_stats_bar.h"


void Fighter::draw()
{	
	// DRAW HP AND SHIELD BAR (if fighters alive)
	if (m_alive && m_opponent->getLifeState())
		m_statsBar->draw();

	graphics::Brush br;

	// DRAW PLAYER
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	br.texture = ASSET_PATH + std::string("fighters\\") + m_fighter_image;
	br.outline_opacity = 0.0f;
	
	// Draw winner in Game Over state.
	if (m_alive && !m_opponent->getLifeState())
	{
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, PLAYER_SIZE, PLAYER_SIZE, br);
		return;
	}

	// Draw fighters in arena (if both alive).
	if (m_alive && m_opponent->getLifeState())
	{
		// Used for floating effect of active fighter.
		float y = (m_active) ? m_pos[1] + soarFighter() : m_pos[1];
		graphics::drawRect(m_pos[0], y, PLAYER_SIZE, PLAYER_SIZE, br);

		// DRAW EVENTS
		for (auto e : m_events)
		{
			e->draw();
		}

		// DRAW SPECIAL ABILITIES
		m_special1->draw();
		m_special2->draw();

		// DRAW ACTIVE STATUS EFFECTS (red circle around ability if active).
		for (auto ef : m_status_effects)
		{
			ef->draw();
		}

		// DRAW CARDS IN HAND
		drawHand(0, false);
	}
}

// Dynamically draws cards based on fighter's position.
void Fighter::drawHand(int firstCardPos, bool leftPositioning)
{
	int i = firstCardPos;
	for (Card* card : m_hand) {
		if (card) {
			card->adjustPosX(i);
			card->draw(leftPositioning);
			i++;
		}

	}
}

// Soar effect (active fighter).
float Fighter::soarFighter()
{
	static float d = 0.f, step = 0.004f;

	if (d <= -0.4f)
		step = (-1) * step;
	if (d >= 0.4f)
		step = (-1) * step;

	d += step;

	return d;
}

// Returns true if fighter's turn is completed (card played or ablility activated).
bool Fighter::update(const graphics::MouseState& ms, float mx, float my)
{
	Card* currentCard = nullptr;
	// Finds hovered card and sets active (rest cards set to inactive).
	for (int i = 0; i < HAND_SIZE; i++) 
	{
		if (m_hand[i]->contains(mx, my))
		{
			m_hand[i]->setActive(true);
			currentCard = m_hand[i];
		}
		else
		{
			m_hand[i]->setActive(false);
		}
	}

	SpecialAbility* currentAbility = nullptr;
	// Finds hovered ability (if it's usable) and sets active (other abilitie(s) set to inactive).
	if (m_special1->getUsable() && m_special1->contains(mx, my)) 
	{
		m_special1->setActive(true);
		currentAbility = m_special1;
		m_special2->setActive(false);
	}
	else if(m_special2->getUsable() && m_special2->contains(mx, my))
	{
		m_special2->setActive(true);
		currentAbility = m_special2;
		m_special1->setActive(false);
	}
	else 
	{
		m_special1->setActive(false);
		m_special2->setActive(false);
	}
	
	// CHECKS IF CARD WAS CLICKED AND USES IT
	if (ms.button_left_released && currentCard) 
	{
		useCard(currentCard);
		for (int i = 0; i < HAND_SIZE; i++)
		{
			if (m_hand[i] == currentCard)
				m_hand[i] = nullptr;
		}
		delete currentCard; // Used card removed from hand and delete.
	
		return true;
	}

	// CHECK IF USABLE ABILITY WAS CLICKED AND ACTIVATES IT
	if (ms.button_left_released && currentAbility)
	{
		currentAbility->activate(this);
		event::addEvent(m_events, new SoundEvent("ability", 0.3f));

		return true;
	}

	return false;
}


// GAME MECHANICS
void Fighter::useCard(const Card* card)
{
	/*
	* cardStats[0]: physical damage amount
	* cardStats[1]: magic damage amount
	* cardStats[2]: shield amount
	* cardStats[3]: heal amount
	*/
	stats_array_t cardStats = card->getStats();
	m_opponent->decreaseHPandSHIELD(cardStats[0], cardStats[1]);
	increaseSHIELD(cardStats[2]);
	increaseHP(cardStats[3]);

	// Attack Card
	if (cardStats[2] == 0 && cardStats[3] == 0)
	{
		event::addEvent(m_opponent->m_events, new DamageEvent(m_opponent->getPosX(), m_opponent->getPosY()));
		event::addEvent(m_opponent->m_events, new ShakeEvent(m_opponent));
		std::string attack = "attack_" + std::to_string(static_cast<int>(interpolation(1, 3, RAND0TO1())));
		event::addEvent(m_opponent->m_events, new SoundEvent(attack, 0.3f));
	}

	// Shield Card
	if (cardStats[2] != 0)
	{
		event::addEvent(m_events, new ShieldEvent(getPosX(), getPosY()));
		event::addEvent(m_events, new SoundEvent("shield"));
	}

	// Heal Card
	if (cardStats[3] != 0)
	{
		event::addEvent(m_events, new HealEvent(getPosX(), getPosY()));
		event::addEvent(m_events, new SoundEvent("heal"));
	}	
}

void Fighter::decreaseHPandSHIELD(int physDMG, int magicDMG)
{
	// Magic damage penetrates shield (if active)
	if (magicDMG > 0)
	{
		m_rem_hp -= magicDMG;
	}

	if (m_rem_shield > 0) 
	{
		m_rem_shield -= physDMG;
		if (m_rem_shield <= 0)			// CASE: shield is destroyed.
		{
			m_rem_shield = 0;
			m_shield_active = false;	
		}
	}
	else 
	{
		m_rem_hp -= physDMG;
	}

	if (m_rem_hp <= 0)					// CASE: fighter dies.
	{
		m_rem_hp = 0;
		m_alive = false;
	}

	// Draw magic damage if dealt.
	if (magicDMG)
	{
		event::addEvent(m_events, new MagicDamageIndicator(magicDMG, m_pos[0], m_pos[1]));
	}
	// Draw physical damage if dealt.
	if (physDMG)
	{
		event::addEvent(m_events, new PhysDamageIndicator(physDMG, m_pos[0], m_pos[1]));
	}
}

void Fighter::increaseHP(int lifePoints)
{
	m_rem_hp = (m_rem_hp + lifePoints > m_max_hp) ? m_max_hp : m_rem_hp + lifePoints;

	// Draw heal amount gained.
	if (lifePoints)
	{
		event::addEvent(m_events, new HealIndicator(lifePoints, m_pos[0], m_pos[1]));
	}
}

void Fighter:: increaseSHIELD(int shield)
{
	m_rem_shield = (m_rem_shield + shield > m_max_shield) ? m_max_shield : m_rem_shield + shield;
	if (m_rem_shield > 0)
	{
		m_shield_active = true;
	}

	// Draw shield amount gained.
	if (shield)
	{
		event::addEvent(m_events, new ShieldIndicator(shield, m_pos[0], m_pos[1]));
	}
}

// If empty card slot in hand adds new card from CardPool.
void Fighter::fillHand()
{
	CardPool* card_pool = CardPool::getInstance();

	for (int i = 0; i < HAND_SIZE; i++) 
	{
		if (!m_hand[i]) 
		{
			m_hand[i] = card_pool->drawCard();
		}
	}
}


void Fighter::updateAbilityCooldowns()
{
	m_special1->updateCooldown();
	m_special2->updateCooldown();
}


void Fighter::processEvents()
{
	event::processEvents(m_events);
}

void Fighter::updateStatusEffects()
{
	effect::updateStatusEffects(m_status_effects, this);
}

void Fighter::addStatusEffect(StatusEffect* status_effect)
{
	effect::addStatusEffect(m_status_effects, status_effect);
}


// CONSTRUCTORS - DESTRUCTORS

Fighter::Fighter(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2)
	:	m_fighter_name(fighter_name), 
		m_fighter_image(fighter_name+".png"),
		m_max_hp(max_hp),
		m_rem_hp(max_hp),
		m_special1(special1),
		m_special2(special2)
{}

Fighter::~Fighter()
{
	delete m_hand[0];
	delete m_hand[1];
	delete m_hand[2];
	delete m_statsBar;

	delete m_special1;
	delete m_special2;
	effect::clearStatusEffects(m_status_effects);
	event::clearEvents(m_events);
}

Villain::Villain(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2)
	:   Fighter(fighter_name, max_hp, special1, special2)
{
	m_pos[0] *= 1;		// left positioning in arena
	m_statsBar = new VillainStatsBar(this);

	// Sets positions of special abilities.
	m_special1->setPosX(4.0f); m_special1->setPosY(SPECIAL_ABILITY_1_H);
	m_special2->setPosX(3.0f); m_special2->setPosY(SPECIAL_ABILITY_2_H);
}


Hero::Hero(const std::string& fighter_name, int max_hp, SpecialAbility* special1, SpecialAbility* special2)
	:	Fighter(fighter_name, max_hp, special1, special2)

{
	m_pos[0] *= 3;		// right positioning in arena
	m_statsBar = new HeroStatsBar(this);
	
	// Sets positions of special abilities.
	m_special1->setPosX(CANVAS_WIDTH - 4.0f); m_special1->setPosY(SPECIAL_ABILITY_1_H);
	m_special2->setPosX(CANVAS_WIDTH - 3.0f); m_special2->setPosY(SPECIAL_ABILITY_2_H);
}

