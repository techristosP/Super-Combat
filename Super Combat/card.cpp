#include "card.h"


// If left = true then draw cardBox displaying card stats to the left (villain), else draw right (hero).
void Card::draw(bool left)
{	
	float boxWidthCenter = CANVAS_WIDTH * (left ? 2 / 9.0f : 7 / 9.0f);

	graphics::Brush br, br2;

	if (m_active) {
		// CARD BOX (STATS)
		br2.outline_opacity = 0.0f;
		drawCardStats(boxWidthCenter, br2);

		// DRAW CARD'S NAME
		graphics::drawText(boxWidthCenter - 3.0f, CARD_BOX_HEIGHT_CENTER - 0.6f, 0.5f, m_card_name, br2);
		
		// HOVERED CARD 
		br.outline_opacity = 1.0f;
		SETCOLOR(br.outline_color, 1.0f, .0f, .0f);
	}

	// DRAW CARD
	br.texture = ASSET_PATH + m_card_image;
	br.outline_width = 0.5f;
	graphics::drawRect(m_card_pos[0], m_card_pos[1], CARD_WIDTH, CARD_HEIGHT, br);
	
}

// Draw cardBox displaying attack card's stats.
void AttackCard::drawCardStats(float x, graphics::Brush& br) 
{
	br.texture = ASSET_PATH + std::string("attack_card_box.png");
	graphics::drawRect(x, CARD_BOX_HEIGHT_CENTER, 3 * CARD_WIDTH, CARD_HEIGHT, br);

	SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
	stats_array_t c_stats = getStats();
	// Draw magic damage ammount.
	graphics::drawText(x + 1.0f, CARD_BOX_STAT_1_Y, CARD_BOX_STATS_FONT_SIZE, std::to_string(c_stats[1]), br);
	// Draw physical damage ammount.
	graphics::drawText(x + 1.0f, CARD_BOX_STAT_2_Y, CARD_BOX_STATS_FONT_SIZE, std::to_string(c_stats[0]), br);
}

// Draw cardBox displaying shield card's stats.
void ShieldCard::drawCardStats(float x, graphics::Brush& br)
{
	br.texture = ASSET_PATH + std::string("shield_card_box.png");
	graphics::drawRect(x, CARD_BOX_HEIGHT_CENTER, 3 * CARD_WIDTH, CARD_HEIGHT, br);

	SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
	stats_array_t c_stats = getStats();
	// Draw shield amount.
	graphics::drawText(x + 1.0f, CARD_BOX_STAT_1_Y, CARD_BOX_STATS_FONT_SIZE, std::to_string(c_stats[2]), br);
}

// Draw cardBox displaying heal card's stats.
void HealCard::drawCardStats(float x, graphics::Brush& br)
{
	br.texture = ASSET_PATH + std::string("heal_card_box.png");
	graphics::drawRect(x, CARD_BOX_HEIGHT_CENTER, 3 * CARD_WIDTH, CARD_HEIGHT, br);

	SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
	stats_array_t c_stats = getStats();
	// Draw heal amount.
	graphics::drawText(x + 1.0f, CARD_BOX_STAT_1_Y, CARD_BOX_STATS_FONT_SIZE, std::to_string(c_stats[3]), br);
}



bool Card::contains(float mx, float my) const
{
	if (mx >= m_card_pos[0] - CARD_WIDTH/2 && mx <= m_card_pos[0] + CARD_WIDTH/2)
	{
		if (my >= m_card_pos[1] - CARD_HEIGHT/2 && my <= m_card_pos[1] + CARD_HEIGHT/2)
		{
			return true;
		}
	}	
	return false;
}

// Determines the drawing position of the card dynamically.
void Card::adjustPosX(int i)
{
	m_card_pos[0] = CANVAS_WIDTH * (i / 9.0f);
}



// Get card stats as array.
stats_array_t AttackCard::getStats() const
{
	stats_array_t cardStats{ m_physical_damage, m_magic_damage, 0, 0 };
	return cardStats;
}

stats_array_t ShieldCard::getStats() const
{
	stats_array_t cardStats{ 0, 0, m_shield_amt, 0 };
	return cardStats;
}

stats_array_t HealCard::getStats() const
{
	stats_array_t cardStats{ 0, 0, 0, m_heal_amt };
	return cardStats;
}



// CONSTRUCTORS - DESTRUCTORS

Card::Card(const std::string& name)
:	m_card_name(name), m_card_image(name + ".png")
{}

Card::Card(const Card& proto)
:	m_card_name(proto.m_card_name),
	m_card_image(proto.m_card_image),
	m_active(false)
{}


AttackCard::AttackCard(const std::string& name, const int(&dmg)[2])
	: Card(name), m_physical_damage(dmg[0]), m_magic_damage(dmg[1])
{}

AttackCard::AttackCard(const AttackCard& proto)
	: Card(proto), m_physical_damage(proto.m_physical_damage), m_magic_damage(proto.m_magic_damage)
{}


ShieldCard::ShieldCard(const std::string& name, int shield_amt)
	: Card(name), m_shield_amt(shield_amt)
{}

ShieldCard::ShieldCard(const ShieldCard& proto)
	: Card(proto), m_shield_amt(proto.m_shield_amt)
{}


HealCard::HealCard(const std::string& name, int heal_amt)
	: Card(name), m_heal_amt(heal_amt)
{}

HealCard::HealCard(const HealCard& proto)
	: Card(proto), m_heal_amt(proto.m_heal_amt)
{}

