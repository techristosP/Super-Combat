#include "status_effect.h"

typedef std::list<StatusEffect*>::iterator effectsIter_t;


// Draws red circle around special ability when the status effect is active.
void StatusEffect::draw()
{
	graphics::Brush br;
	SETCOLOR(br.outline_color, 1.0f, 0.0f, 0.0f);
	br.fill_opacity = 0;
	br.outline_opacity = 1;
	graphics::drawDisk(m_effect_pos[0], m_effect_pos[1], 0.55f, br);
}

StatusEffect::StatusEffect(int initial_cooldown) 
	: m_cooldown(initial_cooldown),
	m_initial_cooldown(initial_cooldown)
{}

StatusEffect::StatusEffect(const StatusEffect& proto)
	: m_cooldown(proto.m_cooldown), m_initial_cooldown(proto.m_initial_cooldown)
{
	m_effect_pos[0] = proto.m_effect_pos[0];
	m_effect_pos[1] = proto.m_effect_pos[1];
}



// DEADPOOL
Regeneration::Regeneration(int initial_cooldown, int regen_amt)
	: StatusEffect(initial_cooldown), m_regen_amt(regen_amt) 
{}

Regeneration::Regeneration(const Regeneration& proto)
	: StatusEffect(proto), m_regen_amt(proto.m_regen_amt)
{}

void Regeneration::activate(Fighter& f)
{
	f.increaseHP(m_regen_amt);
}


KatanaSlash::KatanaSlash(int initial_cooldown, int slash_dmg, int bleed_amt)
	: StatusEffect(initial_cooldown), m_slash_dmg(slash_dmg), m_bleed_amt(bleed_amt)
{}

KatanaSlash::KatanaSlash(const KatanaSlash & proto)
	: StatusEffect(proto), m_slash_dmg(proto.m_slash_dmg), m_bleed_amt(proto.m_bleed_amt)
{}

void KatanaSlash::activate(Fighter& f)
{
	if (m_cooldown == m_initial_cooldown)
	{
		f.decreaseHPandSHIELD(m_slash_dmg, 0);
	}

	f.decreaseHPandSHIELD(0, m_bleed_amt);
}



// WONDER WOMAN
BraceletsOfSubmission::BraceletsOfSubmission(int initial_cooldown, int gain_amt) 
	: StatusEffect(initial_cooldown), m_gain_amt(gain_amt)
{}

BraceletsOfSubmission::BraceletsOfSubmission(const BraceletsOfSubmission & proto)
	: StatusEffect(proto), m_gain_amt(proto.m_gain_amt)
{}

void BraceletsOfSubmission::activate(Fighter& f)
{
	int heal_amt = static_cast<int>(m_gain_amt * RAND0TO1());
	int shield_amt = m_gain_amt - heal_amt;

	f.getOpponent()->increaseHP(heal_amt);
	f.getOpponent()->increaseSHIELD(shield_amt);
}


LassoOfTruth::LassoOfTruth(int initial_cooldown, int dmg_amt) 
	: StatusEffect(initial_cooldown), m_dmg_amt(dmg_amt)
{}

LassoOfTruth::LassoOfTruth(const LassoOfTruth & proto)
	: StatusEffect(proto), m_dmg_amt(proto.m_dmg_amt)
{}

void LassoOfTruth::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(m_dmg_amt, 0);
	f.getOpponent()->increaseHP(m_dmg_amt);
}



// KRATOS
BladesOfAthena::BladesOfAthena(int initial_countdown, int dmg_amt)
	: StatusEffect(initial_countdown), m_dmg_amt(dmg_amt)
{}

BladesOfAthena::BladesOfAthena(const BladesOfAthena & proto)
	: StatusEffect(proto), m_dmg_amt(proto.m_dmg_amt)
{}

void BladesOfAthena::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(m_dmg_amt, 0);

	if (m_cooldown > 0)
		m_dmg_amt *= 2;

}


NemesisRoar::NemesisRoar(int initial_cooldown, int shout_dmg_amt)
	: StatusEffect(initial_cooldown), m_shout_dmg_amt(shout_dmg_amt)
{}

NemesisRoar::NemesisRoar(const NemesisRoar & proto)
	: StatusEffect(proto), m_shout_dmg_amt(proto.m_shout_dmg_amt)
{}

void NemesisRoar::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(0, m_shout_dmg_amt);
	f.getOpponent()->increaseSHIELD(f.get_rem_SHIELD());
	// steal opponent's active shield
}



// SAITAMA
OnePunch::OnePunch(int initial_cooldown)
	: StatusEffect(initial_cooldown)
{}

OnePunch::OnePunch(const OnePunch & proto)
	: StatusEffect(proto)
{}

void OnePunch::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(0, f.get_rem_HP() / 2);
}


Invincible::Invincible(int initial_cooldown)
	: StatusEffect(initial_cooldown)
{}

