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
#include <filesystem>
#include "BookshelfIndex.h"

class BookshelfManager;

class NovelManager {
public:
	void ManageNovel(BookshelfIndex Index);
	std::string GetTitle(std::string URL);
	BookshelfIndex* NovelIndex;
	virtual void GetNovelInfo();
protected:
	std::string InfoURL;
	std::string NovelURL;
	std::string PreChapterTitle="";
	std::string NovelTitle;
	std::string LastChapterPath;

	void CreateChapter(const std::string& ChapterTitle, const std::string& ChapterContents, NovelSite novelSite);
	virtual std::string GetHTML(const std::string& URL);
	
	std::string GetHTMLTitle(const std::string& HTML);
	std::string RemoveNewLines(const std::string& str);
	void SetCookie(const std::string& url, const std::string& cookie);
	std::string SanitizeFilename(const std::string& filename);
	std::string removeSubstring(const std::string& str, const std::string& substring);
	std::string RemoveLeadingSpace(const std::string& input);
	std::string decode_html_entities(const std::string& input);

	virtual void UpdateNovel(const int& ChapterAmount);
	virtual void UpdateShort();
	
};