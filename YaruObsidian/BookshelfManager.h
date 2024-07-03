#pragma once
#include <string>
#include "BookshelfIndex.h"
#include <iostream>
#include <vector>

class BookshelfManager {
public:
    BookshelfManager() = default;
    void ManageBookshelf();
    void AppendToBookshelf(const std::string& filename, const std::string& data);
    std::string UpdateMailText="";
    std::string WarningMailText="";
 
private:

    std::vector<BookshelfIndex> NarouBookshelves;
    std::vector<BookshelfIndex> HamelnBookshelves;
    std::vector<BookshelfIndex> NocBookshelves;
    std::vector<BookshelfIndex> YaruoBookshelves;

    void UpdateNovels();

    void LoadBookshelf(std::string filename);

    void GetMailInfo();
    std::vector<std::string> get_mail_first_lines();
    void SendNotification();
    void SendWarning();
};