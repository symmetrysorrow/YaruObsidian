#pragma once
#include "BookshelfManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>

int main()
{
    BookshelfManager* manager=new BookshelfManager;
    manager->ManageBookshelf();
    std::cout << "\nCompleted\n";
    delete manager;
    std::this_thread::sleep_for(std::chrono::minutes(60));
    system("YaruObsidian.exe");
    return 0;
}