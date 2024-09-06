#include "UserInterface.h"
#include "YaruoManager.h"
#include "HamelnNovelManager.h"
#include "NarouNovelManager.h"
#include "NocNovelManager.h"
#include "BookshelfManager.h"
#include <iostream>

void UserInterface::ManageCommand(const std::string& input)
{
	std::istringstream iss(input);
	std::vector<std::string> tokens;
	std::string token;

    // 入力を空白で分割して、コマンドとオプションを取得
    while (iss >> token) {
        tokens.push_back(token);
    }

    // コマンドが空の場合は何もしない
    if (tokens.empty()) {
        std::cerr << "No command entered." << std::endl;
        return;
    }

    // コマンドに応じて処理を分岐
    const std::string& command = tokens[0];

    if (command == "add") {
        AddNovelCmd();
    }
    else if (command == "del") {
        if (tokens.size() > 1 && tokens[1] == "-all") {
            std::cout << "Delete all items." << std::endl;
        }
        else {
            DeleteNovelCmd();
        }
    }
    else if (command == "update") {
        if (tokens.size() > 1 && tokens[1] == "-all") {
            UpdateAllNovelsCmd();
        }
        else {
            UpdateNovelsCmd();
        }
    }
    else if (command == "move") {
        MoveNovelsCmd();
    }
    else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

UserInterface::UserInterface():
	BookMG(new BookshelfManager())
{
}

void UserInterface::AddNovelCmd()
{
	std::cout << "Enter novel URL: ";
	std::string NovelURL;
	std::cin >> NovelURL;
	BookMG->AddNovel(NovelURL);
}

void UserInterface::DeleteNovelCmd()
{
	std::cout << "Which novel do you want to delete?\n";
	std::string NovelNum;
	std::cin >> NovelNum;
}

void UserInterface::UpdateNovelsCmd()
{
	std::cout << "Which novel do you want to update?\n";
	std::string NovelNum;
	std::cin >> NovelNum;
}

void UserInterface::UpdateAllNovelsCmd()
{
	std::cout << "Updating AllNovels\n";
}

void UserInterface::MoveNovelsCmd()
{
	std::cout << "Which novel do you want to move?\n";
}

void UserInterface::OrganizeNovelsCmd()
{
	std::cout << "Which novel do you want to Organize?\n";
}

void UserInterface::StopUpdateCmd()
{
	std::cout << "Which novel do you want to stp updating?\n";
}
