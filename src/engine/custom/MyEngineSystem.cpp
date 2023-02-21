#include "MyEngineSystem.h"
namespace fs = std::filesystem;

std::vector<std::string> MyEngineSystem::GetAvalibleLanguages(){
     std::cout <<"Getting avalible Lanuages"<< std::endl;
     std::vector<std::string> languages = std::vector<std::string>();

    for (const auto & entry : fs::directory_iterator(LoadPath)){

        if (entry.path().extension() == ".lang") {
            languages.push_back(entry.path().filename().string());

            std::cout << entry.path().filename().string() << std::endl;
        }
    }
    std::cout << "-----------------------------" << std::endl;
    return languages;
}


bool MyEngineSystem::LoadLanguageFile(const char* Language)
{

    std::string Dir = std::string(LoadPath);
    Dir += Language;
    Dir += ".lang";
    //printf(Dir.c_str());
    std::fstream file;

    file.open(Dir, std::ios::in);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            
            std::string key = "";
            std::string value = "";
            bool GettingValue = false;

            for (size_t i = 0; i < line.length(); i++)
            {
                if (line[i] == '=') {
                    GettingValue = true;
                    continue;
                }
                if (GettingValue) {
                    value += line[i];
                }
                else
                {
                    key += line[i];
                }
            }

            Translations.insert(std::pair<std::string, std::string>(key, value));

        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
    
}
void MyEngineSystem::SetLanguage(const char* Language){
    //clear pervious language
    Translations.clear();
    currentLanguage = Language;
    //load language
    if (LoadLanguageFile(Language)) {//loaded sucessfully
        std::cout << "Loaded lang file: " << currentLanguage << std::endl;
    }//Debug failed to load language
    else{ std::cout << "Failed to load lang file: " << currentLanguage << std::endl; }
}
std::string MyEngineSystem::GetText(const char* ID){

    if (Translations.count(ID)) {
        return Translations.at(ID);
    }
    else
    {
        std::cout << "No Entry of '" << ID << "' in lang file: " << currentLanguage << std::endl;
        return std::string("No Text in Lang File");
    }

}