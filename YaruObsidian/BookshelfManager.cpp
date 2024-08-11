#pragma once

#include "NarouNovelManager.h"
#include "HamelnNovelManager.h"
#include "NocNovelManager.h"
#include "YaruoManager.h"
#include "BookshelfManager.h"

#include <algorithm>
#include <locale>
#include <codecvt>
#include <Python.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <Windows.h>
//#include <winhttp.h>
#include <cstdlib>

namespace sstd { class sockSSL; }

void BookshelfManager::ManageBookshelf()
{
    GetMailInfo();
    std::cout << "Loading Bookshelves...\n";
    LoadBookshelf("Narou.csv");
    LoadBookshelf("Noc.csv");
    LoadBookshelf("Hameln.csv");
    LoadBookshelf("Yaruo.csv");
    std::cout << "Bookshelves Loading Finished\n";
    UpdateNovels();
    SendNotification();
    SendWarning();
}

void BookshelfManager::LoadBookshelf(std::string filename) {

    std::ifstream file("Bookshelves/" + filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> tokens;
        size_t pos = 0;
        std::string token;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);

        if (tokens.size() != 3) {
            std::cerr << "Invalid line: " << line << std::endl;
            continue;
        }

        BookshelfIndex bookshelf;
        bookshelf.NovelID = tokens[0];
        bookshelf.ChapterAmount = stoi(tokens[1]);
        bookshelf.LastUpdatedDate = tokens[2];

        if (filename == "Narou.csv")
        {
            NarouBookshelves.push_back(bookshelf);

        }
        if (filename == "Hameln.csv")
        {
            HamelnBookshelves.push_back(bookshelf);
        }
        if (filename == "Noc.csv")
        {
            NocBookshelves.push_back(bookshelf);
        }
        if (filename == "Yaruo.csv")
        {
            YaruoBookshelves.push_back(bookshelf);
        }
        
    }
    file.close();

    std::ofstream ofs("Bookshelves/" + filename, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file for truncating: " << filename << std::endl;
        return;
    }
    ofs.close();
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
    

    for (const auto& bookshelfIndex : NarouBookshelves) {
        NarouNovelManager* NarouManager = new NarouNovelManager;
        NarouManager->BookshelfManagerPtr = this;
        NarouManager->ManageNovel(bookshelfIndex);
        delete NarouManager;
    }
    for (const auto& bookshelfIndex : HamelnBookshelves) {
        HamelnNovelManager* Hamelnmanager = new HamelnNovelManager;
        Hamelnmanager->BookshelfManagerPtr = this;
        Hamelnmanager->ManageNovel(bookshelfIndex);
        delete Hamelnmanager;
    }
    for (const auto& bookshelfIndex : NocBookshelves) {
        NocNovelManager* NocManager = new NocNovelManager;
        NocManager->BookshelfManagerPtr = this;
        NocManager->ManageNovel(bookshelfIndex);
        delete NocManager;
    }
    for (const auto& bookshelfIndex : YaruoBookshelves) {
        YaruoManager* YaruManager = new YaruoManager;
        YaruManager->BookshelfManagerPtr = this;
        YaruManager->ManageNovel(bookshelfIndex);
        delete YaruManager;
    }
}

void BookshelfManager::GetMailInfo()
{

    std::vector<std::string> lines = get_mail_first_lines();

    std::regex URL(R"(https://)");

    // Print the lines
    for (const auto& line : lines) {

        // Regular expression to capture the domain part
        std::regex domainPattern(R"(https://([^/]+)/)");

        // Match object to hold the results for the domain
        std::smatch domainMatches;

        if (regex_search(line, domainMatches, domainPattern) && domainMatches.size() == 2) {
            std::string domain = domainMatches[1].str();

            if (domain == "ncode.syosetu.com") {
                // Regular expression to capture the code part
                std::regex ncodePattern(R"(https://ncode.syosetu.com/([^/]+)/)");
                std::smatch ncodeMatches;

                if (regex_search(line, ncodeMatches, ncodePattern) && ncodeMatches.size() == 2) {
                    std::string ncode = ncodeMatches[1].str();
                    AppendToBookshelf("Narou.csv", ncode + ",1,1");
                    std::cout << "New Novel : " << ncode<<"\n";
                }
            }

            if (domain == "syosetu.org") {
                // Regular expression to capture the code part
                std::regex HamPattern(R"(https://syosetu.org/novel/([^/]+)/)");
                std::smatch HamMatches;

                if (regex_search(line, HamMatches, HamPattern) && HamMatches.size() == 2) {
                    std::string HamID = HamMatches[1].str();
                    AppendToBookshelf("Hameln.csv", HamID + ",1,1");
                    std::cout << "New Novel : " << HamID << "\n";
                }
            }
            if (domain == "novel18.syosetu.com") {
                // Regular expression to capture the code part
                std::regex NocPattern(R"(https://novel18.syosetu.com/([^/]+)/)");
                std::smatch NocMatches;

                if (regex_search(line, NocMatches, NocPattern) && NocMatches.size() == 2) {
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

                if (regex_search(line, YaruoMatches, YaruoPattern) && YaruoMatches.size() == 2) {
                    std:: string YaruoCode = YaruoMatches[1].str();
                    AppendToBookshelf("Yaruo.csv", YaruoCode + ",1,1");
                    std::cout << "New Novel : " << YaruoCode << "\n";
                }
            }

        }
    }
}

std::vector<std::string> BookshelfManager::get_mail_first_lines()
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
            PyObject* pFunc = PyObject_GetAttrString(pModule, "get_mail_first_lines");

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
                    std::cerr << "Call to get_mail_first_lines failed" << std::endl;
                }
            }
            else {
                PyErr_Print();
                std::cerr << "Cannot find function get_mail_first_lines" << std::endl;
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