#pragma once
#include "defines.h"
#include "sgg\graphics.h"

class GameAudio
{
private:
	static GameAudio* m_instance;

	GameAudio();
	~GameAudio() {};

	bool m_sfx_active;
	bool m_music_active;

	float m_sfx_pos[2];
	float m_music_pos[2];

	bool contains_music(float mx, float my) const;
	bool contains_sfx(float mx, float my) const;

	inline void toggleMusic() { m_music_active = !m_music_active; }
	inline void toggleSFX() { m_sfx_active = !m_sfx_active; }

public:
	void draw();
	void update(float mx, float my, graphics::MouseState ms);

	inline bool musicIsActive() const { return m_music_active; }
	inline bool sfxIsActive() const { return m_sfx_active; }

	inline void reset() { m_music_active = true; m_sfx_active = true; }

	static GameAudio* getInstance();
	static void releaseInstance();
};

