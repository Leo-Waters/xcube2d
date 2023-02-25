#include "MyEngineSystem.h"

namespace fs = std::filesystem;

std::vector<std::string> MyEngineSystem::GetAvalibleLanguages(){
     std::cout <<"Getting avalible Lanuages"<< std::endl;
     std::vector<std::string> languages = std::vector<std::string>();

    for (const auto & entry : fs::directory_iterator(LangLoadPath)){

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

    std::string Dir = std::string(LangLoadPath);
    Dir += Language;
    Dir += ".lang";
    std::fstream file;

    file.open(Dir, std::ios::in);

    if (file.is_open()) {
        std::string line;

        //main enclosing tag type
        bool GettingFonts = false;
        bool GettingTranslation = false;

        //getting element data
        bool GettingElement = false;
        std::string ElementValue="";

        //current tag
        bool GettingTag = false;
        bool OpenTag = false;
        std::string TagValue = "";

        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;


            for (size_t i = 0; i < line.length(); i++)
            {

                //has a tag been ended
                if (line[i] == '>') {
                    GettingTag = false;

                    if (OpenTag == true) {
                        if (TagValue == "fonts") {//getting fonts
                            GettingFonts = true;
                            TagValue = "";
                            continue;
                        }
                        else if (TagValue == "translations") {//getting translations
                            GettingTranslation = true;
                            TagValue = "";
                            continue;
                        }
                        else
                        {
                            GettingElement = true;
                            continue;
                        }
                    }
                    else
                    {
                        if (TagValue == "fonts") {//no longer adding fonts
                            GettingFonts = false;
                            TagValue = "";
                            continue;
                        }
                        else if (TagValue == "translations") {//no longer getting translations
                            GettingTranslation = false;
                            TagValue = "";
                            continue;
                        }
                        else
                        {
                            GettingElement = false;

                            if (GettingFonts) {
                                //add font
                                AddFont(TagValue, ElementValue, Language, lineNumber, i);
                                TagValue = "";
                                ElementValue = "";
                            }
                            else if(GettingTranslation)
                            {
                                //add translation
                                AddTranslation(TagValue, ElementValue, Language, lineNumber, i);
                                TagValue = "";
                                ElementValue = "";
                            }
                            

                            continue;
                        }

                    }
 
                }
                //has a tag been started
                if (line[i] == '<') {
                    GettingTag = true;
                    TagValue = "";
                    OpenTag = (line[i + 1] != '/');
                    continue;
                }
                //dosnt need proccessing
                if (line[i] == '/') {
                    continue;
                }
                //get tag information
                if (GettingTag) {
                    TagValue += line[i];
                    continue;
                }

                //get element information
                if (GettingElement) {
                    ElementValue += line[i];
                    continue;
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
inline void MyEngineSystem::AddTranslation(std::string key, std::string value, const char* Language,int lineNumber,int Index)
{
    if (key == " " || value == " " || key == "" || value == "") {
        std::cout << "\nERROR: Cannot add an entry with an empty language key or value!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";

    }
    else if (Translations.count(key)) {
        std::cout << "\nERROR: Cannot Load a duplicate language key!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";
    }
    else
    {
        std::cout << "lang " << key << " :" << value << "\n";
        Translations.insert(std::pair<std::string, std::string>(key, value));
    }
}
inline void MyEngineSystem::AddFont(std::string key, std::string value, const char* Language, int lineNumber, int Index)
{
    if (key == " " || value == " " || key == "" || value == "") {
        std::cout << "\nERROR: Cannot add an entry without a font key or value!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";

    }
    else if (Fonts.count(key)) {
        std::cout << "\nERROR: Cannot Load a duplicate font key!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";
    }
    else
    {
        std::cout <<"Font "<< key <<" :" << value << "\n";
        Fonts.insert(std::pair<std::string, std::string>(key, value));
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

TTF_Font* MyEngineSystem::GetFont(const char* ID,const int& pointsize)
{
    if (Fonts.count(ID)) {
        return ResourceManager::loadFont(FontLoadPath+Fonts.at(ID), pointsize);
    }
    else
    {
        std::cout << "No Entry of '" << ID << "' in fonts from "<<currentLanguage<<" Lang File" << std::endl;
        return nullptr;
    }
    
}
