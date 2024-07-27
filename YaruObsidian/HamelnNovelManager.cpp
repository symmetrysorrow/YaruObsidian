#pragma once
#include "BookshelfManager.h"
#include "HamelnNovelManager.h"

void HamelnNovelManager::UpdateNovel(const int& ChapterAmount)
{
    std::filesystem::path path = "Novel/Hameln/" + NovelTitle;
    std::filesystem::create_directories(path);

    std::cout << "Updating Novel : " + NovelTitle + "\n";
    BookshelfManagerPtr->UpdateMailText += (NovelTitle + "　");

    for (int i = 1; i <= ChapterAmount; i++)
    {        
        std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + std::to_string(i) + ".html"));
        std::string ChapterTitle, ChapterContents;

        std::regex pattern(R"(</div><span style=[^>]*>(.*?)</span><span id=\"analytics_start\">)", std::regex_constants::icase);
        std::smatch matches;
        if (regex_search(ChapterHTML, matches, pattern)) {
           ChapterTitle = RemoveLeadingSpace(matches[1].str());
            //ChapterTitle = matches[1].str();
        }

        std::regex ContentPattern("<div id=\"honbun\"[^>]*>(.*?)</div>", std::regex_constants::icase | std::regex_constants::ECMAScript);
        std::smatch ContentMatches;
        if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
            ChapterContents = ContentMatches[1].str();
        }

        std::string NovelPath = "Novel/Hameln/" + NovelTitle + "/" + ChapterTitle  + ".md";
        std::ofstream file(NovelPath, std::ios::out);
        file << ChapterContents << "\n";
        file.close();

        CreateChapter(ChapterTitle, ChapterContents, NovelType::Hameln);
        PreChapterTitle = ChapterTitle;

        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        std::cout << "\rProgress: " << std::fixed << std::setprecision(2) << progress << "%";
        std::cout.flush();    
    }
    std::cout << std::endl; // 改行して進捗表示をクリア
    return;
}

void HamelnNovelManager::GetNovelInfo()
{
    InfoURL = "https://syosetu.org/?mode=ss_detail&nid=" + NovelIndex.NovelID;
    NovelURL = "https://syosetu.org/novel/" + NovelIndex.NovelID + "/";
    std::string NovelInfoHTML = RemoveNewLines(GetHTML(InfoURL));

    NovelTitle = removeSubstring(GetTitle(NovelURL), " - ハーメルン");

    std::regex DatePattern(R"(最新投稿</td><td>(.*?)</td>)", std::regex_constants::icase);
    std::smatch DateMatches;

    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {

        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
            std::regex pattern(R"(話数</td><td width=[^>]*>[^\s]*\s(.*?)話)");
            std::smatch matches;
            if (regex_search(NovelInfoHTML, matches, pattern) && stoi(matches[1].str()) >= NovelIndex.ChapterAmount)
            {
                // cout << matches[1].str()<<"\n";
                UpdateNovel(stoi(matches[1].str()));
                BookshelfManagerPtr->AppendToBookshelf("Hameln.csv", NovelIndex.NovelID + "," + matches[1].str() + "," + DateMatches[1].str());
                return;
            }
            if (!regex_search(NovelInfoHTML, matches, pattern))
            {
                UpdateShort();
                return;
            }
        }
        BookshelfManagerPtr->AppendToBookshelf("Hameln.csv", NovelIndex.NovelID + "," + std::to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        std::cout << "No Need To Update " + NovelTitle + "\n";
        return;
    }
    std::cout << "Error. Novel Is not Found";
    BookshelfManagerPtr->AppendToBookshelf("Hameln.csv", NovelIndex.NovelID + "," + std::to_string(NovelIndex.ChapterAmount) + "," + NovelIndex.LastUpdatedDate);
    return;
}

void HamelnNovelManager::UpdateShort()
{

}