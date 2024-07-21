#pragma once

#include "NarouNovelManager.h"
#include "HamelnNovelManager.h"
#include "NocNovelManager.h"
#include "YaruoManager.h"

#include "BookshelfManager.h"
#include <locale>
#include <codecvt>
#include <Python.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <winhttp.h>
#include <cstdlib>

namespace sstd { class sockSSL; }

void BookshelfManager::ManageBookshelf()
{
    GetMailInfo();
    cout << "Loading Bookshelves...\n";
    LoadBookshelf("Narou.csv");
    LoadBookshelf("Noc.csv");
    LoadBookshelf("Hameln.csv");
    LoadBookshelf("Yaruo.csv");
    cout << "Bookshelves Loading Finished\n";
    UpdateNovels();
    SendNotification();
    SendWarning();
}

void BookshelfManager::LoadBookshelf(std::string filename) {

    ifstream file("Bookshelves/" + filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
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
            cerr << "Invalid line: " << line << endl;
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

    ofstream ofs("Bookshelves/" + filename, ofstream::out | ofstream::trunc);
    if (!ofs.is_open()) {
        cerr << "Failed to open file for truncating: " << filename << endl;
        return;
    }
    ofs.close();
}

void BookshelfManager::AppendToBookshelf(const std::string& filename, const std::string& data)
{
    ofstream file("Bookshelves/" + filename, ios::app); // 追記モードでファイルを開く
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    file << data << endl; // データをファイルに書き込む

    file.close();
}

void BookshelfManager::UpdateNovels()
{
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
        regex domainPattern(R"(https://([^/]+)/)");

        // Match object to hold the results for the domain
        smatch domainMatches;

        if (regex_search(line, domainMatches, domainPattern) && domainMatches.size() == 2) {
            std::string domain = domainMatches[1].str();

            if (domain == "ncode.syosetu.com") {
                // Regular expression to capture the code part
                regex ncodePattern(R"(https://ncode.syosetu.com/([^/]+)/)");
                smatch ncodeMatches;

                if (regex_search(line, ncodeMatches, ncodePattern) && ncodeMatches.size() == 2) {
                    std::string ncode = ncodeMatches[1].str();
                    AppendToBookshelf("Narou.csv", ncode + ",1,1");
                    cout << "New Novel : " << ncode<<"\n";
                }
            }

            if (domain == "syosetu.org") {
                // Regular expression to capture the code part
                regex HamPattern(R"(https://syosetu.org/novel/([^/]+)/)");
                smatch HamMatches;

                if (regex_search(line, HamMatches, HamPattern) && HamMatches.size() == 2) {
                    std::string HamID = HamMatches[1].str();
                    AppendToBookshelf("Hameln.csv", HamID + ",1,1");
                    cout << "New Novel : " << HamID << "\n";
                }
            }
            if (domain == "novel18.syosetu.com") {
                // Regular expression to capture the code part
                regex NocPattern(R"(https://novel18.syosetu.com/([^/]+)/)");
                smatch NocMatches;

                if (regex_search(line, NocMatches, NocPattern) && NocMatches.size() == 2) {
                    std::string NocCode = NocMatches[1].str();
                    AppendToBookshelf("Noc.csv", NocCode + ",1,1");
                    cout << "New Novel : " << NocCode << "\n";
                }
            }
            if (domain == "rss.r401.net")
            {
                // Regular expression to capture the code part
                regex YaruoPattern(R"(https://rss.r401.net/yaruo/categories/([^/]+))");
                smatch YaruoMatches;

                if (regex_search(line, YaruoMatches, YaruoPattern) && YaruoMatches.size() == 2) {
                    std:: string YaruoCode = YaruoMatches[1].str();
                    AppendToBookshelf("Yaruo.csv", YaruoCode + ",1,1");
                    cout << "New Novel : " << YaruoCode << "\n";
                }
            }

        }
    }
}

std::vector<std::string> BookshelfManager::get_mail_first_lines()
{
        std::vector<std::string> result;

        filesystem::path exePath = filesystem::current_path();
        exePath /= "MailFetcher.py";

        // MailFetcher.pyが存在するかチェック
        if (!filesystem::exists(exePath)) {
            return result;
        }
        cout << "Fetching Email...\n";

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
                    cerr << "Call to get_mail_first_lines failed" << endl;
                }
            }
            else {
                PyErr_Print();
                cerr << "Cannot find function get_mail_first_lines" << endl;
            }
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            cerr << "Failed to load example module" << endl;
        }

        // Finalize Python interpreter
        Py_Finalize();
        cout << "Email Data Extraction Finished\n";
        return result;
    
}

void BookshelfManager::SendNotification()
{
    filesystem::path exePath = filesystem::current_path();
    exePath /= "sendMail.py";

    if (!UpdateMailText.empty()&& filesystem::exists(exePath))
    {
        std::string Command = "python SendMail.py 更新がありました "+UpdateMailText;
        system(Command.c_str());
        cout << "Send Notification";
    }
    if(UpdateMailText.empty())
    {
        cout << "There is no novel to be updated\n";
    }       
}

void BookshelfManager::SendWarning()
{
    filesystem::path exePath = filesystem::current_path();
    exePath /= "sendMail.py";

    // MailFetcher.pyが存在するかチェック
    if (!filesystem::exists(exePath)) {
        return;
    }

    if (!WarningMailText.empty())
    {
        std::string Command = "python SendMail.py 話数が減少した小説がありました "+WarningMailText;
        system(Command.c_str());
    }
    
}