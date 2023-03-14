#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
#include "../include/SDL_ttf.h"
#include "../ResourceManager.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>
/// <summary>
/// 
/// Module CI517 Game Engine Fundamentals 
/// Leo Waters
/// Localization subsystem
/// 
/// Provides localized language for text objects, localized textures and localized fonts
/// 
/// 14/02/2023
/// 
/// </summary>
class MyEngineSystem {
	friend class XCube2Engine;
	private:
		//path of the language files folder
		const char * LangLoadPath="./res/lang/";
		const char*  FontLoadPath = "./res/fonts/";
		const char*  AssetLoadPath = "./res/textures/";
		// map    Key ID --- Text Value
		std::map<std::string,std::string>Translations;
		std::map<std::string, std::string>Fonts;
		std::map<std::string, std::string>Assets;
		std::string currentLanguage;

		//load a language file, returns true on success
		bool LoadLanguageFile(const char* Language);

		//Adds a translation to the storage map and provides debugging
		inline void AddTranslation(std::string key, std::string value, const char* Language, int lineNumber, int Index);
		//Adds a font to the storage map and provides debugging
		inline void AddFont(std::string key, std::string value, const char* Language, int lineNumber, int Index);
		//Adds a texture asset to the storage map and provides debugging
		inline void AddAsset(std::string key, std::string value, const char* Language, int lineNumber, int Index);

	public:
		//returns a list of all the avalible languages in the lang rescource folder
		std::vector<std::string> GetAvalibleLanguages();
		//sets the language based on the language files name and calls the inputted function on success for loading/disposing textures and fonts etc.
		void SetLanguage(const char* Language, std::function<void()> OnLoadLanguage);
		//gets a translation based on its ID
		std::string GetText(const char* ID);
		//creates a font based on its ID and size
		TTF_Font* GetFont(const char* ID,const int& pointsize);
		//gets a localized texture based on its ID
		SDL_Texture* GetTexture(const char* ID);
};

#endif