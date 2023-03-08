#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct ball {
	std::string name;
	SDL_Rect* pos;
	std::string LanguageKey;
	SDL_Texture* Texture;
};

class MyGame : public AbstractGame {
	private:
		std::string ballnames[5]={"blue","green","purple","red","yellow"};
		Rect Player1;

		SDL_Texture* flag;
		SDL_Texture* background;
		SDL_Rect flagPos = { 10, 25, 50, 30 };
		SDL_Rect backgroundpos = { 0, 0, 0, 0 };
		Vector2i Player1_velocity;

		std::vector<std::shared_ptr<ball>> balls;

		/* GAMEPLAY */
		int score,LangIndex,correctBalls=0,IncorrectBalls=0;
		std::string CurrentAnswer= ballnames[0];
		std::string SelectedLanguage;

		bool gameWon;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();

		void NextLanguage();

		void OnLanguageChanged();
	public:
        MyGame();
		~MyGame();
};

#endif