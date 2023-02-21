#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
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
		const char * LoadPath="./res/lang/";
		// map    Key ID --- Text Value
		std::map<std::string,std::string>Translations;
		std::string currentLanguage;

		bool LoadLanguageFile(const char* Language);
	public:
		std::vector<std::string> GetAvalibleLanguages();
		void SetLanguage(const char* Language);
		std::string GetText(const char* ID);
};

#endif