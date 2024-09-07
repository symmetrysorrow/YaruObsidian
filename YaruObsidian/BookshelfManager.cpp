#pragma once

#include "NarouNovelManager.h"
#include "HamelnNovelManager.h"
#include "NocNovelManager.h"
#include "YaruoManager.h"
#include "BookshelfManager.h"
#include "BookshelfIndex.h"

#include <algorithm>
#include <locale>
#include <codecvt>
#include <Python.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <Windows.h>
//#include <winhttp.h>
#include<nlohmann/json.hpp>
#include <cstdlib>

namespace sstd { class sockSSL; }

BookshelfManager::BookshelfManager()
	//:HamelnMG(new HamelnNovelManager()),
	//NarouMG(new NarouNovelManager()),
	//NocMG(new NocNovelManager()),
	//YaruoMG(new YaruoManager())
{
}

void BookshelfManager::AddNovel(const std::string& NovelURL)
{
    BookshelfIndex NewNovel;
    // Regular expression to capture the domain part
    std::regex domainPattern(R"(https://([^/]+)/)");

    // Match object to hold the results for the domain
    std::smatch domainMatches;

    NewNovel.ChapterAmount = 0;
    NewNovel.IsUpdateRequired = true;
    NewNovel.LastUpdatedDate = "Today";

    if (regex_search(NovelURL, domainMatches, domainPattern) && domainMatches.size() == 2) {
        std::string domain = domainMatches[1].str();

        if (domain == "ncode.syosetu.com") {
            std::regex ncodePattern(R"(https://ncode.syosetu.com/([^/]+)/)");
            std::smatch ncodeMatches;
            if (regex_search(NovelURL, ncodeMatches, ncodePattern) && ncodeMatches.size() == 2) {
                NewNovel.NovelID = ncodeMatches[1].str();
                NewNovel.Directory = "Novel/Narou";
                NarouBooks[NewNovel.NovelID] = NewNovel;
                NarouNovelManager* NarouMngr = new NarouNovelManager();
                NarouMngr->NovelIndex = &NewNovel;
                NarouMngr->GetNovelInfo();
                std::cout << "flag1\n";
                std::cout << "New Novel : " << NewNovel.NovelTitle << "\n";
                SaveBookshelves(NovelSite::Narou);
                LoadBookshelf(NovelSite::Narou);
                return;
            }
        }

        if (domain == "syosetu.org") {
            std::regex HamPattern(R"(https://syosetu.org/novel/([^/]+)/)");
            std::smatch HamMatches;

            if (regex_search(NovelURL, HamMatches, HamPattern) && HamMatches.size() == 2) {
                NewNovel.NovelID = HamMatches[1].str();
                NewNovel.Directory = "Novel/Hameln";
                HamelnBooks[NewNovel.NovelID] = NewNovel;
                std::cout << "New Novel : " << NewNovel.NovelTitle << "\n";
                SaveBookshelves(NovelSite::Hameln);
                LoadBookshelf(NovelSite::Hameln);
                return;
            }
        }
        if (domain == "novel18.syosetu.com") {
            std::regex NocPattern(R"(https://novel18.syosetu.com/([^/]+)/)");
            std::smatch NocMatches;

            if (regex_search(NovelURL, NocMatches, NocPattern) && NocMatches.size() == 2) {
                NewNovel.NovelID = NocMatches[1].str();
                NewNovel.Directory = "Novel/Noc";
                NocBooks[NewNovel.NovelID] = NewNovel;
                std::cout << "New Novel : " << NewNovel.NovelTitle << "\n";
                SaveBookshelves(NovelSite::Noc);
                LoadBookshelf(NovelSite::Noc);
                return;
            }
        }
        if (domain == "rss.r401.net")
        {
            std::regex YaruoPattern(R"(https://rss.r401.net/yaruo/categories/([^/]+))");
            std::smatch YaruoMatches;

            if (regex_search(NovelURL, YaruoMatches, YaruoPattern) && YaruoMatches.size() == 2) {
                NewNovel.NovelID = YaruoMatches[1].str();
                NewNovel.Directory = "Yaruo";
                YaruoBooks[NewNovel.NovelID] = NewNovel;
                std::cout << "New Novel : " << NewNovel.NovelTitle << "\n";
                SaveBookshelves(NovelSite::Yaruo);
                LoadBookshelf(NovelSite::Yaruo);
                return;
            }
        }
    }
    std::cout << "Invalid URL\n";
}

