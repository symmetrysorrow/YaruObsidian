#pragma once
#include "NovelManager.h"
#include "BookshelfManager.h"
#include <unordered_map>

#pragma comment(lib, "wininet.lib")

void NovelManager::ManageNovel(BookshelfIndex const Index)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::ofstream outfile;
	outfile.open(LastChapterPath, std::ios::app);
	outfile <<  "\n";
	outfile.close();
}
#if 1
void NovelManager::CreateChapter(const std::string& ChapterTitle, const std::string& ChapterContents, NovelSite novelSite) 
{
	std::string ChapterPath, PreChapterPath, chapterTitle,preChapterTitle;

	ChapterTitle == "" ? chapterTitle = "1" : chapterTitle = RemoveLeadingSpace(ChapterTitle) ;
	preChapterTitle = RemoveLeadingSpace(PreChapterTitle);

	ChapterPath = NovelIndex->Directory + "/" + NovelIndex->NovelTitle+"/"+ chapterTitle + ".md";
	PreChapterPath= NovelIndex->Directory + "/" + NovelIndex->NovelTitle + "/" + preChapterTitle + ".md";

	LastChapterPath = ChapterPath;

	if (PreChapterTitle!="")
	{

		std::ofstream outfile;
		outfile.open(PreChapterPath, std::ios::app);
		outfile << " [[" << chapterTitle << "| 次へ >]] \n" << std::endl;
		outfile.close();

	   // std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::ofstream file(ChapterPath, std::ios::out);

		file << ChapterContents << "\n\n" << "<br></br>\n   [[" << preChapterTitle << "| < 前へ]]\n" << std::endl;
		file.close();
	}
	else
	{
		std::ofstream file(ChapterPath, std::ios::out);
		file << ChapterContents << "\n\n" << std::endl;
		file.close();
	}
	
}
#endif
std::string NovelManager::GetHTML(const std::string& URL)
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(1000, 2000);
	int random_ms = dist(rd);
	std::this_thread::sleep_for(std::chrono::milliseconds(random_ms));

	// Unicode文字列に変換
	std::wstring wideUrl;
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, nullptr, 0);
	if (bufferSize > 0) 
	{
		wideUrl.resize(bufferSize - 1);
		MultiByteToWideChar(CP_UTF8, 0, URL.c_str(), -1, &wideUrl[0], bufferSize);
	}
	else 
	{
		std::cerr << "Failed to convert URL to wide string" << std::endl;
		return "";
	}
	// WinINetの初期化
	HINTERNET hInternet = InternetOpen(L"MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) 
	{
		std::cerr << "Failed to initialize WinINet" << std::endl;
		return "";
	}
	SetCookie("https://novel18.syosetu.com", "over18=yes");
	SetCookie("https://syosetu.org", "over18=off");
	// URLを開く
	HINTERNET hUrl = InternetOpenUrl(hInternet, wideUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hUrl == NULL) 
	{
		std::cerr << "Failed to open URL" << std::endl;
		InternetCloseHandle(hInternet);
		return "";
	}
	// HTMLコンテンツを取得
	std::string htmlContent;
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
	std::string TargetHTML = GetHTML(URL);
	std::regex titlePattern(R"(<title>(.*?)<\/title>)");
	std::smatch matches;
	if (regex_search(TargetHTML, matches, titlePattern) && matches.size() > 1) 
	{
		return matches[1].str();
	}
	return "No title found";
}

std::string NovelManager::GetHTMLTitle(const std::string& HTML)
{
	std::regex titlePattern(R"(<title>(.*?)<\/title>)");
	std::smatch matches;
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

std::string NovelManager::RemoveLeadingSpace(const std::string& input)
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

	// 文字列が全角スペースで終わるかチェック
	if (result.length() >= fullWidthSpace.length() &&
		result.compare(result.length() - fullWidthSpace.length(), fullWidthSpace.length(), fullWidthSpace) == 0) {
		// 全角スペースを削除した新しい文字列に更新
		result = result.substr(0, result.length() - fullWidthSpace.length());
	}

	// 文字列が半角スペースで終わるかチェック
	if (result.length() >= halfWidthSpace.length() &&
		result.compare(result.length() - halfWidthSpace.length(), halfWidthSpace.length(), halfWidthSpace) == 0) {
		// 半角スペースを削除した新しい文字列に更新
		result = result.substr(0, result.length() - halfWidthSpace.length());
	}

	// スペースを削除した結果を返す
	return result;
}

std::string NovelManager::decode_html_entities(const std::string& input)
{
	std::unordered_map<std::string, std::string> html_entities = {
	  {"&hellip;", "…"},
	  {"&#8230;", "..."}
	  // 必要に応じて他のエンティティを追加
	};

	std::string output;
	std::string::size_type pos = 0, prev_pos = 0;

	while ((pos = input.find('&', pos)) != std::string::npos) {
		output.append(input, prev_pos, pos - prev_pos);
		auto end_pos = input.find(';', pos);
		if (end_pos != std::string::npos) {
			std::string entity = input.substr(pos, end_pos - pos + 1);
			auto it = html_entities.find(entity);
			if (it != html_entities.end()) {
				output.append(it->second);
			}
			else {
				output.append(entity); // エンティティが見つからない場合はそのまま
			}
			pos = end_pos + 1;
		}
		else {
			output.append("&"); // エンティティの終わりが見つからない場合はそのまま
			++pos;
		}
		prev_pos = pos;
	}

	output.append(input, prev_pos, input.length() - prev_pos);
	return output;
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
