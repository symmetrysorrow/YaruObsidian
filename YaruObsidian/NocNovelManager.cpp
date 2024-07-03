#pragma once
#include "NocNovelManager.h"
#include "BookshelfManager.h"

void NocNovelManager::UpdateNovel(const int& ChapterAmount)
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::filesystem::path path = "Novel/Noc/" + NovelTitle;
    std::filesystem::create_directories(path);

    cout << "Updating Novel : " + NovelTitle + "\n";
    BookshelfManagerPtr->UpdateMailText = BookshelfManagerPtr->UpdateMailText + NovelTitle+" / ";

    for (int i = 1; i <= ChapterAmount; i++)
    {
        std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + to_string(i) + "/"));
        std::string ChapterTitle, ChapterContents;

        regex pattern("<p class=\"novel_subtitle\">(.+?)</p>");
        smatch matches;
        // 正規表現を使用してテキストを抽出
        if (regex_search(ChapterHTML, matches, pattern)) {
            // マッチした部分を表示
            ChapterTitle= matches[1].str();
        }

        regex ContentPattern("<div id=\"novel_honbun\"[^>]*>(.*?)</div>", regex_constants::icase | regex_constants::ECMAScript);
        smatch ContentMatches;
        if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
            // マッチした部分を表示
            ChapterContents=ContentMatches[1].str();
        }

        CreateChapter(NovelTitle, ChapterTitle, ChapterContents, NovelType::Noc);
        PreChapterTitle = ChapterTitle;

        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        cout << "\rProgress: " << fixed << setprecision(2) << progress << "%";
        cout.flush();
    }
    cout << endl; // 改行して進捗表示をクリア

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

    regex DatePattern(R"(<th>最.*日</th><td>(.*?)</td>)", regex_constants::icase);
    smatch DateMatches;
    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {
        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
            regex pattern(R"(全(\d+)エピソード)");
            smatch matches;
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
        BookshelfManagerPtr->AppendToBookshelf("Noc.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        cout << "No Need To Update " + GetTitle(NovelURL) + "\n";
        return;
    }
    BookshelfManagerPtr->AppendToBookshelf("Noc.csv", NovelIndex.NovelID + ",1,1");
    cout << "Error. Novel Is not Found";
    return;
}

void NocNovelManager::UpdateShort()
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::string NovelPath = "Novel/Noc/" + NovelTitle + ".md";
    ofstream file(NovelPath, ios::out);

    cout << "Updating Novel : " + NovelTitle + "\n";

    if (!file) {
        cerr << "Failed to open file for writing.\n" << endl;
        return;
    }
    std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + "/"));

    regex ContentPattern("<div id=\"novel_honbun\"[^>]*>(.*?)</div>", regex_constants::icase | regex_constants::ECMAScript);
    smatch ContentMatches;
    if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
        // マッチした部分を表示
        file << ContentMatches[1].str() << "\n" << endl;
    }
    if (!regex_search(ChapterHTML, ContentMatches, ContentPattern))
    {
        cout << "Not Found";
    }
    cout << "100%\n";

    file.close();

    return;
}

std::string NocNovelManager::GetHTML(const std::string& URL)
{
    random_device rd;
    uniform_int_distribution<int> dist(1000, 2000);
    int random_ms = dist(rd);
    this_thread::sleep_for(chrono::milliseconds(random_ms));
    // Unicode文字列に変換
    wstring wideUrl;
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, nullptr, 0);
    if (bufferSize > 0) {
        wideUrl.resize(bufferSize - 1);
        MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, &wideUrl[0], bufferSize);
    }
    else {
        cerr << "Failed to convert URL to wide string" << endl;
        return "";
    }

    // WinINetの初期化
    HINTERNET hInternet = InternetOpen(L"MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        cerr << "Failed to initialize WinINet" << endl;
        return "";
    }

    SetCookie("https://novel18.syosetu.com", "over18=yes");


    // URLを開く
    HINTERNET hUrl = InternetOpenUrl(hInternet, wideUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL) {
        cerr << "Failed to open URL" << endl;
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