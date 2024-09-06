#pragma once
#include <string>

class BookshelfManager;
class HamelnNovelManager;
class NarouNovelManager;
class NocNovelManager;
class YaruoManager;

class UserInterface
{
public:
	void ManageCommand(const std::string& command);
private:
	void AddNovelCmd();
	void DeleteNovelCmd();
	void UpdateNovelsCmd();
	void UpdateAllNovelsCmd();
	void MoveNovelsCmd();
	void OrganizeNovelsCmd();
	void StopUpdateCmd();

	BookshelfManager& BookMG;

	HamelnNovelManager& HamelnMG;
	NarouNovelManager& NarouMG;
	NocNovelManager& NocMG;
	YaruoManager& YaruoMG;
};

