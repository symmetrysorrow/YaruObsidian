#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <Wininet.h>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <vector>
#include <locale>
#include <filesystem>
#include "BookshelfIndex.h"


using namespace std;
class BookshelfManager;

class NovelManager {
public:
	NovelManager();
	void ManageNovel(BookshelfIndex Index);
	BookshelfManager* BookshelfManagerPtr;

protected:
	BookshelfIndex NovelIndex;
	std::string InfoURL;
	std::string NovelURL;
	std::string PreChapterTitle="";

	void CreateChapter(const std::string& NovelTitle, const std::string& ChapterTitle, const std::string& ChapterContents, NovelType novelSite);
	virtual std::string GetHTML(const std::string& URL);
	std::string GetTitle(std::string URL);
	std::string GetHTMLTitle(const std::string& HTML);
	std::string RemoveNewLines(const std::string& str);
	void SetCookie(const std::string& url, const std::string& cookie);
	std::string SanitizeFilename(const std::string& filename);
	std::string removeSubstring(const std::string& str, const std::string& substring);

	virtual void UpdateNovel(const int& ChapterAmount);
	virtual void UpdateShort();
	virtual void GetNovelInfo();
};