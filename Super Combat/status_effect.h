#pragma once
#include <list>
#include "fighter.h"
#include "sgg/graphics.h"

class Fighter;

class StatusEffect
{
	float m_effect_pos[2]{};

protected:

	int m_cooldown;
	const int m_initial_cooldown;

public:

	inline void decreaseCooldown() { m_cooldown -= 1; }
	inline bool conditionActive() { return m_cooldown > 0; }
	
	StatusEffect(int initial_cooldown);
	StatusEffect(const StatusEffect& proto);
	virtual ~StatusEffect() {};

	virtual void activate(Fighter& self) = 0;

	// creates copy of dynamic status effect instance.
	virtual StatusEffect* getCopy() = 0;

	inline void setPosX(float posX) { m_effect_pos[0] = posX; }
	inline void setPosY(float posY) { m_effect_pos[1] = posY; }

	// Draws red circle around special ability when the status effect is active.
	void draw();
};


// DEADPOOL
class Regeneration : public StatusEffect
{
private:
	const int m_regen_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new Regeneration(*this); }

	Regeneration(int initial_cooldown, int regen_amt);
	Regeneration(const Regeneration& proto);
	virtual ~Regeneration() {};
};

class KatanaSlash : public StatusEffect
{
private:
	int m_slash_dmg;
	const int m_bleed_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new KatanaSlash(*this); }

	KatanaSlash(int initial_cooldown, int slash_dmg, int bleed_amt);
	KatanaSlash(const KatanaSlash& proto);
	virtual ~KatanaSlash() {};
};


// WONDER WOMAN
class BraceletsOfSubmission : public StatusEffect
{
private:
	int m_gain_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new BraceletsOfSubmission(*this); }

	BraceletsOfSubmission(int initial_cooldown, int gain_amt);
	BraceletsOfSubmission(const BraceletsOfSubmission& proto);
	virtual ~BraceletsOfSubmission() {};
};

class LassoOfTruth : public StatusEffect
{
private:
	const int m_dmg_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new LassoOfTruth(*this); }

	LassoOfTruth(int initial_cooldown, int dmg_amt);
	LassoOfTruth(const LassoOfTruth& proto);
	virtual ~LassoOfTruth() {};
};


// KRATOS
class BladesOfAthena : public StatusEffect
{
private:
	int m_dmg_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new BladesOfAthena(*this); }

	BladesOfAthena(int initial_cooldown, int m_dmg_amt);
	BladesOfAthena(const BladesOfAthena& proto);
	virtual ~BladesOfAthena() {};
};

class NemesisRoar : public StatusEffect
{
private:
	int m_shout_dmg_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new NemesisRoar(*this); }

	NemesisRoar(int initial_cooldown, int shout_dmg_amt);
	NemesisRoar(const NemesisRoar& proto);
	virtual ~NemesisRoar() {};
};


// SAITAMA
class OnePunch : public StatusEffect
{
public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new OnePunch(*this); }

	OnePunch(int initial_cooldown);
	OnePunch(const OnePunch& proto);
	virtual ~OnePunch() {};
};

class Invincible : public StatusEffect
{
public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new Invincible(*this); }

	Invincible(int initial_cooldown);
	Invincible(const Invincible& proto);
	virtual ~Invincible() {};
};


// JOKER
class JokesOnYou : public StatusEffect
{
public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new JokesOnYou(*this); }
	
	JokesOnYou(int initial_cooldown);
	JokesOnYou(const JokesOnYou& proto);
	virtual ~JokesOnYou() {};
};

class HappyGas : public StatusEffect
{
private:
	int m_poison_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new HappyGas(*this); }

	HappyGas(int initial_cooldown, int poison_amt);
	HappyGas(const HappyGas& proto);
	virtual ~HappyGas() {};
};


// BOWSER
class HealShell : public StatusEffect
{
private:
	const int m_shield_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new HealShell(*this); }

	HealShell(int initial_cooldown, int shield_amt);
	HealShell(const HealShell& proto);
	virtual ~HealShell() {};
};


class ShieldCrusher : public StatusEffect
{
public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new ShieldCrusher(*this); }

	ShieldCrusher(int initial_cooldown);
	ShieldCrusher(const ShieldCrusher& proto);
	virtual ~ShieldCrusher() {};
};


// DARTH VADER
class ForceChoke : public StatusEffect
{
private:
	const int m_force_amt;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new ForceChoke(*this); }

	ForceChoke(int initial_cooldown, int force_amt);
	ForceChoke(const ForceChoke& proto);
	virtual ~ForceChoke() {};
};


class SithHealing : public StatusEffect
{
private:
	const int m_drain_dmg;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new SithHealing(*this); }

	SithHealing(int initial_cooldown, int drain_dmg);
	SithHealing(const SithHealing& proto);
	virtual ~SithHealing() {};
};


//FRIEZA
class DeathCannon : public StatusEffect
{
private:
	const int m_boom_dmg;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new DeathCannon(*this); }

	DeathCannon(int initial_cooldown, int boom_dmg);
	DeathCannon(const DeathCannon& proto);
	virtual ~DeathCannon() {};
};


class EyeLaser : public StatusEffect
{
private:
	const int m_laser_dmg;

public:
	virtual void activate(Fighter& f) override;
	virtual StatusEffect* getCopy() override { return new EyeLaser(*this); }

	EyeLaser(int initial_cooldown, int laser_dmg);
	EyeLaser(const EyeLaser& proto);
	virtual ~EyeLaser() {};
};

namespace effect
{
	void updateStatusEffects(std::list<StatusEffect*>& effectsList, Fighter* fighter);

	void addStatusEffect(std::list<StatusEffect*>& effectsList, StatusEffect* status_effect);

	void clearStatusEffects(std::list<StatusEffect*>& effectsList);
}