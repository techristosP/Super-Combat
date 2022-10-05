#include "game.h"


Game* Game::m_instance = nullptr;

void Game::draw()
{	
	graphics::Brush br;
	br.outline_opacity = 0.0f;

	// In TERMINATION STATE unnecessary condition checks are avoided.
	if (m_state == TERMINATION_STATE)
	{
		return;
	}

	// DRAW LOADING STATE & INIT STATE (Loading screen drawn twice, game music starts).
	if (m_state == LOADING_STATE || m_state == INIT_STATE)
	{
		br.texture = ASSET_PATH + std::string("loading_background.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		if (m_state == LOADING_STATE)
		{
			// Seed pseudo-random number generator.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// GAME MUSIC
			graphics::playMusic(ASSET_PATH + std::string("sounds\\mvc3.mp3"), 0.3f, true, 0);
			m_state = HERO_SELECTION_STATE;
		}
		
		return;
	}

	// DRAW SELECTION STATE
	if (m_state == HERO_SELECTION_STATE || m_state == VILLAIN_SELECTION_STATE)
	{
		// SELECTION BACKGROUND
		br.texture = ASSET_PATH + std::string("selection.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		draw_hero_selection_state();		// Draw hero icons and select.
		
		draw_villain_selection_state();		// Draw villain icons and select.

		// DRAW RED LINE SEPARATOR
		br.texture = "";
		SETCOLOR(br.fill_color, 1.0f, 0.0f, 0.0f); br.fill_opacity = 0.5f;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH - 8, 0.1f, br);

		return;
	}

	// DRAW ARENA BACKGROUND
	br.texture = ASSET_PATH + std::string("arena.png");
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	// DRAW GAMEOVER STATE
	if (m_state == GAMEOVER_STATE)
	{
		// DRAW GAMEOVER BACKGROUND
		br.texture = ASSET_PATH + std::string("selection.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		// DRAW GAMEOVER LOGO
		SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
		if (m_winner == m_hero)
			br.texture = ASSET_PATH + std::string("game_over_hero.png");
		else
			br.texture = ASSET_PATH + std::string("game_over_villain.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 6, 14.0f, 3.5f, br);

		// DRAW GAME WINNER'S IMAGE
		m_winner->draw();

		// DRAW GAME WINNER LOGO
		if (m_winner == m_hero)
			br.texture = ASSET_PATH + std::string("winner_hero.png");
		else
			br.texture = ASSET_PATH + std::string("winner_villain.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT * 4.5f / 6, 12.0f, 3.0f, br);

		// DRAW "REPLAY" AND "END GAME"
		graphics::drawText(CANVAS_WIDTH / 3, CANVAS_HEIGHT * 8.5f / 9, 0.4f, "Press  [R]  to replay or  [ENTER]  to exit game", br);

		return;
	}

	// DRAW VILLAIN AND HERO
	m_villain->draw();
	m_hero->draw();

	// DRAW TOGGLE SFX AND MUSIC ICONS
	GameAudio::getInstance()->draw();

}

// Draw hero icons and "CHOOSE HERO" message.
void Game::draw_hero_selection_state()
{
	graphics::Brush br, br2;

	br.outline_opacity = 0.0f;
	// "CHOOSE HERO" MESSAGE
	if (m_state == HERO_SELECTION_STATE) 
	{
		SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

		br.texture = ASSET_PATH + std::string("choose_hero.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 6, CANVAS_WIDTH / 2, 2.0f, br);
	}

	// DRAW HERO ICONS
	for (int i = 0; i < static_cast<int>(hero_t::NUMBER_OF_HEROES); i++) 
	{
		hero_t hero = static_cast<hero_t>(i);
		switch (hero)
		{
		case hero_t::DEADPOOL:
			br.texture = ASSET_PATH + std::string("icons\\deadpool_icon.png");
			break;

		case hero_t::WONDER_WOMAN:
			br.texture = ASSET_PATH + std::string("icons\\wonder_woman_icon.png");
			break;

		case hero_t::KRATOS:
			br.texture = ASSET_PATH + std::string("icons\\kratos_icon.png");
			break;

		case hero_t::SAITAMA:
			br.texture = ASSET_PATH + std::string("icons\\saitama_icon.png");
			break;

		}
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * i - 6), CANVAS_HEIGHT / 2 - 1.5f, ICON_SIZE, ICON_SIZE, br);

	}
	
	SETCOLOR(br2.outline_color, 0.0f, 0.0f, 1.0f);
	SETCOLOR(br2.fill_color, 0.0f, 0.0f, 1.0f);

	// Highlight icon when hovered (blue color)
	if (m_state == HERO_SELECTION_STATE && m_selected_hero != hero_t::NO_HERO) 
	{
		br2.fill_opacity = ICON_HIGHLIGHT_OPACITY;
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * static_cast<int>(m_selected_hero) - 6), CANVAS_HEIGHT / 2 - 1.5f, ICON_SIZE, ICON_SIZE, br2);
	}

	// When hero has been selected, icon is darkened.
	if (m_state != HERO_SELECTION_STATE && m_hero) 
	{
		br2.fill_opacity = ICON_HIGHLIGHT_OPACITY * 2;
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * static_cast<int>(m_selected_hero) - 6), CANVAS_HEIGHT / 2 - 1.5f, ICON_SIZE, ICON_SIZE, br2);
	}
	
}

