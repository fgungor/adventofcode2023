#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <string>
#include <chrono>
#include <cassert>
#include <stack>

int main() {
    // Open the file
    std::ifstream file("../input_04.txt");

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

    int sum = 0;
    auto start = std::chrono::high_resolution_clock::now();



    auto isDigit = [](char ch) -> bool {
        return ch >= '0' && ch <= '9';
    };

    std::string strNumber;
    int countParsed = 0;
    int countMatch = 0;

    constexpr int numWinning = 10;
    constexpr int numPlayed = 25;

    std::vector<int> winning;

    std::vector<int> cardMatches;
    std::vector<int> cardCopies;

    for (const auto& line : lines) {

        auto card = line.substr(9);
        card.push_back(' '); // hackish but whatever...
        winning.clear();
        countParsed = 0;
        countMatch = 0;

        for (const auto ch : card) {
            if (isDigit(ch)){
                strNumber.push_back(ch);
            }
            else {
                if (!strNumber.empty()) {
                    const auto num = std::stoi(strNumber);

                    if (countParsed < numWinning) {
                        winning.push_back(num);
                    } else {
                        if (std::find(winning.begin(), winning.end(), num) != winning.end()) {
                            ++countMatch;
                        }
                    }

                    ++countParsed;
                }

                strNumber.clear();
            }
        }

        cardMatches.push_back(countMatch);
        cardCopies.push_back(1);

        assert(countParsed == numWinning + numPlayed);
    }

    for (int i = 0; i < cardCopies.size(); ++i) {
        const auto matches = cardMatches[i];
        for (int m = 1; m < matches + 1; ++m) {
                cardCopies[m + i] += cardCopies[i];
        }

        sum += cardCopies[i];
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
