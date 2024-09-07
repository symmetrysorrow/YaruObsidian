#pragma once
#include "NarouNovelManager.h"
#include "BookshelfManager.h"

void NarouNovelManager::UpdateNovel(const int& ChapterAmount)
{
    
    std::filesystem::path path = NovelIndex->Directory+"/" + NovelIndex->NovelTitle;
    std::filesystem::create_directories(path); 

    std::cout << "Updating Novel : " + NovelIndex->NovelTitle + "\n";

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
            ChapterContents = ContentMatches[1].str();
        }

        CreateChapter(ChapterTitle, ChapterContents, NovelSite::Narou);
        PreChapterTitle = ChapterTitle;
       
        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        std::cout << "\rProgress: " << std::fixed << std::setprecision(2) << progress << "%";
        std::cout.flush();
    }
    std::cout << std::endl; // 改行して進捗表示をクリア
    return;
}

void NarouNovelManager::UpdateShort()
{
    std::string NovelTitle = GetTitle(NovelURL);
    std::string NovelPath = "Novel/Narou/" + NovelTitle + ".md";
    std::ofstream file(NovelPath, std::ios::out);

    std::cout << "Updating Short : " + NovelTitle + "\n";

    if (!file) {
	    std::cerr << "Failed to open file for writing.\n" << std::endl;
        return;
    }

    std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + "/"));

    std::regex ContentPattern("<div id=\"novel_honbun\" class=\"novel_view\">(.*?)</div>", std::regex_constants::icase | std::regex_constants::ECMAScript);
    std::smatch ContentMatches;
    if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
        // マッチした部分を表示
        file << ContentMatches[1].str() << "\n" << std::endl;
    }
    if (!regex_search(ChapterHTML, ContentMatches, ContentPattern))
    {
        std::cout << "Not Found\n";
    }
    std::cout << "100%\n";

    file.close();

    return;
}



void NarouNovelManager::GetNovelInfo()
{
    InfoURL = "https://ncode.syosetu.com/novelview/infotop/ncode/" + NovelIndex->NovelID + "/";
    NovelURL = "https://ncode.syosetu.com/" + NovelIndex->NovelID + "/";
    std::string NovelInfoHTML = GetHTML(InfoURL);
    NovelIndex->NovelTitle = GetTitle(NovelURL);
    std::regex DatePattern(R"(<th>最.*掲載日</th>\s*<td>(.*?)<\/td>)", std::regex_constants::icase);
    std::smatch DateMatches;
    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {
        if (DateMatches[1].str() != NovelIndex->LastUpdatedDate)
        {
            NovelIndex->LastUpdatedDate = DateMatches[1].str();

            std::regex pattern(R"(</span>全(\d+)エピソード)");
            std::smatch matches;
            if (regex_search(NovelInfoHTML, matches, pattern) && stoi(matches[1].str()) >= NovelIndex->ChapterAmount)
            {
                UpdateNovel(stoi(matches[1].str()));
                NovelIndex->ChapterAmount = stoi(matches[1].str());
                return;
            }
            if (!regex_search(NovelInfoHTML, matches, pattern))
            {
                UpdateShort();
                return;
            }
        }
        return;
    }
    std::regex DatePattern_short(R"(掲載日)", std::regex_constants::icase);
    std::smatch Matches;
    if (regex_search(NovelInfoHTML, Matches, DatePattern_short))
    {
        UpdateShort();
        return;
    }
    std::cout << "Error. Novel Is not Found";
    return;
}