// Draw villain icons and "CHOOSE VILLAIN" message.
void Game::draw_villain_selection_state() 
{
	graphics::Brush br, br2;

	br.outline_opacity = 0.0f;
	// "CHOOSE VILLAIN" MESSAGE
	if (m_state == VILLAIN_SELECTION_STATE) 
	{	
		SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

		br.texture = ASSET_PATH + std::string("choose_villain.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT * 5.0f / 6, CANVAS_WIDTH / 2, 2.0f, br);
	}

	// DRAW VILLAIN ICONS
	for (int i = 0; i < static_cast<int>(villain_t::NUMBER_OF_VILLAINS); i++) 
	{
		villain_t villain = static_cast<villain_t>(i);
		switch (villain)
		{
		case villain_t::JOKER:
			br.texture = ASSET_PATH + std::string("icons\\joker_icon.png");
			break;

		case villain_t::BOWSER:
			br.texture = ASSET_PATH + std::string("icons\\bowser_icon.png");
			break;

		case villain_t::DARTH_VADER:
			br.texture = ASSET_PATH + std::string("icons\\darth_vader_icon.png");
			break;

		case villain_t::FRIEZA:
			br.texture = ASSET_PATH + std::string("icons\\frieza_icon.png");
			break;

		}
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * i - 6), CANVAS_HEIGHT / 2 + 1.5f, ICON_SIZE, ICON_SIZE, br);

	}

	SETCOLOR(br2.outline_color, 1.0f, 0.0f, 0.0f);
	SETCOLOR(br2.fill_color, 1.0f, 0.0f, 0.0f);

	// Highlight icon when hovered (red color)
	if (m_state == VILLAIN_SELECTION_STATE && m_selected_villain != villain_t::NO_VILLAIN) 
	{
		br2.fill_opacity = ICON_HIGHLIGHT_OPACITY;
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * static_cast<int>(m_selected_villain) - 6), CANVAS_HEIGHT / 2 + 1.5f, ICON_SIZE, ICON_SIZE, br2);
	}

	// When villain has been selected, icon is darkened.
	if (m_state != VILLAIN_SELECTION_STATE && m_villain) 
	{
		br2.fill_opacity = ICON_HIGHLIGHT_OPACITY * 2;
		graphics::drawRect(CANVAS_WIDTH / 2 + (4 * static_cast<int>(m_selected_villain) - 6), CANVAS_HEIGHT / 2 + 1.5f, ICON_SIZE, ICON_SIZE, br2);
	}
}


