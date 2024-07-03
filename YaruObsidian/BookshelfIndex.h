#pragma once
#include <string>

struct BookshelfIndex {
    std::string NovelID;
    int ChapterAmount;
    std::string LastUpdatedDate;
};

enum NovelType {
    Narou,
    Hameln,
    Noc,
    Yaruo,
};
