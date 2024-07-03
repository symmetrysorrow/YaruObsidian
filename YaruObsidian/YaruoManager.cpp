#pragma once
#include "BookshelfManager.h"
#include "YaruoManager.h"

std::string YaruoManager::CorrectForYaruyomi(const std::string& HTML)
{
    std::string result;
    std::regex pattern(R"(<p id="res\d+">.*?<\/p>)");
    std::smatch matches;

    std::string::const_iterator searchStart(HTML.cbegin());
    while (std::regex_search(searchStart, HTML.cend(), matches, pattern)) {
        result += matches[0];
        searchStart = matches.suffix().first;
    }

    return result;
}

std::string YaruoManager::CorrectForRusalka(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<Div Align="right">.*?</Div>)"),
        std::regex(R"(<hr size="1" />)"),
        std::regex(R"(<hr noshade size="1">)"),
        std::regex(R"(<dl class="relate_dl fc2relate_entry_thumbnail_off">.*?<\/dl>)"),
        std::regex(R"(<div class="bottom_navi">.*?<\/div>)")
    };

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    return result;
}

std::string YaruoManager::CorrectForMex(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<div class="fc2_footer".*?</script>)"),
        std::regex(R"(<hr noshade size="1">)"),
    };

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    return result;
}

std::string YaruoManager::CorrectForEro(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<hr size="1">)"),
    };

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    return result;
}

std::string YaruoManager::CorrectForIitokolo(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<dl class="relate_dl.*?</dl>)"),
        std::regex(R"(<div class="buttom_navi".*?</div>)"),
    };

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    return result;
}

std::string YaruoManager::CorrectForBura(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<hr size="1" />)"),
        std::regex(R"(<table.*?</table>)"),
    };

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    result = "<span class=\"aa\">\n"+result;
    return result;
}

std::string YaruoManager::CorrectForOyogu(const std::string& HTML)
{
    std::string result = HTML;

    // 定義するタグの正規表現パターン
    std::regex patterns[] = {
        std::regex(R"(<div class="fc2_footer.*</div>)"),
        std::regex(R"(<dl class="relate.*</dl>)"),
        std::regex(R"(<div class="textar.*<!--▲ページナビゲーション▲-->)"),
        std::regex(R"(<table.*?</table>)"),
    }; 

    // 各パターンで文字列から一致する部分を削除
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern, "");
    }
    result = "<span class=\"aa\">\n" + result;
    return result;
}

std::string YaruoManager::removeBracketsContent(const std::string& input)
{
    std::regex bracketPattern("【[^【】]*】");
    return std::regex_replace(input, bracketPattern, "");
}

std::string YaruoManager::RemoveLeadingSpace(const std::string& input)
{
    const std::string fullWidthSpace = "\u3000";
    const std::string halfWidthSpace = " ";

    std::string result = input;

    // 文字列が全角スペースで始まるかチェック
    if (result.compare(0, fullWidthSpace.length(), fullWidthSpace) == 0) {
        // 全角スペースを削除した新しい文字列に更新
        result = result.substr(fullWidthSpace.length());
    }

    // 文字列が半角スペースで始まるかチェック
    if (result.compare(0, halfWidthSpace.length(), halfWidthSpace) == 0) {
        // 半角スペースを削除した新しい文字列に更新
        result = result.substr(halfWidthSpace.length());
    }

    // スペースを削除した結果を返す
    return result;
}


