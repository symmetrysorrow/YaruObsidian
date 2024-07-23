#pragma once
#include "NocNovelManager.h"
#include "BookshelfManager.h"

void NocNovelManager::UpdateNovel(const int& ChapterAmount)
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::filesystem::path path = "Novel/Noc/" + NovelTitle;
    std::filesystem::create_directories(path);

    std::cout << "Updating Novel : " + NovelTitle + "\n";
    BookshelfManagerPtr->UpdateMailText += (NovelTitle + "　");

    for (int i = 1; i <= ChapterAmount; i++)
    {
        std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + std::to_string(i) + "/"));
        std::string ChapterTitle, ChapterContents;

        std::regex pattern("<p class=\"novel_subtitle\">(.+?)</p>");
        std::smatch matches;
        // 正規表現を使用してテキストを抽出
        if (regex_search(ChapterHTML, matches, pattern)) {
            // マッチした部分を表示
            ChapterTitle= matches[1].str();
        }

        std::regex ContentPattern("<div id=\"novel_honbun\"[^>]*>(.*?)</div>", std::regex_constants::icase | std::regex_constants::ECMAScript);
        std::smatch ContentMatches;
        if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
            // マッチした部分を表示
            ChapterContents=ContentMatches[1].str();
        }

        CreateChapter(NovelTitle, ChapterTitle, ChapterContents, NovelType::Noc);
        PreChapterTitle = ChapterTitle;

        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        std::cout << "\rProgress: " << std::fixed << std::setprecision(2) << progress << "%";
        std::cout.flush();
    }
    std::cout << std::endl; // 改行して進捗表示をクリア

    return;
}

void NocNovelManager::GetNovelInfo()
{
    InfoURL = "https://novel18.syosetu.com/novelview/infotop/ncode/" + NovelIndex.NovelID + "/";
    NovelURL = "https://novel18.syosetu.com/" + NovelIndex.NovelID + "/";
    std::string NovelInfoHTML = RemoveNewLines(GetHTML(InfoURL));

    //  cout << GetTitle(NovelInfoHTML)<<"\n";
    // GetCookies("https://novel18.syosetu.com");
    // cout << NovelInfoHTML<<"\n";

    std::regex DatePattern(R"(<th>最.*日</th><td>(.*?)</td>)", std::regex_constants::icase);
    std::smatch DateMatches;
    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {
        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
	        std::regex pattern(R"(全(\d+)エピソード<a)");
	        std::smatch matches;
            if (regex_search(NovelInfoHTML, matches, pattern) && stoi(matches[1].str()) >= NovelIndex.ChapterAmount)
            {
                UpdateNovel(stoi(matches[1].str()));
                BookshelfManagerPtr->AppendToBookshelf("Noc.csv", NovelIndex.NovelID + "," + matches[1].str() + "," + DateMatches[1].str());
                return;
            }
            if (!regex_search(NovelInfoHTML, matches, pattern))
            {
	            UpdateShort();
	            return;
            }
        }
        BookshelfManagerPtr->AppendToBookshelf("Noc.csv", NovelIndex.NovelID + "," + std::to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        std::cout << "No Need To Update " + GetTitle(NovelURL) + "\n";
        return;
    }
    BookshelfManagerPtr->AppendToBookshelf("Noc.csv", NovelIndex.NovelID + ",1,1");
    std::cout << "Error. Novel Is not Found";
    return;
}

void NocNovelManager::UpdateShort()
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::string NovelPath = "Novel/Noc/" + NovelTitle + ".md";
    std::ofstream file(NovelPath, std::ios::out);

    std::cout << "Updating Novel : " + NovelTitle + "\n";

    if (!file) {
	    std::cerr << "Failed to open file for writing.\n" << std::endl;
        return;
    }
    std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL));

    std::regex ContentPattern("<div id=\"novel_honbun\" class=\"novel_view\">(.*?)</div>", std::regex_constants::icase | std::regex_constants::ECMAScript);
    std::smatch ContentMatches;
    if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
        // マッチした部分を表示
        file << ContentMatches[1].str() << "\n" << std::endl;
    }
    if (!regex_search(ChapterHTML, ContentMatches, ContentPattern))
    {
        std::cout << "Not Found";
    }
    std::cout << "100%\n";

    file.close();

    return;
}

std::string NocNovelManager::GetHTML(const std::string& URL)
{
	std::random_device rd;
    std::uniform_int_distribution<int> dist(1000, 2000);
    int random_ms = dist(rd);
	std::this_thread::sleep_for(std::chrono::milliseconds(random_ms));
    // Unicode文字列に変換
    std::wstring wideUrl;
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, nullptr, 0);
    if (bufferSize > 0) {
        wideUrl.resize(bufferSize - 1);
        MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, &wideUrl[0], bufferSize);
    }
    else {
        std::cerr << "Failed to convert URL to wide string" << std::endl;
        return "";
    }

    // WinINetの初期化
    HINTERNET hInternet = InternetOpen(L"MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        std::cerr << "Failed to initialize WinINet" << std::endl;
        return "";
    }

    SetCookie("https://novel18.syosetu.com", "over18=yes");


    // URLを開く
    HINTERNET hUrl = InternetOpenUrl(hInternet, wideUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL) {
	    std::cerr << "Failed to open URL" << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // HTMLコンテンツを取得
    std::string htmlContent;
    char buffer[1024];
    DWORD bytesRead = 0;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        htmlContent.append(buffer, bytesRead);
    }

    // ハンドルを閉じる
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return htmlContent;
}