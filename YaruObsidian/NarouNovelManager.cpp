#pragma once
#include "NarouNovelManager.h"
#include "BookshelfManager.h"

void NarouNovelManager::UpdateNovel(const int& ChapterAmount)
{
    std::string NovelTitle =GetTitle(NovelURL);
    std::filesystem::path path = "Novel/Narou/" + NovelTitle;
    std::filesystem::create_directories(path); 

    cout << "Updating Novel : " + NovelTitle + "\n";
    BookshelfManagerPtr->UpdateMailText += (NovelTitle + "　");

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
            ChapterContents = ContentMatches[1].str();
        }

        CreateChapter(NovelTitle, ChapterTitle, ChapterContents, NovelType::Narou);
        PreChapterTitle = ChapterTitle;
       
        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        cout << "\rProgress: " << fixed << setprecision(2) << progress << "%";
        cout.flush();
    }
    cout << endl; // 改行して進捗表示をクリア
    return;
}

void NarouNovelManager::UpdateShort()
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::string NovelPath = "Novel/Narou/" + NovelTitle + ".md";
    ofstream file(NovelPath, ios::out);

    cout << "Updating Novel : " + NovelTitle + "\n";

    if (!file) {
        cerr << "Failed to open file for writing.\n" << endl;
        return;
    }

    std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + "/"));

    regex ContentPattern("<div id=\"novel_honbun\" class=\"novel_view\">(.*?)</div>", regex_constants::icase | regex_constants::ECMAScript);
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

void NarouNovelManager::GetNovelInfo()
{
    InfoURL = "https://ncode.syosetu.com/novelview/infotop/ncode/" + NovelIndex.NovelID + "/";
    NovelURL = "https://ncode.syosetu.com/" + NovelIndex.NovelID + "/";
    std::string NovelInfoHTML = GetHTML(InfoURL);

    regex DatePattern(R"(<th>最.*掲載日</th>\s*<td>(.*?)<\/td>)", regex_constants::icase);
    smatch DateMatches;
    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {
        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
            regex pattern(R"(全(\d+)エピソード<a)");
            smatch matches;
            if (regex_search(NovelInfoHTML, matches, pattern) && stoi(matches[1].str()) >= NovelIndex.ChapterAmount)
            {
                UpdateNovel(stoi(matches[1].str()));
                BookshelfManagerPtr->AppendToBookshelf("Narou.csv", NovelIndex.NovelID + "," + matches[1].str() + "," + DateMatches[1].str());
                return;
            }
            if (!regex_search(NovelInfoHTML, matches, pattern))
            {
                UpdateShort();
                return;
            }
        }
        BookshelfManagerPtr->AppendToBookshelf("Narou.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        cout << "No Need To Update " + GetTitle(NovelURL) + "\n";
        return;
    }
    BookshelfManagerPtr->AppendToBookshelf("Narou.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + NovelIndex.LastUpdatedDate);
    cout << "Error. Novel Is not Found";
    return;
}