void Game::update()
{
	
	if (m_state == LOADING_STATE)
	{
		return;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	// Get mouse position.
	float mx = graphics::windowToCanvasX(ms.cur_pos_x);
	float my = graphics::windowToCanvasY(ms.cur_pos_y);

	// Update game audio (mute or unmute sfx/music).
	GameAudio::getInstance()->update(mx, my, ms);

	if (m_state == HERO_SELECTION_STATE)
	{
		// Get enum type hero indicating hero selected.
		m_selected_hero = contains_hero(mx, my);

		if (ms.button_right_released && m_selected_hero != hero_t::NO_HERO)
		{
			event::addEvent(m_events, new SoundEvent("pop_drip", 0.8f));

			// CREATE SELECTED HERO AND SET ABILITIES
			switch (m_selected_hero)
			{
				case hero_t::DEADPOOL:
					m_hero = new Hero("deadpool", 380, 
						new DelayedAbility(m_hero, new Regeneration(3, 50), "regeneration", 4),
						new ImmediateAbility(m_hero, new KatanaSlash(4, 40, 10), "katana_slash", 4));
					break;
				case hero_t::WONDER_WOMAN:
					m_hero = new Hero("wonder_woman", 420, 
						new ImmediateAbility(m_hero, new BraceletsOfSubmission(1, 100), "bracelets_of_submission", 3),
						new ImmediateAbility(m_hero, new LassoOfTruth(1, 60), "lasso_of_truth", 3));
					break;
				case hero_t::KRATOS:
					m_hero = new Hero("kratos", 460, 
						new ImmediateAbility(m_hero, new BladesOfAthena(3, 15), "blades_of_athena", 4),
						new ImmediateAbility(m_hero, new NemesisRoar(1, 30), "nemesis_roar", 3));
					break;
				case hero_t::SAITAMA:
					m_hero = new Hero("saitama", 330,
						new DelayedAbility(m_hero, new Invincible(2), "invincible", 2),
						new ImmediateAbility(m_hero, new OnePunch(1), "one_punch", 4));
					break;
			}
			m_hero->fillHand();		// Fill hero's starting hand with cards.
			m_state = VILLAIN_SELECTION_STATE;
			
		}
		return;

	}

	if (m_state == VILLAIN_SELECTION_STATE)
	{
		// Get enum type villain indicating villain selected.
		m_selected_villain = contains_villain(mx, my);

		if (ms.button_right_released && m_selected_villain != villain_t::NO_VILLAIN)
		{
			event::addEvent(m_events, new SoundEvent("pop_drip", 0.8f));

			// CREATE SELECTED VILLAIN AND SET ABILITIES
			switch (m_selected_villain)
			{
			case villain_t::JOKER:
				m_villain = new Villain("joker", 300, 
					new ImmediateAbility(m_villain, new JokesOnYou(1), "jokes_on_you", 3),
					new ImmediateAbility(m_villain, new HappyGas(3, 140), "happy_gas", 4));
				break;
			case villain_t::BOWSER:
				m_villain = new Villain("bowser", 480, 
					new DelayedAbility(m_villain, new HealShell(4, 25), "heal_shell", 6),
					new ImmediateAbility(m_villain, new ShieldCrusher(1), "shield_crusher", 3));
				break;
			case villain_t::DARTH_VADER:
				m_villain = new Villain("darth_vader", 360, 
					new ImmediateAbility(m_villain, new ForceChoke(4, 30), "force_choke", 5),
					new ImmediateAbility(m_villain, new SithHealing(2, 25), "sith_healing", 3));
				break;
			case villain_t::FRIEZA:
				m_villain = new Villain("frieza", 430,
					new ImmediateAbility(m_villain, new EyeLaser(1, 40), "eye_laser", 2),
					new ImmediateAbility(m_villain, new DeathCannon(1, 80), "death_cannon", 4));
				break;
			}
			m_villain->fillHand();		// Fill vaillain's starting hand with cards.
			m_state = INIT_STATE;
			
		}
		return;
	}
	
	if (m_state == INIT_STATE)
	{
		init();
		m_state = VILLAIN_TURN;
		m_villain->toggleActive();

		return;
	}
	
	// Process events: update and delete inactive ones.
	event::processEvents(m_events);
	m_villain->processEvents();
	m_hero->processEvents();

	// Plays: VILLAIN
	if (m_state == VILLAIN_TURN)
	{
		bool turnComplete = m_villain->update(ms, mx, my);
		if (turnComplete)
		{
			m_hero->updateStatusEffects();	

			// CHECK GAME OVER
			if (!m_hero->getLifeState())		// If hero dies.
			{
				m_winner = m_villain;
				m_state = GAMEOVER_STATE;

				event::addEvent(m_events, new SoundEvent("Game Over"));

				return;
			}
			m_villain->toggleActive();			// Sets villain inactive.
			m_state = HERO_TURN;
			m_hero->toggleActive();				// Sets hero active.
			m_hero->fillHand();					// Fill hero's hand before his turn starts.
			m_hero->updateAbilityCooldowns();

			return;
		}
	}

	// Plays: HERO
	if (m_state == HERO_TURN)
	{
		bool turnComplete = m_hero->update(ms, mx, my);
		if (turnComplete)
		{
			m_villain->updateStatusEffects();

			// CHECK GAME OVER
			if (!m_villain->getLifeState())		// If villain dies.
			{
				m_winner = m_hero;
				m_state = GAMEOVER_STATE;

				event::addEvent(m_events, new SoundEvent("Game Over"));

				return;
			}
			m_hero->toggleActive();				// Sets hero inactive.
			m_state = VILLAIN_TURN;
			m_villain->toggleActive();			// Sets villain active.
			m_villain->fillHand();				// Fill villain's hand before his turn starts.
			m_villain->updateAbilityCooldowns();

			return;
		}
	}

	// GAME IS OVER
	if (m_state == GAMEOVER_STATE)
	{
		// [ENTER] key terminates the game.		// Memory deallocated in ~Game().
		bool terminate = graphics::getKeyState(graphics::SCANCODE_RETURN);
		if (terminate)
		{
			m_state = TERMINATION_STATE;
			
			return;
		}

		// [R] key starts the game again.
		bool replay = graphics::getKeyState(graphics::SCANCODE_R);
		if (replay)
		{
			m_state = LOADING_STATE;
			GameAudio::getInstance()->reset();	// Sets sfx and music active.
			deallocateMem();					// Only unnecessary memory deallocated.

			return;
		}
		return;
	}

	// GAME TERMINATED (Control returned to main in "combat.cpp" for final memory deallocation.)
	if (m_state == TERMINATION_STATE)
	{
		graphics::stopMessageLoop();

		return;
	}

}


Game::~Game()
{
	deallocateMem(); 
	CardPool::releaseInstance();
	GameAudio::releaseInstance();
}


// Checks if mouse is hovered over hero icon and returns specific enum hero type.
Game::hero_t Game::contains_hero(float mx, float my) const 
{
	for (int i = 0; i < static_cast<int>(hero_t::NUMBER_OF_HEROES); i++) 
	{
		float x = CANVAS_WIDTH / 2 + (4 * i - 6);
		float y = CANVAS_HEIGHT / 2 - 1.5f;

		if (mx >= x - ICON_SIZE / 2 && mx <= x + ICON_SIZE / 2) 
		{
			if (my >= y - ICON_SIZE / 2 && my <= y + ICON_SIZE / 2) 
			{
				return static_cast<hero_t>(i);
			}
		}	
	}

	return hero_t::NO_HERO;
}


// Checks if mouse is hovered over villain icon and returns specific enum villain type.
Game::villain_t Game::contains_villain(float mx, float my) const 
{
	for (int i = 0; i < static_cast<int>(villain_t::NUMBER_OF_VILLAINS); i++) 
	{
		float x = CANVAS_WIDTH / 2 + (4 * i - 6);
		float y = CANVAS_HEIGHT / 2 + 1.5f;

		if (mx >= x - ICON_SIZE / 2 && mx <= x + ICON_SIZE / 2) 
		{
			if (my >= y - ICON_SIZE / 2 && my <= y + ICON_SIZE / 2) 
			{
				return static_cast<villain_t>(i);
			}
		}
	}

	return villain_t::NO_VILLAIN;
}


// Preloads essential pngs, sets the font, plays "Fight!" sound.
void Game::init()
{
	graphics::preloadBitmaps(ASSET_PATH);
	graphics::preloadBitmaps(ASSET_PATH + std::string("effects\\"));
	graphics::preloadBitmaps(ASSET_PATH + std::string("cooldown_number\\"));

	graphics::setFont(ASSET_PATH + std::string("Xen Galaxy.ttf"));

	m_hero->setOpponent(m_villain);
	m_villain->setOpponent(m_hero);

	event::addEvent(m_events, new SoundEvent("Fight!"));
}


// Deletes hero, villain and possible remaining game events.
void Game::deallocateMem()
{
	delete m_hero;
	delete m_villain;
	m_hero = m_villain = m_winner = nullptr;

	event::clearEvents(m_events);
}


Game* Game::getInstance()
{
	if (!m_instance)
	{
		m_instance = new Game();
	}

	return m_instance;
}

void Game::releaseInstance()
{
	if (m_instance)
	{
		delete m_instance;
	}

	m_instance = nullptr;
}