void YaruoManager::UpdateNovel(const int& CharacterAmount)
{
    std::vector<std::string> LinkList;

    string NovelTitle = removeSubstring(GetHTMLTitle(NovelInfoHTML), " | やる夫RSS+インデックス");
    NovelTitle = removeBracketsContent(NovelTitle);
    std::filesystem::path path = "Yaruo/" + NovelTitle;
    std::filesystem::create_directories(path);

    regex PagePattern(R"(<div class="paginate_link">(.*?)</div>)", regex_constants::icase);
    smatch PageMatches;
    if (regex_search(NovelInfoHTML, PageMatches, PagePattern))
    {
        regex urlPattern(R"(<a href=\"(.*?)\">)", regex_constants::icase);
        // 正規表現による検索
        std::string LinkHTML = PageMatches[1].str();
        auto it = std::sregex_iterator(LinkHTML.begin(), LinkHTML.end(), urlPattern);
        auto end = std::sregex_iterator();
        // マッチ結果をvectorに格納
        for (; it != end; ++it) {
            std::smatch match = *it;
            std::string PageContents = GetHTML("https://rss.r401.net" + match[1].str());
            regex LinkPattern(R"(<a target="_blank" rel="nofollow" class="exlink" href="(.*?)\">)", regex_constants::icase);
            auto it1 = std::sregex_iterator(PageContents.begin(), PageContents.end(), LinkPattern);
            auto end1 = std::sregex_iterator();

            for (; it1 != end1; ++it1)
            {
                std::smatch Match = *it1;
                LinkList.push_back(Match[1].str());
            }
        }

        ChapterAm = LinkList.size();        

        if (ChapterAm < NovelIndex.ChapterAmount)
        {
            cout << ChapterAm <<"<<"<<NovelIndex.ChapterAmount;
            cout << "\nWarning : Chapter is decreased\n";
            return;
        }

        std::string Target = LinkList[1];
        int i = 1;
        cout << "Updating Novel : " + NovelTitle + "\n";
        BookshelfManagerPtr->UpdateMailText = BookshelfManagerPtr->UpdateMailText+SanitizeFilename(NovelTitle)+" / ";

        for (auto& Link : std::vector<std::string>(LinkList.rbegin(), LinkList.rend()))
        {
            std::string ChapterHTML = RemoveNewLines(GetHTML(Link));
            std::string ChapterTitle, ChapterContents, RemovedSiteTitle;

            regex ChapterContentsPattern;
            smatch ChapterContentsMatch;

            if (Target.find("yaruomatomex") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<div class="article">(.*?)</div>)", regex_constants::icase);
                RemovedSiteTitle = "やる夫まとめくす ";
            }
            if (Target.find("rusalka") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(</a></h3>(.*?)<!--/article)", regex_constants::icase);
                RemovedSiteTitle = "100%自分用やる夫まとめ ";
            }
            if (Target.find("yaruobook") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<dl>(.*?)</dl>)", regex_constants::icase);
                RemovedSiteTitle = "やる夫スレ本棚 _ ";
            }
            if (Target.find("himanatokiniyaruo") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<div class="entry-body"><dl>(.*?)</dl>)", regex_constants::icase);
                RemovedSiteTitle = " - 暇な時にやる夫まとめ";
            }
            if (Target.find("yaruyomi") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<article class="wrapper aa">(.*?)</article>)", regex_constants::icase);
                RemovedSiteTitle = " - Yaruyomi";
            }
            if (Target.find("iitokolo") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<div class="article">(.*?)</div>)", regex_constants::icase);
                RemovedSiteTitle = "面白かったやる夫スレをまとめるサイト ";
            }
            if (Target.find("burakio") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<div class="article"><span class="aa">(.*?)<div class="fc2_footer")", regex_constants::icase);
                RemovedSiteTitle = "やる夫の暇つぶし麻亜屈（まーくつー） ";
            }
            if (Target.find("oyogu") != std::string::npos)
            {
                ChapterContentsPattern = std::regex(R"(<div class="article">(.*?)</div>)", regex_constants::icase);
                RemovedSiteTitle = "泳ぐやる夫シアター ";
            }

            if (regex_search(ChapterHTML, ChapterContentsMatch, ChapterContentsPattern))
            {
                ChapterContents = ChapterContentsMatch[1].str();
            }

            if (Target.find("yaruyomi") != std::string::npos)
            {
                ChapterContents = CorrectForYaruyomi(ChapterContents);
            }
            if (Target.find("rusalka") != std::string::npos)
            {
                ChapterContents = CorrectForRusalka(ChapterContents);
            }
            if (Target.find("yaruomatomex") != std::string::npos)
            {
                ChapterContents = CorrectForMex(ChapterContents);
            }
            if (Target.find("yaruobook") != std::string::npos)
            {
                ChapterContents = CorrectForEro(ChapterContents);
            }
            if (Target.find("iitokolo") != std::string::npos)
            {
                ChapterContents = CorrectForIitokolo(ChapterContents);
            }
            if (Target.find("burakio") != std::string::npos)
            {
                ChapterContents = CorrectForBura(ChapterContents);
            }
            if (Target.find("oyogu") != std::string::npos)
            {
                ChapterContents = CorrectForOyogu(ChapterContents);
            }
            
            ChapterTitle = removeSubstring(SanitizeFilename(GetHTMLTitle(ChapterHTML)),RemovedSiteTitle);
            ChapterTitle = RemoveLeadingSpace( removeBracketsContent( removeSubstring(ChapterTitle, NovelTitle)));
            CreateChapter(NovelTitle, ChapterTitle, ChapterContents, NovelType::Yaruo);
            PreChapterTitle = ChapterTitle;

            //進捗率の計算と表示
            double progress = i == LinkList.size() ? 100 : static_cast<double>(i) / LinkList.size() * 100.0;
            cout << "\rProgress: " << fixed << setprecision(2) << progress << "%";
            i++;
            cout.flush();
        }
        cout << "\n";
    }
}

void YaruoManager::GetNovelInfo()
{
    InfoURL = "https://rss.r401.net/yaruo/categories/" + NovelIndex.NovelID;

    NovelInfoHTML = RemoveNewLines(GetHTML(InfoURL));

    regex DatePattern(R"(<div class="item-date">(.*?)<a href=)", regex_constants::icase);
    smatch DateMatches;

    if (regex_search(NovelInfoHTML, DateMatches, DatePattern))
    {
        if (DateMatches[1].str() != NovelIndex.LastUpdatedDate)
        {
            UpdateNovel(NovelIndex.ChapterAmount);
            BookshelfManagerPtr->AppendToBookshelf("Yaruo.csv", NovelIndex.NovelID + "," + to_string(ChapterAm) + "," + DateMatches[1].str());
            return;
        }
        BookshelfManagerPtr->AppendToBookshelf("Yaruo.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + DateMatches[1].str());
        cout << "No Need To Update " + GetTitle(InfoURL) + "\n";
        return;
    }
    cout << "Error. Novel Is not Found\n";
    BookshelfManagerPtr->AppendToBookshelf("Yaruo.csv", NovelIndex.NovelID + "," + to_string(NovelIndex.ChapterAmount) + "," + NovelIndex.LastUpdatedDate);
    return;
}

