#include "fighter_stats_bar.h"
#include "fighter.h"


void FighterStatsBar::draw()
{
	graphics::Brush br;

	// DRAW HEALTHBAR (white part of hp bar)
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawRect(m_HP_bar_pos[0], m_HP_bar_pos[1], 8, 0.5f, br);

	// remaining hp / max hp
	SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
	graphics::drawText(m_HP_bar_pos[0] - 0.75f, m_HP_bar_pos[1] + 0.1f, 0.4f, std::to_string(m_fighter->get_rem_HP()) + std::string(" / ") + std::to_string(m_fighter->get_max_HP()), br);

	// DRAW REMAINING HEALTH (red part of hp bar)
	SETCOLOR(br.fill_color, 1.0f, 0.0f, 0.0f);
	graphics::drawRect(m_rem_HP_bar_pos[0], m_rem_HP_bar_pos[1], rem_HP_bar_length(), 0.5f, br);


	if (m_fighter->get_SHIELD_ACTIVE()) {
		// DRAW SHIELDBAR (white part of shield bar)
		SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
		graphics::drawRect(m_SHIELD_bar_pos[0], m_SHIELD_bar_pos[1], 4, 0.35f, br);

		// remaining shield / max shield
		SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::drawText(m_SHIELD_bar_pos[0] - 0.35f, m_SHIELD_bar_pos[1] + 0.1f, 0.28f, std::to_string(m_fighter->get_rem_SHIELD()) + std::string(" / ") + std::to_string(m_fighter->get_max_SHIELD()), br);

		// DRAW REMAINING SHIELD (blue part of shield bar)
		SETCOLOR(br.fill_color, 0.0f, 0.0f, 1.0f);
		graphics::drawRect(m_rem_SHIELD_bar_pos[0], m_rem_SHIELD_bar_pos[1], rem_SHIELD_bar_length(), 0.35f, br);
	}
}

FighterStatsBar::FighterStatsBar(Fighter* f)
	: m_fighter(f)
{}


VillainStatsBar::VillainStatsBar(Fighter* f)
	: FighterStatsBar(f)
{

	m_HP_bar_pos[0] = 4.0f;
	m_HP_bar_pos[1] = HP_BAR_POSITION_Y;

	m_rem_HP_bar_pos[0] = 0.0f;
	m_rem_HP_bar_pos[1] = HP_BAR_POSITION_Y;

	m_SHIELD_bar_pos[0] = 2.0f;
	m_SHIELD_bar_pos[1] = SHIELD_BAR_POSITION_Y;

	m_rem_SHIELD_bar_pos[0] = 0.0f;
	m_rem_SHIELD_bar_pos[1] = SHIELD_BAR_POSITION_Y;
}


HeroStatsBar::HeroStatsBar(Fighter* f)
	: FighterStatsBar(f)
{

	m_HP_bar_pos[0] = CANVAS_WIDTH - 4.0f;
	m_HP_bar_pos[1] = HP_BAR_POSITION_Y;

	m_rem_HP_bar_pos[0] = CANVAS_WIDTH;
	m_rem_HP_bar_pos[1] = HP_BAR_POSITION_Y;

	m_SHIELD_bar_pos[0] = CANVAS_WIDTH - 2.0f;
	m_SHIELD_bar_pos[1] = SHIELD_BAR_POSITION_Y;

	m_rem_SHIELD_bar_pos[0] = CANVAS_WIDTH;
	m_rem_SHIELD_bar_pos[1] = SHIELD_BAR_POSITION_Y;
}



float FighterStatsBar::rem_HP_bar_length()
{
	return (2 * 8.0f * m_fighter->get_rem_HP()) / m_fighter->get_max_HP();
}

float FighterStatsBar::rem_SHIELD_bar_length()
{
	return (2 * 4.0f * m_fighter->get_rem_SHIELD()) / m_fighter->get_max_SHIELD();
}



float VillainStatsBar::rem_HP_bar_length() {
	return FighterStatsBar::rem_HP_bar_length();
}

float VillainStatsBar::rem_SHIELD_bar_length() {
	return FighterStatsBar::rem_SHIELD_bar_length();
}

float HeroStatsBar::rem_HP_bar_length() {
	return -FighterStatsBar::rem_HP_bar_length();
}

float HeroStatsBar::rem_SHIELD_bar_length() {
	return -FighterStatsBar::rem_SHIELD_bar_length();
}
