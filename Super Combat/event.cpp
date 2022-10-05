#include "event.h"
#include "sgg/graphics.h"
#include "fighter.h"

typedef std::list<Event*>::iterator eventsIter_t; // for event list iterator.


void ShakeEvent::shake()
{

	float step = 0.05f;
	static float sh = 0.0f;

	if (m_active)
	{
		// "shakes" damaged fighter.
		if (fighter->getPosX() >= m_init_pos + 0.25)
		{
			step *= -1;
		}
		if (fighter->getPosX() <= m_init_pos - 0.25)
		{
			step *= -1;
		}
		sh += step;

		fighter->setPosX(fighter->getPosX() + sh);
	}
	
	// Return fighter to initial position.
	if (!m_active)
	{
		fighter->setPosX(m_init_pos);
		sh = 0;
	}

}

void ShakeEvent::update()
{
	Event::update();
	shake();
}

void Event::update()
{
	if (!m_active)
		return;

	if (waiting())
	{
		m_elapsed_delay += graphics::getDeltaTime() / 1000.0f;
		return;
	}
	
	m_elapsed_time += graphics::getDeltaTime() / 1000.0f;
	if (m_elapsed_time > m_duration)
	{
		m_active = false;
	}
}


bool Event::waiting() const
{
	return m_elapsed_delay < m_delay;
}


void DamageEvent::draw()
{
	graphics::Brush br, br2;

	br.texture = ASSET_PATH + std::string("effects\\slash.png");
	br.outline_opacity = 0.0f;

	graphics::setOrientation(m_orientation);

	graphics::drawRect(m_pos_x, m_pos_y, PLAYER_SIZE, PLAYER_SIZE, br);

	graphics::resetPose();

	br2.texture = ASSET_PATH + std::string("effects\\") + m_comic_sound_pic;
	br2.outline_opacity = 0.0f;

	graphics::drawRect(m_pos_x + 2.0f, m_pos_y - 2.0f, PLAYER_SIZE/2, PLAYER_SIZE/2, br2);

}

void HealEvent::draw()
{
	graphics::Brush br;

	float s = m_elapsed_time / m_duration;	// scale increase by elapsed time.

	br.texture = ASSET_PATH + std::string("effects\\heal.png");
	br.outline_opacity = 0.0f;
	
	graphics::setScale(m_scale + s, m_scale + s); // set scale.

	graphics::drawRect(m_pos_x, m_pos_y, PLAYER_SIZE / 6.0f, PLAYER_SIZE / 6.0f, br);

	graphics::resetPose();
}

void ShieldEvent::draw()
{
	graphics::Brush br;

	float s = m_elapsed_time / m_duration; // scale increase by elapsed time.

	br.texture = ASSET_PATH + std::string("effects\\shield.png");
	br.outline_opacity = 0.0f;
	
	graphics::setScale(m_scale + s, m_scale + s); // set scale.

	graphics::drawRect(m_pos_x, m_pos_y, PLAYER_SIZE / 6.0f, PLAYER_SIZE / 6.0f, br);

	graphics::resetPose();
}

// Draws damage/shield/heal amount indication close to fighter.
void Indicator::draw()
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, m_red, m_green, m_blue);

	graphics::drawText(m_posX, m_posY, 0.6f, std::to_string(m_amount), br);
}




// CONSTRUCTORS
Event::Event(float dur, float del)
	: m_duration(dur), m_delay(del)
{}


SoundEvent::SoundEvent(const std::string& soundFile, float vol)
	: Event(0.5f, 0.2f), m_sound(ASSET_PATH + std::string("sounds\\") + soundFile + ".wav")
{
	m_active = true;

	if (!GameAudio::getInstance()->sfxIsActive()) { return; } // if sfx inactive, do not play sound.
	graphics::playSound(m_sound, vol, false);
}


ShakeEvent::ShakeEvent(Fighter* f) :
	Event(0.3f, 0.2f),
	fighter(f),
	m_init_pos(f->getInitialPosX())
{
	m_active = true;
}


CardEvent::CardEvent(float posX, float posY)
	: Event(1.8f, 0.0f), m_pos_x(posX), m_pos_y(posY)
{
	m_active = true;
}


DamageEvent::DamageEvent(float posX, float posY)
	: CardEvent(posX, posY)
{
	m_orientation = RAND0TO1() * 180.0f - 90.0f;
	m_comic_sound_pic = "comic_sound_" + std::to_string(static_cast<int>(interpolation(1, 5, RAND0TO1()))) + std::string(".png");
}

ShieldEvent::ShieldEvent(float posX, float posY)
	: CardEvent(posX, posY)
{
	m_scale = 0.8f + RAND0TO1() * 0.4f;
}

HealEvent::HealEvent(float posX, float posY)
	: CardEvent(posX, posY)
{
	m_scale = 0.8f + RAND0TO1() * 0.4f;
}



Indicator::Indicator(int amount, float posX, float posY, float red, float green, float blue)
	: Event(1.0f, 0.1f), m_amount(amount), m_posX(posX), m_posY(posY), m_red(red), m_green(green), m_blue(blue)
{
	m_active = true;
	m_posX = m_posX + ( RAND0TO1() * 2.0f) * (1 - 2 * (static_cast<int>(RAND0TO1() + 0.5f)) ) ;
	m_posY = m_posY + ( RAND0TO1() * 1.0f) * (1 - 2 * (static_cast<int>(RAND0TO1() + 0.5f)));
}

// PhysDamage: red color
PhysDamageIndicator::PhysDamageIndicator(int amount, float posX, float posY)
	: Indicator(amount, posX, posY, 1.0f, 0.0f, 0.0f)
{}

// MagicDamage: purple color
MagicDamageIndicator::MagicDamageIndicator(int amount, float posX, float posY)
	: Indicator(amount, posX, posY, 1.0f, 0.0f, 1.0f)
{}

// Shield: blue color
ShieldIndicator::ShieldIndicator(int amount, float posX, float posY)
	: Indicator(amount, posX, posY, 0.0f, 1.0f, 1.0f)
{}

// Heal: green color
HealIndicator::HealIndicator(int amount, float posX, float posY)
	: Indicator(amount, posX, posY, 0.0f, 0.8f, 0.0f)
{}


// Updates events and clears inactive onces.
void event::processEvents(std::list<Event*>& evtlist)
{
	for (auto e : evtlist)
	{
		e->update();
	}


	for (eventsIter_t it = evtlist.begin(); it != evtlist.end();)
	{

		if (!(*it)->active())
		{
			delete* it;
			it = evtlist.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void event::addEvent(std::list<Event*>& evtlist, Event* evt)
{
	evtlist.push_front(evt);
}

// Clears remaining events after round completion.
void event::clearEvents(std::list<Event*>& evtlist)
{
	for (eventsIter_t it = evtlist.begin(); it != evtlist.end();)
	{
		delete* it;
		it = evtlist.erase(it);
	}
}