void BookshelfManager::DeleteNovel(const NovelSite& Site, std::string& NovelID)
{
}

void BookshelfManager::UpdateNovel(const NovelSite& Site, std::string& NovelID)
{
}

void BookshelfManager::UpdateAll(const NovelSite& Site)
{
    std::map<std::string, BookshelfIndex>* Bookshelf = GetBookshelf(Site);

    for (const std::pair<const std::string, BookshelfIndex>& index : *Bookshelf) {
        NarouNovelManager* NarouMngr = new NarouNovelManager();
        NarouMngr->NovelIndex = const_cast<BookshelfIndex*>(&index.second);
        
    }
}

void BookshelfManager::MoveNovel(const NovelSite& Site, std::string& NovelID)
{
}

void BookshelfManager::ManageBookshelf()
{
    std::cout << "Loading Bookshelves...\n";
    LoadBookshelf(NovelSite::Hameln);
    LoadBookshelf(NovelSite::Narou);
    LoadBookshelf(NovelSite::Yaruo);
    LoadBookshelf(NovelSite::Noc);
    std::cout << "Bookshelves Loading Finished\n";
    UpdateNovels();
    SendNotification();
    SendWarning();
}

void BookshelfManager::OrganizeNovel(const NovelSite& Site, std::string& NovelID)
{
}

void BookshelfManager::LoadBookshelf(const NovelSite& Site) {

    nlohmann::ordered_json json_obj;
    std::string filename;
    std::map<std::string, BookshelfIndex>* Bookshelf;

    switch (Site)
    {
    case::Hameln:
        Bookshelf = &HamelnBooks;
        filename = "./.Bookshelves/Hameln.json";
        break;
    case::Narou:
        Bookshelf = &NarouBooks;
        filename = "./.Bookshelves/Narou.json";
        break;
    case::Yaruo:
        Bookshelf = &YaruoBooks;
        filename = "./.Bookshelves/Yaruo.json";
        break;
    case::Noc:
        Bookshelf = &NocBooks;
        filename = "./.Bookshelves/Noc.json";
        break;
    default:
        Bookshelf = nullptr;
        break;
    }
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::ofstream new_file(filename);
        new_file.close();
        return;
    }
    file >> json_obj; 
    file.close();
    if(!Bookshelf)
    {
        std::cout << "Bookshelf is nullptr\n";
    }
    for (auto it = json_obj.begin(); it != json_obj.end(); ++it) {
        BookshelfIndex index;
        index.ChapterAmount = it.value().at("ChapterAmount");
        index.NovelTitle = it.value().at("NovelTitle");
        index.Directory = it.value().at("Directory");
        index.IsUpdateRequired = it.value().at("IsUpdateRequired");
        index.LastUpdatedDate = it.value().at("LastUpdatedDate");
        index.NovelID = it.key();
       ( *Bookshelf)[it.key()] = index;
    }
}

void BookshelfManager::AddNovelInfo(const NovelSite& Site, const BookshelfIndex& Index)
{
    std::map<std::string, BookshelfIndex>* Bookshelf;
    switch(Site)
    {
    case::Hameln:
        Bookshelf = &HamelnBooks;
        break;
    case::Yaruo:
        Bookshelf = &YaruoBooks;
        break;
    case::Narou:
        Bookshelf = &NarouBooks;
        break;
    case::Noc:
        Bookshelf = &NocBooks;
        break;
    default:
    	Bookshelf = nullptr;
    }

    (*Bookshelf)[Index.NovelID] = Index;
}

bool BookshelfManager::stringToBool(const std::string& str)
{
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower); // 小文字に変換
    return (lower_str == "true");
}

void BookshelfManager::AppendToBookshelf(const std::string& filename, const std::string& data)
{
    std::ofstream file("Bookshelves/" + filename, std::ios::app); // 追記モードでファイルを開く
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << data << std::endl; // データをファイルに書き込む

    file.close();
}

