#include "Game.h"      // Chỉ cần include Cỗ máy game
#include <iostream>    // Cho std::cerr
#include <stdexcept>   // Cho std::runtime_error
#include <ctime>       // Cho time()
#include <cstdlib>     // Cho srand()

int main()
{
    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(static_cast<unsigned>(time(0)));

    try
    {
        // 1. Tạo Cỗ máy game
        // (Hàm dựng Game::Game() sẽ chạy và tải mọi thứ)
        Game game;

        // 2. Chạy Cỗ máy game
        // (Vòng lặp while(isOpen) sẽ bắt đầu)
        game.run();
    }
    catch (const std::exception& e)
    {
        // Bắt lỗi nếu Game::loadTextures() bị "ném" ra
        std::cerr << "Da xay ra loi nghiem trong: " << e.what() << std::endl;
        return -1;
    }

    // Game kết thúc bình thường
    return 0;
}