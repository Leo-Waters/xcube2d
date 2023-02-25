#include "MyEngineSystem.h"
namespace fs = std::filesystem;

std::vector<std::string> MyEngineSystem::GetAvalibleLanguages(){
     std::cout <<"Getting avalible Lanuages"<< std::endl;
     std::vector<std::string> languages = std::vector<std::string>();

    for (const auto & entry : fs::directory_iterator(LoadPath)){

        if (entry.path().extension() == ".lang") {
            std::string NameWithExt = entry.path().filename().string();

            std::string name= NameWithExt.substr(0, NameWithExt.find_last_of("."));
            languages.push_back(name);

            std::cout << name << std::endl;
        }
        else {
            std::cout << entry.path().filename().string() << " is not a lang file";
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
    std::fstream file;

    file.open(Dir, std::ios::in);

    if (file.is_open()) {
        std::string line;

        std::string key = "";
        std::string value = "";
        bool GettingValue = false;
        bool QuoteStart = false;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;


            for (size_t i = 0; i < line.length(); i++)
            {
                if (!GettingValue && line[i] == ' ') {
                    continue;
                }
                if (line[i] == '=') {
                    GettingValue = true;
                    continue;
                }
                

                if (!QuoteStart && line[i] == '"') {
                    QuoteStart = true;
                    continue;
                }
                else if (QuoteStart && line[i] == '"')
                {
                    GettingValue = QuoteStart = false;
                    if (key == " " || value == " "|| key == "" || value == "") {
                        std::cout << "\nCannot add an entry with an empty language key or value!!\nLanguage file :"<< Language<<" Ending on Line " << lineNumber << " [" << i << "]\n";

                    }
                    else if (Translations.count(key)) {
                        std::cout << "\nCannot Load a duplicate language key!!\nLanguage file :" << Language << " Ending on Line " << lineNumber<<" ["<<i<<"]\n";
                    }
                    else
                    {
                        std::cout << value << "\n";
                        Translations.insert(std::pair<std::string, std::string>(key, value));
                    }
                    
                    
                    key = "";
                    value = "";
                    
                    continue;
                }

                if (GettingValue && QuoteStart) {
                    value += line[i];
                }
                else if (!GettingValue)
                {
                    key += line[i];
                }

            }

            

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