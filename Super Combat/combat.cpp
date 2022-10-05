/*
* GAME CREATORS:	
* Christos Patrinopoulos		3200150 (p3200150@aueb.gr)
* Foivos-Nikolaos Papathanasiou 3200138 (p3200138@aueb.gr)
*/


#include <string>
#include "sgg/graphics.h"
#include "defines.h"
#include "game.h"


void draw()
{
	Game* game = Game::getInstance();
	game->draw();
}

void update(float ms)
{	
	Game* game = Game::getInstance();
	game->update();
}

int main(int argc, char** argv) {
	graphics::createWindow(1200, 800, "Super Combat");
	
	Game* game = Game::getInstance();

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
	
	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::preloadBitmaps(ASSET_PATH + std::string("icons\\"));

	graphics::startMessageLoop();

	graphics::destroyWindow();
	
	Game::releaseInstance();

	return 0;
}