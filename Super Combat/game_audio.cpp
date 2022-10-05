#include "game_audio.h"

GameAudio* GameAudio::m_instance = nullptr;

GameAudio* GameAudio::getInstance()
{
	if (!m_instance) {
		m_instance = new GameAudio();
	}

	return m_instance;
}

void GameAudio::releaseInstance()
{
	if (m_instance) {
		delete m_instance;
	}

	m_instance = nullptr;
}

GameAudio::GameAudio() :
	m_sfx_active(true),
	m_music_active(true)
{
	m_sfx_pos[0] = CANVAS_WIDTH / 2 - 1.0f;
	m_sfx_pos[1] = 0.6f;

	m_music_pos[0] = CANVAS_WIDTH / 2 + 1.0f;
	m_music_pos[1] = 0.6f;
}

void GameAudio::draw()
{	
	graphics::Brush br1;
	graphics::Brush br2;

	br1.outline_opacity = 0.0f, br2.outline_opacity = 0.0f;

	if (m_music_active)
	{
		br1.texture = ASSET_PATH + std::string("music_volume.png");
	}
	else {
		br1.texture = ASSET_PATH + std::string("music_mute.png");
	}

	if (m_sfx_active) {
		br2.texture = ASSET_PATH + std::string("sfx_volume.png");
	}
	else {
		br2.texture = ASSET_PATH + std::string("sfx_mute.png");
	}

	graphics::drawRect(m_music_pos[0], m_music_pos[1], 1.0f, 1.0f, br1); // draw music toggle icon
	graphics::drawRect(m_sfx_pos[0], m_sfx_pos[1], 1.5f, 1.5f, br2); // draw sfx toggle icon
}

void GameAudio::update(float mx, float my, graphics::MouseState ms)
{
	if (contains_music(mx, my) && ms.button_right_released) 
	{ 
		toggleMusic(); 
		if (musicIsActive())
		{
			graphics::playMusic(ASSET_PATH + std::string("sounds\\mvc3.mp3"), 0.3f, true);
		}
		else {
			graphics::stopMusic();
		}
	}

	if (contains_sfx(mx, my) && ms.button_right_released){toggleSFX();}

}

bool GameAudio::contains_music(float mx, float my) const
{	
    return distance(m_music_pos[0], m_music_pos[1], mx, my) < 0.5f;
}

bool GameAudio::contains_sfx(float mx, float my) const
{
	return distance(m_sfx_pos[0], m_sfx_pos[1], mx, my) < 0.5f;
}
