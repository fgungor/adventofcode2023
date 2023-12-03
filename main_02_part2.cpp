#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string_view>
#include <string>
#include <stack>
#include <chrono>


struct Game {
    int number;

    struct Round {
        int red;
        int blue;
        int green;
    };

    std::vector<Round> rounds;
};

int main() {
    // Open the file
    std::ifstream file("../input_02.txt");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    // get lines
    std::vector<int> values;
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> tokens;
    std::vector<Game> games;
    int gameIndex = 1;
    for (const auto &line: lines) {
        tokens.clear();
        std::istringstream iss(line);
        std::string token;

        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        // parse game
        auto *game = &games.emplace_back();
        game->number = gameIndex++;

        // parse rounds
        auto *gameRound = &game->rounds.emplace_back();
        for (int i = 2; i < tokens.size(); ++i) {
            int count = std::atoi(tokens[i].c_str()); // yolo!
            ++i;
            switch (tokens[i][0]) {
                case 'r':
                    gameRound->red = count;
                    break;
                case 'g':
                    gameRound->green = count;
                    break;
                case 'b':
                    gameRound->blue = count;
                    break;
                default:
                    assert(false);
            }

            // new round
            if (tokens[i].back() == ';') {
                gameRound = &game->rounds.emplace_back();
            }
        }
    }

    // validate
    int sum = 0;
    int maxRed = 0;
    int maxGreen = 0;
    int maxBlue = 0;

    for (const auto &g: games) {
        maxRed = 0;
        maxGreen = 0;
        maxBlue = 0;

        for (const auto &r: g.rounds) {
            if (r.red > maxRed)
                maxRed = r.red;
            if (r.green > maxGreen)
                maxGreen = r.green;
            if (r.blue > maxBlue)
                maxBlue = r.blue;
        }

        sum += maxRed * maxGreen * maxBlue;
    }


    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Result: " << sum << std::endl;
    std::cout << "Duration:" << duration << std::endl;

    // Close the file (not strictly necessary as the destructor will do this)
    file.close();

    return 0;
}
