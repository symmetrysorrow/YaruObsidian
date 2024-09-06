#pragma once
#include <string>
#include <vector>
#include "BookshelfIndex.h"
#include <map>

class BookshelfManager;
class HamelnNovelManager;
class NarouNovelManager;
class NocNovelManager;
class YaruoManager;

enum NovelSite;
struct BookshelfIndex;

class BookshelfManager {
public:
    BookshelfManager() = default;
    void AddNovel(const std::string& NovelURL);
    void DeleteNovel(const NovelSite& Site, std::string& NovelID);
    void UpdateNovel(const NovelSite& Site, std::string& NovelID);
    void MoveNovel(const NovelSite& Site, std::string& NovelID);
    void ManageBookshelf();
    void OrganizeNovel(const NovelSite& Site, std::string& NovelID);
    void AppendToBookshelf(const std::string& filename, const std::string& data);
    std::string UpdateMailText="";
    std::string WarningMailText="";

    HamelnNovelManager& HamelnMG;
    NarouNovelManager& NarouMG;
    NocNovelManager& NocMG;
    YaruoManager& YaruoMG;
 
private:

    std::vector<BookshelfIndex> NarouBookshelves;
    std::vector<BookshelfIndex> HamelnBookshelves;
    std::vector<BookshelfIndex> NocBookshelves;
    std::vector<BookshelfIndex> YaruoBookshelves;

    std::map<std::string, BookshelfIndex> NarouBooks;
    std::map<std::string, BookshelfIndex> HamelnBooks;
    std::map<std::string, BookshelfIndex> NocBooks;
    std::map<std::string, BookshelfIndex> YaruoBooks;

    void UpdateNovels();
    std::string GetCSVFile(const NovelSite& Site);
    void SaveBookshelves(const NovelSite& Site);
    void LoadBookshelf(const NovelSite& Site);
    bool stringToBool(const std::string& str);
    void GetMailInfo();
    std::vector<std::string> get_mail_first_NovelURLs();
    void SendNotification();
    void SendWarning();
};