#include "MyEngineSystem.h"

namespace fs = std::filesystem;




bool MyEngineSystem::LoadLanguageFile(const char* Language)
{
    //the directory the language files are stored
    std::string Dir = std::string(LangLoadPath);

    //adds the specific language file to the path
    Dir += Language;
    Dir += ".lang";
    std::fstream file;

    //opens the file
    file.open(Dir, std::ios::in);

    //did the file open correctly
    if (file.is_open()) {
        std::string line;

        //main enclosing tag type
        bool GettingFonts = false;
        bool GettingTranslation = false;
        bool GettingAssets = false;

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

                    if (OpenTag == true) {//is it a tag oppening an element or category
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
                        else if (TagValue == "assets") {//getting translations
                            GettingAssets = true;
                            TagValue = "";
                            continue;
                        }
                        else{ GettingElement = true; continue; }//getting an element
                    }
                    else//element is closing an element or category
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
                        else if (TagValue == "assets") {//no longer getting translations
                            GettingAssets = false;
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
                            else if (GettingAssets)
                            {
                                //add translation
                                AddAsset(TagValue, ElementValue, Language, lineNumber, i);
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

        if (GettingFonts) {
            std::cout << "\nERROR: Fonts Tag wasnt closed in Language File :" << Language << "\n\n";
        }
        if (GettingTranslation) {
            std::cout << "\nERROR: Translations Tag wasnt closed in Language File :" << Language << "\n\n";
        }
        if (GettingAssets) {
            std::cout << "\nERROR: Assets Tag wasnt closed in Language File :" << Language << "\n\n";
        }
        if (GettingElement) {
            std::cout << "\nERROR: Element Tag wasnt closed in Language File :" << Language << "\n\n";
        }
        file.close();//end of file
        return true;//loaded language 
    }
    else// file didnt exist, failed to load language
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
inline void MyEngineSystem::AddAsset(std::string key, std::string value, const char* Language, int lineNumber, int Index)
{
    if (key == " " || value == " " || key == "" || value == "") {
        std::cout << "\nERROR: Cannot add an entry without a localized texture key or value!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";

    }
    else if (Assets.count(key)) {
        std::cout << "\nERROR: Cannot Load a localized texture key!!\nLanguage file :" << Language << " Ending on Line " << lineNumber << " [" << Index << "]\n\n";
    }
    else
    {
        std::cout << "Asset " << key << " :" << value << "\n";
        Assets.insert(std::pair<std::string, std::string>(key, value));
    }
}




std::vector<std::string> MyEngineSystem::GetAvalibleLanguages() {
    std::cout << "Getting avalible Lanuages" << std::endl;
    std::vector<std::string> languages = std::vector<std::string>();

    for (const auto& entry : fs::directory_iterator(LangLoadPath)) {

        if (entry.path().extension() == ".lang") {
            std::string NameWithExt = entry.path().filename().string();

            std::string name = NameWithExt.substr(0, NameWithExt.find_last_of("."));
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

void MyEngineSystem::SetLanguage(const char* Language, std::function<void()> OnLoadLanguage){
    //clear pervious language
    Translations.clear();
    Assets.clear();
    Fonts.clear();
    currentLanguage = Language;
    //load language
    if (LoadLanguageFile(Language)) {//loaded sucessfully
        std::cout << "Loaded lang file: " << currentLanguage << std::endl;
        OnLoadLanguage();
    }//Debug failed to load language
    else{ std::cout << "Failed to load lang file: " << currentLanguage << std::endl; }
}
std::string MyEngineSystem::GetText(const char* ID){

    if (Translations.count(ID)) {//does ID exist
        return Translations.at(ID);
    }
    else// no log error
    {
        std::cout << "No Entry of '" << ID << "' in lang file: " << currentLanguage << std::endl;
        return std::string("No Text in Lang File");
    }

}

TTF_Font* MyEngineSystem::GetFont(const char* ID,const int& pointsize)
{
    if (Fonts.count(ID)) {//does ID exist
        return ResourceManager::loadFont(FontLoadPath+Fonts.at(ID), pointsize);
    }
    else// no log error
    {
        std::cout << "No Entry of '" << ID << "' in fonts from "<<currentLanguage<<" Lang File" << std::endl;
        return nullptr;
    }
    
}

SDL_Texture* MyEngineSystem::GetTexture(const char* ID)
{
    if (Assets.count(ID)) {//does ID exist
        std::string TexutureName = AssetLoadPath + Assets.at(ID);
        return ResourceManager::getTexture(TexutureName);
    }
    else// no log error
    {
        std::cout << "No Entry of '" << ID << "' in assets from " << currentLanguage << " Lang File" << std::endl;
        return nullptr;
    }

    
    
}

void MyEngineSystem::EasterEgg()
{
    std::cout << "LEO_WATERS-CI517-2022" << std::endl;
}
