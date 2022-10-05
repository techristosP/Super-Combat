#include "special_ability.h"


void SpecialAbility::draw()
{
	graphics::Brush br;

	// draw special ability icon
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	br.texture = ASSET_PATH + std::string("abilities\\") + m_abiliy_image;
	br.outline_opacity = 0.0f;

	// if the ability is not usable, then make slightly gray
	if (!m_usable) {
		SETCOLOR(br.fill_color, 0.58f, 0.58f, 0.58f);
	}

	graphics::drawRect(m_ability_pos[0], m_ability_pos[1], 1.1f, 1.1f, br);

	// if ability is hovered and is usable.
	if (m_usable && m_active)
	{

		// Enlarge special ability icon and make gray-ish for text to appear better.
		graphics::Brush br2;
		br2.texture = ASSET_PATH + std::string("abilities\\") + m_abiliy_image;
		SETCOLOR(br2.fill_color, 0.35f, 0.35f, 0.35f);
		br2.outline_opacity = 0.f;
		graphics::drawRect(m_ability_pos[0], m_ability_pos[1], 3.0f, 3.0f, br2);
		
		// Draw special ability's description.
		graphics::Brush br3;
		br3.texture = ASSET_PATH + std::string("abilities\\") + m_ability_desc;
		br3.outline_opacity = 0.f;
		graphics::drawRect(m_ability_pos[0], m_ability_pos[1], 3.0f, 3.0f, br3);
		

	}

	// If special ability is not usable, make gray-ish and draw cooldown number.
	if (!m_usable) {
		graphics::Brush br4;
		br4.texture = ASSET_PATH + std::string("cooldown_number\\cooldown_") + std::to_string(m_cooldown) + std::string(".png");
		br4.outline_opacity = 0.0f;
		graphics::drawRect(m_ability_pos[0], m_ability_pos[1], 1.1f, 1.1f, br4);
	}
}

bool SpecialAbility::contains(float mx, float my) const
{
	return distance(m_ability_pos[0], m_ability_pos[1], mx, my) < 0.5f;
}

void SpecialAbility::updateCooldown()
{	
	if (m_cooldown > 0) { decreaseCooldown(); }
	if (m_cooldown == 0) { setUsable(true); } // if cooldown is over, set card usable.
}

void SpecialAbility::setPosX(float pos_x)
{
	m_ability_pos[0] = pos_x; m_effect->setPosX(pos_x); // also sets position of effect that draws red circle around ability when active.
}

void SpecialAbility::setPosY(float pos_y)
{
	m_ability_pos[1] = pos_y; m_effect->setPosY(pos_y); // also sets position of effect that draws red circle around ability when active.
}


// CONSTRUCTORS - DESTRUCTORS - ACTIVATE

SpecialAbility::SpecialAbility(StatusEffect* ef, const std::string& ability_name, int cooldown_duration) 
:
	m_effect(ef),
	m_ability_name(ability_name),
	m_abiliy_image(ability_name + ".png"),
	m_ability_desc(ability_name + "_desc.png"),
	m_active(false),
	m_usable(true),
	m_cooldown_duration(cooldown_duration)
{}

SpecialAbility::~SpecialAbility()
{
	delete m_effect;
}


DelayedAbility::DelayedAbility(Fighter* self, StatusEffect* ef, const std::string& name, int cooldown_duration)
	: SpecialAbility(ef, name, cooldown_duration)
{}

// A delayed ability takes effect on the next turn of the same player that activated the ability.
void DelayedAbility::activate(Fighter* self)
{
	setUsable(false); resetCooldown();
	self->addStatusEffect(m_effect->getCopy());
}


ImmediateAbility::ImmediateAbility(Fighter* self, StatusEffect* ef, const std::string& name, int cooldown_duration)
	: SpecialAbility(ef, name, cooldown_duration)
{}

// An immediate ability takes effect at the following turn of the opponent, that is, it happens instantly.
void ImmediateAbility::activate(Fighter* self)
{
	setUsable(false); resetCooldown();
	self->getOpponent()->addStatusEffect(m_effect->getCopy());
}
