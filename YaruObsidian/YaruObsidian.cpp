#pragma once
#include "BookshelfManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
//#include <windows.h>
#include <future>

int main() {
    // Create and manage BookshelfManager
    {
        BookshelfManager manager;
        manager.ManageBookshelf();
        std::cout << "Completed\n";
    } // manager goes out of scope and is destructed here

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

    // Execute the external program
    system("YaruObsidian.exe");

    return 0;
}
