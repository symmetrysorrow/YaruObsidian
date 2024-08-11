#pragma once
#include <string>

struct BookshelfIndex {
    std::string NovelID;
    int ChapterAmount;
    std::string LastUpdatedDate;

    bool operator==(const BookshelfIndex& Other)const
    {
        return NovelID == Other.NovelID;
    }
    bool operator<(const BookshelfIndex& Other)const
    {
        return NovelID < Other.NovelID;
    }
};

enum NovelType {
    Narou,
    Hameln,
    Noc,
    Yaruo,
};
