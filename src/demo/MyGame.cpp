#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), gameWon(false), Player1(300, 20, 30, 30) {
	
	gfx->setVerticalSync(true);

	//load textures--------------------------------------------------------------
	ResourceManager::loadTexture("./res/textures/germany.png", SDL_Color());
	ResourceManager::loadTexture("./res/textures/england.png", SDL_Color()); 
	ResourceManager::loadTexture("./res/textures/france.png", SDL_Color());
	
	background = ResourceManager::loadTexture("./res/textures/background.png", SDL_Color());
	

	for (size_t i = 0; i < ballnames->length(); i++)
	{
		//create balls----------------------------------------------------------------
		std::shared_ptr<ball> k = std::make_shared<ball>();
		k->pos = new SDL_Rect{ getRandom(0, 750), getRandom(100, 550), 30, 30 };
		k->Texture = ResourceManager::loadTexture("./res/textures/ball_" + ballnames[i] + ".png", SDL_Color());
		k->name = ballnames[i];
		balls.push_back(k);
	}

	



	//test language select menu
	std::cout << "-----Test set starting Language \n";
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
			SelectedLanguage = languages.at(selection);
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
	CurrentAnswer = ballnames[getRandom(0, ballnames->length())];
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
	int speed = 5;

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

}

void MyGame::update() {
	Player1.x += Player1_velocity.x;
	Player1.y += Player1_velocity.y;


	for (auto key : balls) {
		Point2 pos(key->pos->x+ (key->pos->w/2), key->pos->y + (key->pos->h / 2));
		if (Player1.contains(pos)) {

			if (key->name == CurrentAnswer) {
				score += 200;
				correctBalls++;
			}
			else
			{
				IncorrectBalls++;
			}

			key->pos = new SDL_Rect{ getRandom(0, 750), getRandom(100, 550), 30, 30 };
			NextLanguage();
		}
	}

	Player1_velocity.x = 0;
	Player1_velocity.y = 0;
	if (correctBalls+IncorrectBalls == 10&&!gameWon) {
		mySystem->SetLanguage(SelectedLanguage.c_str(), std::bind(&MyGame::OnLanguageChanged, this));
		gameWon = true;
	}

}

void MyGame::render() {

	backgroundpos.w = gfx->getCurrentWindowSize().w;
	backgroundpos.h = gfx->getCurrentWindowSize().h;

	gfx->drawTexture(background, &backgroundpos, SDL_FLIP_NONE);

	if (gameWon) {
		return;
	}

	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(Player1);

	gfx->setDrawColor(SDL_COLOR_YELLOW);

	gfx->drawTexture(flag, &flagPos, SDL_FLIP_NONE);

	for (auto key : balls) {
		gfx->drawTexture(key->Texture, key->pos, SDL_FLIP_NONE);
	}
}

void MyGame::renderUI() {
	if (gameWon) {
		gfx->setDrawColor(SDL_COLOR_YELLOW);
		gfx->useFont(Font_Large_Bold);
		gfx->drawText(mySystem->GetText("gameover"), 0, 500);
		gfx->useFont(Font_Bold);

		std::string correctballs = mySystem->GetText("CorrectBalls");
		correctballs += std::to_string(correctBalls);

		std::string Incorrectballs = mySystem->GetText("IncorrectBalls");
		Incorrectballs += std::to_string(IncorrectBalls);

		gfx->drawText(correctballs, 0, 100);
		gfx->drawText(Incorrectballs, 0, 130);

		std::string scoreStr = mySystem->GetText("score") + std::to_string(score);
		gfx->drawText(scoreStr, 0, 160);
	}
	else
	{
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->useFont(Font_Large_Main);
		gfx->drawText(mySystem->GetText("name"), 65, 27);

		gfx->useFont(Font_Main);
		gfx->drawText(mySystem->GetText(CurrentAnswer.c_str()), 65, 60);

	}




}



void MyGame::OnLanguageChanged()
{
	flag = mySystem->GetTexture("flag");

	if (Font_Large_Bold != nullptr) {
		TTF_CloseFont(Font_Large_Bold);
		TTF_CloseFont(Font_Bold);
		TTF_CloseFont(Font_Large_Main);
		TTF_CloseFont(Font_Main);

	}

	Font_Large_Bold = mySystem->GetFont("bold", 72);
	Font_Bold = mySystem->GetFont("bold", 30);

	Font_Large_Main = mySystem->GetFont("main", 30);
	Font_Main = mySystem->GetFont("main", 20);
}
