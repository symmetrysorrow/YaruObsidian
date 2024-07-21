#pragma once
#include "BookshelfManager.h"
#include "HamelnNovelManager.h"

void HamelnNovelManager::UpdateNovel(const int& ChapterAmount)
{
    std::string NovelTitle=  removeSubstring(GetTitle(NovelURL), " - ハーメルン");
    std::filesystem::path path = "Novel/Hameln/" + NovelTitle;
    std::filesystem::create_directories(path);

    cout << "Updating Novel : " + GetTitle(NovelURL) + "\n";
    BookshelfManagerPtr->UpdateMailText = BookshelfManagerPtr->UpdateMailText +GetTitle(NovelURL)+" / ";

    for (int i = 1; i <= ChapterAmount; i++)
    {        
        std::string ChapterHTML = RemoveNewLines(GetHTML(NovelURL + to_string(i) + ".html"));
        std::string ChapterTitle, ChapterContents;

        regex pattern(R"(</div><span style=[^>]*>(.*?)</span><span id=\"analytics_start\">)", regex_constants::icase);
        smatch matches;
        if (regex_search(ChapterHTML, matches, pattern)) {
           ChapterTitle = RemoveLeadingSpace(matches[1].str());
        }

        regex ContentPattern("<div id=\"honbun\"[^>]*>(.*?)</div>", regex_constants::icase | regex_constants::ECMAScript);
        smatch ContentMatches;
        if (regex_search(ChapterHTML, ContentMatches, ContentPattern)) {
            ChapterContents = ContentMatches[1].str();
        }

        std::string NovelPath = "Novel/Hameln/" + NovelTitle + "/" + ChapterTitle  + ".md";
        ofstream file(NovelPath, ios::out);
        file << ChapterContents << "\n";
        file.close();

        CreateChapter(NovelTitle, ChapterTitle, ChapterContents, NovelType::Hameln);
        PreChapterTitle = ChapterTitle;

        //進捗率の計算と表示
        double progress = i == ChapterAmount ? 100 : static_cast<double>(i) / ChapterAmount * 100.0;
        cout << "\rProgress: " << fixed << setprecision(2) << progress << "%";
        cout.flush();    
    }
    cout << endl; // 改行して進捗表示をクリア
    return;
}

void HamelnNovelManager::GetNovelInfo()
{
    InfoURL = "https://syosetu.org/?mode=ss_detail&nid=" + NovelIndex.NovelID;
    NovelURL = "https://syosetu.org/novel/" + NovelIndex.NovelID + "/";
    std::string NovelInfoHTML = RemoveNewLines(GetHTML(InfoURL));

    regex DatePattern(R"(最新投稿</td><td>(.*?)</td>)", regex_constants::icase);
    smatch DateMatches;

    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {

        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
            regex pattern(R"(話数</td><td width=[^>]*>[^\s]*\s(.*?)話)");
            smatch matches;
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
        BookshelfManagerPtr->AppendToBookshelf("Hameln.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        cout << "No Need To Update " + GetTitle(NovelURL) + "\n";
        return;
    }
    cout << "Error. Novel Is not Found";
    BookshelfManagerPtr->AppendToBookshelf("Hameln.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + NovelIndex.LastUpdatedDate);
    return;
}

void HamelnNovelManager::UpdateShort()
{

}