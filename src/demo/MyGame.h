#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};
struct LangKey {
	SDL_Rect pos;
	std::string LanguageKey;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:
		Rect Player1,Player2;

		Vector2i Player2_velocity, Player1_velocity;

		std::vector<std::shared_ptr<GameKey>> gameKeys;

		std::vector<std::shared_ptr<LangKey>> LanguageKeys;

		/* GAMEPLAY */
		int score, numKeys, lives;
		bool gameWon;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();

		void OnLanguageChanged();
	public:
        MyGame();
		~MyGame();
};

#endif