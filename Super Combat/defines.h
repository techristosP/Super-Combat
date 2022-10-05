#pragma once
#include <array>
#include <random>


#define CANVAS_WIDTH 28.0f
#define CANVAS_HEIGHT 16.0f

#define ICON_SIZE 2.0f
#define ICON_HIGHLIGHT_OPACITY 0.2f

#define CARDPOOL_SIZE 10

#define CARD_WIDTH 2.6f
#define CARD_HEIGHT 2.6f

#define CARD_HEIGHT_CENTER		CANVAS_HEIGHT * 8/9

#define CARD_BOX_HEIGHT_CENTER		(CARD_HEIGHT_CENTER - (CARD_HEIGHT + 0.4f))

#define CARD_BOX_STAT_1_Y		(CARD_BOX_HEIGHT_CENTER - CARD_HEIGHT / 2) + 5.0f / 8 * CARD_HEIGHT
#define CARD_BOX_STAT_2_Y		(CARD_BOX_HEIGHT_CENTER - CARD_HEIGHT / 2) + 7.0f / 8 * CARD_HEIGHT
#define CARD_BOX_STATS_FONT_SIZE		0.35f

#define SPECIAL_ABILITY_1_H		2.5f
#define SPECIAL_ABILITY_2_H		4.5f


#define PLAYER_SIZE 7.0f

#define MAX_SHIELD 100

#define HP_BAR_POSITION_Y 0.5f
#define SHIELD_BAR_POSITION_Y 1.0f

#define HAND_SIZE 3

#define ASSET_PATH ".\\assets\\"
//#define ASSET_PATH ".\\bin\\assets\\"

#define SETCOLOR(c, r, g, b) { c[0] = r; c[1] = g; c[2] = b; }

#define RAND0TO1() (rand() / (float)RAND_MAX)

inline float distance(float x1, float y1, float x2, float y2) 
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrtf(dx * dx + dy * dy);
}

typedef std::array<int, 4> stats_array_t; // for card stats array.

inline float interpolation(float a, float b, float x)
{
	return a * (1 - x) + b * x;
}