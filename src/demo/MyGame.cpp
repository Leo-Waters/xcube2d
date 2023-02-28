#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), Player1(5, 5, 30, 30), Player2(5,5,30,30) {
	
	gfx->setVerticalSync(true);

	//load textures--------------------------------------------------------------
	ResourceManager::loadTexture("./res/textures/germany.png", SDL_Color());
	ResourceManager::loadTexture("./res/textures/england.png", SDL_Color());

	//create flags----------------------------------------------------------------
	for (int i = 0; i < numKeys; i++) {
		std::shared_ptr<LangKey> k = std::make_shared<LangKey>();
		k->isAlive = true;
		k->pos = new SDL_Rect{ getRandom(0, 750), getRandom(0, 550), 50, 30 };
		LanguageKeys.push_back(k);
	}



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
			LangIndex = selection;
			mySystem->SetLanguage(languages.at(selection).c_str(), std::bind(&MyGame::OnLanguageChanged, this));
			SelectingLang = false;
		}
		else
		{
			std::cout << "Language " << selection << " is not a valid option\n";
		}
	}


}

void MyGame::NextLanguage()
{
	LangIndex++;
	std::vector<std::string> languages = mySystem->GetAvalibleLanguages();
	if (LangIndex == languages.size()) {
		LangIndex = 0;
	}
	mySystem->SetLanguage(languages.at(LangIndex).c_str(), std::bind(&MyGame::OnLanguageChanged, this));
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


	for (auto key : LanguageKeys) {
		Point2 pos(key->pos->x+ (key->pos->w/2), key->pos->y + (key->pos->h / 2));
		if (key->isAlive && (Player1.contains(pos) || Player2.contains(pos))) {
			score += 200;
			key->isAlive = false;
			numKeys--;
			NextLanguage();
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

	for (auto key : LanguageKeys) {
		if (key->isAlive) {
			gfx->drawTexture(mySystem->GetTexture("flag"), key->pos, SDL_FLIP_NONE);
		}
	}
}

void MyGame::renderUI() {
	
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->useFont(mySystem->GetFont("main", 30));
	gfx->drawText(mySystem->GetText("name"), 0, 25);

	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);
	if (gameWon) {
		gfx->useFont(mySystem->GetFont("bold", 72));
		gfx->drawText(mySystem->GetText("win"), 0, 500);
	}
}



void MyGame::OnLanguageChanged()
{
	gfx->useFont(mySystem->GetFont("main", 72));

	for (int i = 0; i < LanguageKeys.size(); i++) {
		LanguageKeys[i]->Texture = mySystem->GetTexture("flag");
	}
}
