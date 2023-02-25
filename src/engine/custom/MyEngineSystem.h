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
/// <summary>
/// 
/// Module CI517 Game Engine Fundamentals 
/// Leo Waters
/// Localization subsystem
/// 
/// Provide localized language for text
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
		// map    Key ID --- Text Value
		std::map<std::string,std::string>Translations;
		std::map<std::string, std::string>Fonts;
		std::string currentLanguage;

		bool LoadLanguageFile(const char* Language);

		inline void AddTranslation(std::string key, std::string value, const char* Language, int lineNumber, int Index);
		inline void AddFont(std::string key, std::string value, const char* Language, int lineNumber, int Index);

	public:
		std::vector<std::string> GetAvalibleLanguages();
		void SetLanguage(const char* Language);
		std::string GetText(const char* ID);
		TTF_Font* GetFont(const char* ID,const int& pointsize);
};

#endif