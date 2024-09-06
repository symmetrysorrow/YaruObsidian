#pragma once
#include "BookshelfManager.h"
#include "UserInterface.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
//#include <windows.h>
#include <future>

int main() {
    UserInterface UI;

    std::atomic<bool> inputReceived(false); // 入力受信フラグ
    std::mutex mtx; // メモリ保護用

    while (true) {
        inputReceived = false; // 入力受信フラグをリセット

        // タイマー用スレッドを作成
        std::thread timerThread([&]() {
            std::this_thread::sleep_for(std::chrono::hours(1));
            if (!inputReceived) {
                std::cout << "1 hour passed" << std::endl;
                inputReceived = true; // タイマーで1時間経過とする
            }
            });

        // 入力待ち用スレッドを作成
        std::thread inputThread([&]() {
            std::string input;
            std::getline(std::cin, input);
            if (!input.empty()) {
                inputReceived = true; // 入力があったことを記録
                UI.ManageCommand(input); // コマンドを処理
            }
            });

        // スレッドが終了するのを待つ
        inputThread.join();
        timerThread.join();

        if (inputReceived) {
            // 入力があったか、1時間経過のいずれかの条件でループを再開
            continue; // 次のループを開始
        }

        // 追加処理が必要な場合はここに記述
        // 例: 追加の処理がある場合は次の1時間のタイマー待ちへ
    }

    return 0;
}