Invincible::Invincible(const Invincible & proto)
	: StatusEffect(proto)
{}


void Invincible::activate(Fighter& f)
{
	f.increaseSHIELD(MAX_SHIELD);
}


// JOKER
JokesOnYou::JokesOnYou(int initial_cooldown)
	: StatusEffect(initial_cooldown)
{}

JokesOnYou::JokesOnYou(const JokesOnYou & proto)
	: StatusEffect(proto)
{}

void JokesOnYou::activate(Fighter& f)
{
	int drainAmount = static_cast<int>(RAND0TO1() * 20 + 80);
	f.decreaseHPandSHIELD(0, drainAmount);
	f.increaseSHIELD(drainAmount);
}


HappyGas::HappyGas(int initial_cooldown, int poison_amt) 
	: StatusEffect(initial_cooldown), m_poison_amt(poison_amt)
{}

HappyGas::HappyGas(const HappyGas & proto)
	: StatusEffect(proto), m_poison_amt(proto.m_poison_amt)
{}

void HappyGas::activate(Fighter& f)
{
	if (m_cooldown == 1) {
		f.decreaseHPandSHIELD(m_poison_amt, 0);
		return;
	}

	int poison_dmg = static_cast<int>(m_poison_amt * RAND0TO1());
	f.decreaseHPandSHIELD(poison_dmg, 0);
	m_poison_amt -= poison_dmg;
}



// BOWSER
HealShell::HealShell(int initial_cooldown, int shield_amt) 
	: StatusEffect(initial_cooldown), m_shield_amt(shield_amt)
{}

HealShell::HealShell(const HealShell & proto)
	: StatusEffect(proto), m_shield_amt(proto.m_shield_amt)
{}

void HealShell::activate(Fighter& f)
{
	f.increaseSHIELD(m_shield_amt);
}


ShieldCrusher::ShieldCrusher(int initial_cooldown) 
	: StatusEffect(initial_cooldown)
{}

ShieldCrusher::ShieldCrusher(const ShieldCrusher & proto)
	: StatusEffect(proto)
{}

void ShieldCrusher::activate(Fighter& f)
{
	// remove all shield
	f.decreaseHPandSHIELD(f.get_rem_SHIELD(), 0);
}



//DARTH VADER
ForceChoke::ForceChoke(int initial_cooldown, int force_amt) 
	: StatusEffect(initial_cooldown), m_force_amt(force_amt)
{}

ForceChoke::ForceChoke(const ForceChoke & proto)
	: StatusEffect(proto), m_force_amt(proto.m_force_amt)
{}

void ForceChoke::activate(Fighter& f)
{
	if (m_cooldown % 2 == 0)
		f.decreaseHPandSHIELD(0, m_force_amt);
	else
		f.decreaseHPandSHIELD(m_force_amt, 0);
}


SithHealing::SithHealing(int initial_cooldown, int drain_dmg)
	: StatusEffect(initial_cooldown), m_drain_dmg(drain_dmg)
{}

SithHealing::SithHealing(const SithHealing& proto)
	: StatusEffect(proto), m_drain_dmg(proto.m_drain_dmg)
{}

void SithHealing::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(0, m_drain_dmg);
	f.getOpponent()->increaseHP(2 * m_drain_dmg);
}



// FRIEZA
DeathCannon::DeathCannon(int initial_cooldown, int boom_dmg)
	: StatusEffect(initial_cooldown), m_boom_dmg(boom_dmg)
{}

DeathCannon::DeathCannon(const DeathCannon & proto)
	: StatusEffect(proto), m_boom_dmg(proto.m_boom_dmg)
{}

void DeathCannon::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(0, m_boom_dmg);
}


EyeLaser::EyeLaser(int initial_cooldown, int laser_dmg)
	: StatusEffect(initial_cooldown), m_laser_dmg(laser_dmg)
{}

EyeLaser::EyeLaser(const EyeLaser & proto)
	: StatusEffect(proto), m_laser_dmg(proto.m_laser_dmg)
{}

void EyeLaser::activate(Fighter& f)
{
	f.decreaseHPandSHIELD(m_laser_dmg / 2, m_laser_dmg / 2);
}


void effect::updateStatusEffects(std::list<StatusEffect*>& effectsList, Fighter* fighter)
{
	effectsIter_t i = effectsList.begin();
	while (i != effectsList.end())
	{
		(*i)->activate(*fighter);

		(*i)->decreaseCooldown();

		if (!(*i)->conditionActive())
		{
			delete* i;
			i = effectsList.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void effect::addStatusEffect(std::list<StatusEffect*>& effectsList, StatusEffect* status_effect)
{
	effectsList.push_front(status_effect);
}

void effect::clearStatusEffects(std::list<StatusEffect*>& effectsList)
{
	for (effectsIter_t it = effectsList.begin(); it != effectsList.end(); )
	{
		delete* it;
		it = effectsList.erase(it);
	}

}

