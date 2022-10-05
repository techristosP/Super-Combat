#pragma once
#include <list>
#include <string>
#include "defines.h"
#include "sgg/graphics.h"
#include "game_audio.h"


class Event
{
protected:
	const float m_duration = 0.0f;
	const float m_delay = 0.0f;
	float m_elapsed_time = 0.0f;
	float m_elapsed_delay = 0.0f;

	bool m_active = false;

public:
	virtual void draw() = 0;
	virtual void update();

	Event(float dur = 2.0f, float del = 0.0f);
	virtual ~Event() {};

	inline bool active() const { return m_active; }

	bool waiting() const ;
	
};

class SoundEvent : public Event
{
	const std::string m_sound;

public:
	SoundEvent(const std::string& soundFile, float vol = 0.4f);
	virtual ~SoundEvent() {};

	virtual void draw() override {};
	virtual void update() {};
};

class ShakeEvent : public Event
{
	class Fighter* fighter;
	const float m_init_pos;
	void shake();

public:
	ShakeEvent(Fighter* f);

	virtual ~ShakeEvent() {};

	virtual void draw() override {};
	virtual void update() override;
};

class CardEvent : public Event
{
protected:
	float m_pos_x;
	float m_pos_y;

public:
	CardEvent(float posX, float posY);
	virtual ~CardEvent() {};

	virtual void draw() = 0;
};

class DamageEvent : public CardEvent
{
	float m_orientation;
	std::string m_comic_sound_pic;

public:
	DamageEvent(float posX, float posY);
	virtual ~DamageEvent() {};

	virtual void draw() override;
	
};


class ShieldEvent : public CardEvent
{
public:
	float m_scale;

	ShieldEvent(float posX, float posY);
	virtual ~ShieldEvent() {};

	virtual void draw() override;
};


class HealEvent : public CardEvent
{

public:
	float m_scale;

	HealEvent(float posX, float posY);
	virtual ~HealEvent() {};

	virtual void draw() override;
};


class Indicator : public Event
{
	const int m_amount;
	float m_posX, m_posY;

	const float m_red, m_green, m_blue;

public:
	Indicator(int amount, float posX, float posY, float red, float green, float blue);
	virtual ~Indicator() {};

	// Draws damage/shield/heal amount indication close to fighter.
	void draw() override;
};

class PhysDamageIndicator : public Indicator
{
public:
	PhysDamageIndicator(int amount, float posX, float posY);
	virtual ~PhysDamageIndicator() {};
};

class MagicDamageIndicator : public Indicator
{
public:
	MagicDamageIndicator(int amount, float posX, float posY);
	virtual ~MagicDamageIndicator() {};
};

class ShieldIndicator : public Indicator
{
public:
	ShieldIndicator(int amount, float posX, float posY);
	virtual ~ShieldIndicator() {};
};

class HealIndicator : public Indicator
{
public:
	HealIndicator(int amount, float posX, float posY);
	virtual ~HealIndicator() {};
};


namespace event
{
	// Updates events and clears inactive onces.
	void processEvents(std::list<Event*>& evtlist);

	void addEvent(std::list<Event*>& evtlist, Event* evt);

	// Clears remaining events after round completion.
	void clearEvents(std::list<Event*>& evtlist);
}