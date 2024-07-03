#pragma once
#include "NovelManager.h"

class HamelnNovelManager :public NovelManager {

	virtual void UpdateNovel(const int& ChapterAmount)override;
	virtual void GetNovelInfo()override;
	virtual void UpdateShort()override;
};