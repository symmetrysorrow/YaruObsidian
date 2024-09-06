#include "UserInterface.h"
#include "YaruoManager.cpp"
#include "HamelnNovelManager.cpp"
#include "NarouNovelManager.cpp"
#include "NocNovelManager.cpp"
#include <iostream>

void UserInterface::ManageCommand(const std::string& command)
{

}

void UserInterface::AddNovelCmd()
{
	std::cout << "Enter novel URL: ";
	std::string NovelURL;
	std::cin >> NovelURL;
	BookMG.AddNovel(NovelURL);
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
	std::cout << "Which novel do you want to move?";
}

void UserInterface::OrganizeNovelsCmd()
{
	std::cout << "Which novel do you want to Organize?";
}

void UserInterface::StopUpdateCmd()
{
	std::cout << "Which novel do you want to stp updating?\n";
}
