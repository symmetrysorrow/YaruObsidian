#pragma once
#include "NovelManager.h"

class NarouNovelManager :public NovelManager {
public:
	virtual void UpdateNovel(const int& ChapterAmount)override;
	virtual void GetNovelInfo()override;
	virtual void UpdateShort()override;
};
