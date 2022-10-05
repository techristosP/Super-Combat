#pragma once
#include <ctime>
#include "sgg\graphics.h"
#include "defines.h"
#include "fighter.h"
#include "card.h"
#include "game_audio.h"


class Game
{
private:
	
	static Game* m_instance;

	Game() {}
	~Game();

	enum game_state_t
	{
		LOADING_STATE,
		HERO_SELECTION_STATE,
		VILLAIN_SELECTION_STATE,
		INIT_STATE,
		VILLAIN_TURN,
		HERO_TURN,
		GAMEOVER_STATE,
		TERMINATION_STATE
	};

	game_state_t m_state = LOADING_STATE;

	enum class villain_t
	{
		JOKER,
		BOWSER,
		DARTH_VADER,
		FRIEZA,

		NUMBER_OF_VILLAINS,
		NO_VILLAIN
	};

	villain_t m_selected_villain = villain_t::NO_VILLAIN;

	enum class hero_t
	{
		DEADPOOL,
		WONDER_WOMAN,
		KRATOS,
		SAITAMA, 

		NUMBER_OF_HEROES,
		NO_HERO
	};

	hero_t m_selected_hero = hero_t::NO_HERO;

	Fighter* m_hero;
	Fighter* m_villain;
	Fighter* m_winner;

	std::list<Event*> m_events;

	// Draw hero icons and "CHOOSE HERO" message.
	void draw_hero_selection_state();
	// Draw villain icons and "CHOOSE VILLAIN" message.
	void draw_villain_selection_state();

	// Checks if mouse is hovered over hero icon and returns specific enum hero type.
	hero_t contains_hero(float mx, float my) const;
	// Checks if mouse is hovered over villain icon and returns specific enum villain type.
	villain_t contains_villain(float mx, float my) const;

	// Preloads essential pngs, sets the font, plays "Fight!" sound.
	void init();

	// Deletes hero, villain and possible remaining game events.
	void deallocateMem();

public:
	void draw();
	void update();

	static Game* getInstance();
	static void releaseInstance();



};