void BookshelfManager::UpdateNovels()
{
    
    std::sort(NarouBookshelves.begin(), NarouBookshelves.end());
    NarouBookshelves.erase(std::unique(NarouBookshelves.begin(),NarouBookshelves.end()), NarouBookshelves.end());
    
    std::sort(HamelnBookshelves.begin(), HamelnBookshelves.end());
    auto Hit = std::unique(HamelnBookshelves.begin(), HamelnBookshelves.end());
    HamelnBookshelves.erase(Hit, HamelnBookshelves.end());

    std::sort(NocBookshelves.begin(), NocBookshelves.end());
    auto Nocit = std::unique(NocBookshelves.begin(), NocBookshelves.end());
    NocBookshelves.erase(Nocit, NocBookshelves.end());

    std::sort(YaruoBookshelves.begin(), YaruoBookshelves.end());
    auto Yit = std::unique(YaruoBookshelves.begin(), YaruoBookshelves.end());
    YaruoBookshelves.erase(Yit, YaruoBookshelves.end());
}

std::string BookshelfManager::GetCSVFile(const NovelSite& Site)
{
   
	std::string filename;
	switch (Site)
	{
	case::Hameln:
		filename = "Hameln.csv";
		break;
	case::Narou:
		filename = "Narou.csv";
		break;
	case::Noc:
		filename = "Noc.csv";
		break;
	case::Yaruo:
		filename = "yaruo.csv";
		break;
	}
    return filename;


}

std::map<std::string, BookshelfIndex>* BookshelfManager::GetBookshelf(const NovelSite& Site)
{
    std::map<std::string, BookshelfIndex>* Bookshelf;

    switch (Site)
    {
    case::Hameln:
        Bookshelf = &HamelnBooks;
        break;
    case::Narou:
        Bookshelf = &NarouBooks;
        break;
    case::Yaruo:
        Bookshelf = &YaruoBooks;
        break;
    case::Noc:
        Bookshelf = &NocBooks;
        break;
    default:
        Bookshelf = nullptr;
        break;
    }

    return Bookshelf;
}

std::string BookshelfManager::GetJsonPath(const NovelSite& Site)
{
    std::string filename;
    switch (Site)
    {
    case::Hameln:
        filename = "./.Bookshelves/Hameln.json";
        break;
    case::Narou:
        filename = "./.Bookshelves/Narou.json";
        break;
    case::Yaruo:
        filename = "./.Bookshelves/Yaruo.json";
        break;
    case::Noc:
        filename = "./.Bookshelves/Noc.json";
        break;
    default:
        filename = "";
        break;
    }
    return filename;
}

void BookshelfManager::SaveBookshelves(const NovelSite& Site)
{
    nlohmann::ordered_json json_obj;
    std::string filename=GetJsonPath(Site);
    std::map<std::string, BookshelfIndex>* Bookshelf=GetBookshelf(Site);

    for(const auto& NovelIndex:*Bookshelf)
    {
        json_obj[NovelIndex.first]["ChapterAmount"] = NovelIndex.second.ChapterAmount;
        json_obj[NovelIndex.first]["LastUpdatedDate"] = NovelIndex.second.LastUpdatedDate;
        json_obj[NovelIndex.first]["NovelTitle"] = NovelIndex.second.NovelTitle;
        json_obj[NovelIndex.first]["Directory"] = NovelIndex.second.Directory;
        json_obj[NovelIndex.first]["IsUpdateRequired"] = NovelIndex.second.IsUpdateRequired;
    }
    std::ofstream JsonFile(filename);
    JsonFile << std::setw(4) << json_obj;
    JsonFile.close();
}


