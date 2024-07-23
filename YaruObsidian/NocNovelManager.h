#pragma once
#include "NovelManager.h"

class NocNovelManager :public NovelManager {

	virtual void UpdateNovel(const int& ChapterAmount)override;
	virtual void GetNovelInfo()override;
	virtual void UpdateShort()override;
	//virtual std::string GetHTML(const std::string& URL)override;
};