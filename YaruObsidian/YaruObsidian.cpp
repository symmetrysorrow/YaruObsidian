﻿#pragma once
#include "BookshelfManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
//#include <windows.h>
#include <future>

int main()
{
    BookshelfManager* manager = new BookshelfManager;
    manager->ManageBookshelf();
    std::cout << "Completed\n";
    delete manager;

    // Future object to wait for user input in another thread
    std::future<void> future = std::async(std::launch::async, []() {
        std::cin.get(); // Wait for Enter key
        });

    // Wait for either the input or the timeout
    if (future.wait_for(std::chrono::hours(1)) == std::future_status::ready) {
        std::cout << "Enter key pressed, executing YaruObsidian.exe\n";
    }
    else {
        std::cout << "1 hour passed, executing YaruObsidian.exe\n";
    }

    system("YaruObsidian.exe");
    return 0;
}