void BookshelfManager::GetMailInfo()
{

    std::vector<std::string> NovelURLs = get_mail_first_NovelURLs();

    std::regex URL(R"(https://)");

    // Print the NovelURLs
    for (const auto& NovelURL : NovelURLs) {

        // Regular expression to capture the domain part
        std::regex domainPattern(R"(https://([^/]+)/)");

        // Match object to hold the results for the domain
        std::smatch domainMatches;

        if (regex_search(NovelURL, domainMatches, domainPattern) && domainMatches.size() == 2) {
            std::string domain = domainMatches[1].str();

            if (domain == "ncode.syosetu.com") {
                // Regular expression to capture the code part
                std::regex ncodePattern(R"(https://ncode.syosetu.com/([^/]+)/)");
                std::smatch ncodeMatches;

                if (regex_search(NovelURL, ncodeMatches, ncodePattern) && ncodeMatches.size() == 2) {
                    std::string ncode = ncodeMatches[1].str();
                    AppendToBookshelf("Narou.csv", ncode + ",1,1");
                    std::cout << "New Novel : " << ncode<<"\n";
                }
            }

            if (domain == "syosetu.org") {
                // Regular expression to capture the code part
                std::regex HamPattern(R"(https://syosetu.org/novel/([^/]+)/)");
                std::smatch HamMatches;

                if (regex_search(NovelURL, HamMatches, HamPattern) && HamMatches.size() == 2) {
                    std::string HamID = HamMatches[1].str();
                    AppendToBookshelf("Hameln.csv", HamID + ",1,1");
                    std::cout << "New Novel : " << HamID << "\n";
                }
            }
            if (domain == "novel18.syosetu.com") {
                // Regular expression to capture the code part
                std::regex NocPattern(R"(https://novel18.syosetu.com/([^/]+)/)");
                std::smatch NocMatches;

                if (regex_search(NovelURL, NocMatches, NocPattern) && NocMatches.size() == 2) {
                    std::string NocCode = NocMatches[1].str();
                    AppendToBookshelf("Noc.csv", NocCode + ",1,1");
                    std::cout << "New Novel : " << NocCode << "\n";
                }
            }
            if (domain == "rss.r401.net")
            {
                // Regular expression to capture the code part
                std::regex YaruoPattern(R"(https://rss.r401.net/yaruo/categories/([^/]+))");
                std::smatch YaruoMatches;

                if (regex_search(NovelURL, YaruoMatches, YaruoPattern) && YaruoMatches.size() == 2) {
                    std:: string YaruoCode = YaruoMatches[1].str();
                    AppendToBookshelf("Yaruo.csv", YaruoCode + ",1,1");
                    std::cout << "New Novel : " << YaruoCode << "\n";
                }
            }

        }
    }
}

std::vector<std::string> BookshelfManager::get_mail_first_NovelURLs()
{
        std::vector<std::string> result;

        std::filesystem::path exePath = std::filesystem::current_path();
        exePath /= "MailFetcher.py";

        // MailFetcher.pyが存在するかチェック
        if (!std::filesystem::exists(exePath)) {
            return result;
        }
        std::cout << "Fetching Email...\n";

        // Initialize Python interpreter
        Py_Initialize();

        // Load the Python module
        PyObject* pName = PyUnicode_DecodeFSDefault("MailFetcher");
        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != nullptr) {
            // Get the function from the module
            PyObject* pFunc = PyObject_GetAttrString(pModule, "get_mail_first_NovelURLs");

            if (pFunc && PyCallable_Check(pFunc)) {
                // Call the function
                PyObject* pValue = PyObject_CallObject(pFunc, nullptr);

                if (pValue != nullptr) {
                    // Check if the result is a list
                    if (PyList_Check(pValue)) {
                        // Get the size of the list
                        Py_ssize_t size = PyList_Size(pValue);

                        for (Py_ssize_t i = 0; i < size; i++) {
                            // Get the item from the list
                            PyObject* pItem = PyList_GetItem(pValue, i);
                            if (PyUnicode_Check(pItem)) {
                                // Convert Python string to C++ string
                                const char* str = PyUnicode_AsUTF8(pItem);
                                result.push_back(str);
                            }
                        }
                    }
                    Py_DECREF(pValue);
                }
                else {
                    PyErr_Print();
                    std::cerr << "Call to get_mail_first_NovelURLs failed" << std::endl;
                }
            }
            else {
                PyErr_Print();
                std::cerr << "Cannot find function get_mail_first_NovelURLs" << std::endl;
            }
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            std::cerr << "Failed to load example module" << std::endl;
        }

        // Finalize Python interpreter
        Py_Finalize();
        std::cout << "Email Data Extraction Finished\n";
        return result;
    
}

void BookshelfManager::SendNotification()
{
    std::filesystem::path exePath = std::filesystem::current_path();
    exePath /= "sendMail.py";

    if (!UpdateMailText.empty()&& std::filesystem::exists(exePath))
    {
        std::string Command = "python SendMail.py 更新がありました "+UpdateMailText;
        system(Command.c_str());
        std::cout << "Send Notification\n";
    }
    if(UpdateMailText.empty())
    {
        std::cout << "There is no novel to be updated\n";
    }       
}

void BookshelfManager::SendWarning()
{
    std::filesystem::path exePath = std::filesystem::current_path();
    exePath /= "sendMail.py";

    // MailFetcher.pyが存在するかチェック
    if (!std::filesystem::exists(exePath)) {
        return;
    }

    if (!WarningMailText.empty())
    {
        std::string Command = "python SendMail.py 話数が減少した小説がありました "+WarningMailText;
        system(Command.c_str());
    }
    
}