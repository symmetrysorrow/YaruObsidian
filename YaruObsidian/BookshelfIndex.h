#pragma once
#include <string>

struct BookshelfIndex {
    std::string NovelID;
    int ChapterAmount;
    std::string LastUpdatedDate;
    std::string NovelTitle;
    std::string Directory;
    bool IsUpdateRequired;

    bool operator==(const BookshelfIndex& Other)const
    {
        return NovelID == Other.NovelID;
    }
    bool operator<(const BookshelfIndex& Other)const
    {
        return NovelID < Other.NovelID;
    }
};

enum NovelSite {
    Narou,
    Hameln,
    Noc,
    Yaruo,
};


