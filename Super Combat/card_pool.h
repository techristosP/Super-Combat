#pragma once
#include <array>
#include "card.h"

class CardPool
{
private:
	static CardPool* m_instance;
	std::array<Card*, CARDPOOL_SIZE> m_pool;

	CardPool() : m_pool({
			new AttackCard("Iron Fist", {50, 0}),

			new AttackCard("Enchanted Punch", {50, 20}),

			new AttackCard("Chaos Burst", {0, 40}),

			new AttackCard("Elemental Nova", {0, 80}),

			new AttackCard("Staggering Slash", {90, 0}),

			new AttackCard("Ethereal Swarm", {25, 45}),
		
			new HealCard("Divine Heal", 120),

			new HealCard("Vital Blessing", 60),

			new ShieldCard("Shield of Judgment", 80),

			new ShieldCard("Shield Potion", 45)
		})
	{}

	~CardPool();

public:
	static CardPool* getInstance();
	static void releaseInstance();

	// Returns a copy of a random card from the card pool.
	static Card* drawCard();
};

