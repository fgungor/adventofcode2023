#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <string>
#include <chrono>
#include <unordered_map>

int main() {
    // Open the file
    std::ifstream file("../input_03.txt");

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

    const int rows = static_cast<int>(lines.size());
    const int cols = static_cast<int>(lines[0].length());


    auto isDigit = [](char ch) -> bool {
        return ch >= '0' && ch <= '9';
    };

    auto getChar = [&lines, &rows, &cols](int row, int col) -> char {
        if (row >= 0 && row < rows &&
            col >= 0 && col < cols)
            return lines[row][col];

        return 0;
    };


    auto getGearsAround = [&](int row, int col, int numLength, std::vector<int> &out) -> void {

        for (int i = row - 1; i < row + 2; ++i) {
            if (i < 0 || i >= rows)
                continue;

            for (int j = col - 1; j < col + numLength + 1; ++j) {
                char ch = getChar(i, j);
                const auto hash = i * cols + j;
                if (ch == '*' && std::find(out.begin(), out.end(), hash) == out.end()) {
                    out.push_back(hash);
                }
            }
        }
    };

    std::string currentNumber;
    std::unordered_map<int, std::vector<int>> gearNumbers;

    std::vector<int> gears;

    int currentNumCol = 0;
    int currentNumRow = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            const char cur = lines[i][j];
            if (isDigit(cur)) {
                if (currentNumber.empty()){
                    currentNumRow = i;
                    currentNumCol = j;
                }
                currentNumber.push_back(cur);
            }

            // evaluate current
            if (!isDigit(cur) || j == cols - 1) {
                if (!currentNumber.empty()) {
                    const auto num = std::stoi(currentNumber);
                    const auto len = static_cast<int>(currentNumber.length());

                    getGearsAround(currentNumRow, currentNumCol, len, gears);

                    for (const auto gearHash : gears) {
                        gearNumbers[gearHash].push_back(num);
                    }
                }

                currentNumber = "";
                gears.clear();
            }
        }
    }

    for (const auto &nums: gearNumbers) {
        if (nums.second.size() == 2) {
            sum += nums.second[0] * nums.second[1];
        }
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
