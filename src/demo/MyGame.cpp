#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), Player1(5, 5, 30, 30), Player2(5,5,30,30) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	//test language select menu
	std::cout << "-----Test Language Select\n";
	std::vector<std::string> languages = mySystem->GetAvalibleLanguages();

	for (size_t i = 0; i < languages.size(); i++)
	{
		std::cout << "(" << i << ")" << languages.at(i) << std::endl;
	}bool SelectingLang = true;
	while (SelectingLang)
	{
		int selection;
		std::cin >> selection;

		if (selection < languages.size()) {
			mySystem->SetLanguage(languages.at(selection).c_str());
			SelectingLang = false;
		}
		else
		{
			std::cout << "Language " << selection << " is not a valid option\n";
		}
	}


	
    for (int i = 0; i < numKeys; i++) {
        std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
        k->isAlive = true;
        k->pos = Point2(getRandom(0, 750), getRandom(0, 550));
        gameKeys.push_back(k);
    }
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	int speed = 3;

	if (eventSystem->isPressed(Key::W)) {
		Player1_velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::S)) {
		Player1_velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::A)) {
		Player1_velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::D)) {
		Player1_velocity.x = speed;
	}

	if (eventSystem->isPressed(Key::UP)) {
		Player2_velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::DOWN)) {
		Player2_velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		Player2_velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::RIGHT)) {
		Player2_velocity.x = speed;
	}
}

void MyGame::update() {
	Player1.x += Player1_velocity.x;
	Player1.y += Player1_velocity.y;
	Player2.x += Player2_velocity.x;
	Player2.y += Player2_velocity.y;

	for (auto key : gameKeys) {
		if (key->isAlive && (Player1.contains(key->pos)|| Player2.contains(key->pos))) {
			score += 200;
			key->isAlive = false;
			numKeys--;
		}
	}

	Player1_velocity.x = 0;
	Player1_velocity.y = 0;
	Player2_velocity.x = 0;
	Player2_velocity.y = 0;

	if (numKeys == 0) {
		gameWon = true;
	}
}

void MyGame::render() {
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(Player1);

	gfx->setDrawColor(SDL_COLOR_BLUE);
	gfx->drawRect(Player2);

	gfx->setDrawColor(SDL_COLOR_YELLOW);
	for (auto key : gameKeys)
        if (key->isAlive)
		    gfx->drawCircle(key->pos, 5);
}

void MyGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	if (gameWon) {
		gfx->drawText(mySystem->GetText("win"), 0, 500);
		gfx->drawText(mySystem->GetText("mutliLine"), 0, 100);
	}
}