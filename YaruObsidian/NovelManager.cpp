#pragma once
#include "NovelManager.h"
#include "BookshelfManager.h"


#pragma comment(lib, "wininet.lib")

NovelManager::NovelManager()
{
    BookshelfManagerPtr = nullptr;
}

void NovelManager::ManageNovel(BookshelfIndex Index)
{
	NovelIndex = Index;
    GetNovelInfo();
}
#if 1
void NovelManager::CreateChapter(const std::string& NovelTitle, const std::string& ChapterTitle, const std::string& ChapterContents, NovelType novelSite)
{
    std::string ChapterPath, PreChapterPath, chapterTitle;

    ChapterTitle == "" ? chapterTitle = "1" : chapterTitle = ChapterTitle;

    switch (novelSite)
    {
    case Narou:
        ChapterPath = "Novel/Narou/" + NovelTitle + "/" + chapterTitle +".md";
        PreChapterPath= "Novel/Narou/" + NovelTitle + "/" + PreChapterTitle + ".md";
        break;
    case Hameln:
        ChapterPath = "Novel/Hameln/" + NovelTitle + "/" + chapterTitle + ".md";
        PreChapterPath = "Novel/Hameln/" + NovelTitle + "/" + PreChapterTitle + ".md";
        break;
    case Noc:
        ChapterPath = "Novel/Noc/" + NovelTitle + "/" + chapterTitle + ".md";
        PreChapterPath = "Novel/Noc/" + NovelTitle + "/" + PreChapterTitle + ".md";
        break;
    case Yaruo:
        ChapterPath = "Yaruo/" + NovelTitle + "/" + chapterTitle + ".md";
        PreChapterPath = "Yaruo/" + NovelTitle + "/" + PreChapterTitle + ".md";
        break;
    }

    if (PreChapterTitle!="")
    {
        ofstream file(ChapterPath, ios::out);
        file << ChapterContents << "\n\n" << " [[" << PreChapterTitle << "| < 前へ]]\n" << endl;
        file.close();

        std::ofstream outfile;
        outfile.open(PreChapterPath, std::ios::app);
        outfile << " [[" << chapterTitle << "| 次へ >]] \n" << endl;
        outfile.close();
    }
    else
    {
        ofstream file(ChapterPath, ios::out);
        file << ChapterContents << "\n\n" << endl;
        file.close();
    }
 
    

}
#endif
std::string NovelManager::GetHTML(const std::string& URL)
{
    random_device rd;
    uniform_int_distribution<int> dist(1000, 2000);
    int random_ms = dist(rd);
    this_thread::sleep_for(chrono::milliseconds(random_ms));

    // Unicode文字列に変換
    wstring wideUrl;
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, nullptr, 0);
    if (bufferSize > 0) 
    {
        wideUrl.resize(bufferSize - 1);
        MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, &wideUrl[0], bufferSize);
    }
    else 
    {
        cerr << "Failed to convert URL to wide string" << endl;
        return "";
    }
    // WinINetの初期化
    HINTERNET hInternet = InternetOpen(L"MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) 
    {
        cerr << "Failed to initialize WinINet" << endl;
        return "";
    }
    // URLを開く
    HINTERNET hUrl = InternetOpenUrl(hInternet, wideUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL) 
    {
        cerr << "Failed to open URL" << endl;
        InternetCloseHandle(hInternet);
        return "";
    }
    // HTMLコンテンツを取得
    string htmlContent;
    char buffer[1024];
    DWORD bytesRead = 0;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) 
    {
        htmlContent.append(buffer, bytesRead);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return htmlContent;
}

std::string NovelManager::GetTitle(std::string URL)
{
    string TargetHTML = GetHTML(URL);
    regex titlePattern(R"(<title>(.*?)<\/title>)");
    smatch matches;
    if (regex_search(TargetHTML, matches, titlePattern) && matches.size() > 1) 
    {
        return matches[1].str();
    }
    return "No title found";
}

std::string NovelManager::GetHTMLTitle(const std::string& HTML)
{
    regex titlePattern(R"(<title>(.*?)<\/title>)");
    smatch matches;
    if (regex_search(HTML, matches, titlePattern) && matches.size() > 1)
    {
        return matches[1].str();
    }
    return "No title found";
}

std::string NovelManager::RemoveNewLines(const std::string& str)
{
    std::string result = str;
    // \n と \r を削除する
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(remove(result.begin(), result.end(), '\r'), result.end());
    return result;
}

void NovelManager::SetCookie(const std::string& url,const std::string& cookie)
{
    // URLとCookieをワイド文字列に変換
    std::wstring wUrl(url.begin(), url.end());
    std::wstring wCookie(cookie.begin(), cookie.end());

    BOOL result = InternetSetCookieEx(wUrl.c_str(), NULL, wCookie.c_str(), INTERNET_COOKIE_THIRD_PARTY, 0);
    if (!result) 
    {
        std::cerr << "Failed to set cookie for URL: " << url << std::endl;
    }
}

std::string NovelManager::SanitizeFilename(const std::string& filename)
{
    std::string sanitized = filename;
    std::regex illegalChars(R"([\/:*?"<>|])");
    sanitized = std::regex_replace(sanitized, illegalChars, "_"); // 不正な文字をアンダースコアに置き換え
    return sanitized;
}

std::string NovelManager::removeSubstring(const std::string& str, const std::string& substring)
{
    std::string result = str;
    size_t pos;
    while ((pos = result.find(substring)) != std::string::npos) 
    {
        result.erase(pos, substring.length());
    }
    return result;
}

void NovelManager::UpdateNovel(const int& ChapterAmount)
{
}

void NovelManager::UpdateShort()
{
}

void NovelManager::GetNovelInfo()
{
}