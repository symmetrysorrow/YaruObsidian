#pragma once
#include "NovelManager.h"

class YaruoManager :public NovelManager {
	std::string NovelInfoHTML;
	int ChapterAm = 1;

	std::string CorrectForYaruyomi(const std::string& HTML);
	std::string CorrectForRusalka(const std::string& HTML);
	std::string CorrectForMex(const std::string& HTML);
	std::string CorrectForEro(const std::string& HTML);
	std::string CorrectForIitokolo(const std::string& HTML);
	std::string CorrectForBura(const std::string& HTML);
	std::string CorrectForOyogu(const std::string& HTML);

	std::string removeBracketsContent(const std::string& input);


	void UpdateNovel(const int& CharacterAmount)override;
	void GetNovelInfo()override;
};