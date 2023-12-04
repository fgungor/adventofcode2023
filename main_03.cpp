#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <string>
#include <chrono>


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

    const auto rows = lines.size();
    const auto cols = lines[0].length();


    auto isDigit = [](char ch) -> bool {
        return ch >= '0' && ch <= '9';
    };

    auto isSymbol = [&isDigit](char ch) -> bool {
        return ch != 0 && ch != '.' && !isDigit(ch);
    };

    auto getChar = [&lines, &rows, &cols](int row, int col) -> char {
        if (row >= 0 && row < rows &&
            col >= 0 && col < cols)
            return lines[row][col];

        return 0;
    };

    auto hasSymbolAround = [&](int row, int col) -> bool {

        for (int i = row - 1; i < row + 2; ++i) {
            for (int j = col - 1; j < col + 2; ++j) {
                if (col == j && row == i)
                    continue;
                char ch = getChar(i, j);
                if (isSymbol(ch))
                    return true;
            }
        }

        return false;
    };

    std::string currentNumber;
    bool valid = false;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            const char cur = lines[i][j];
            if (isDigit(cur)) {
                currentNumber.push_back(cur);

                if (!valid)
                    valid = hasSymbolAround(i, j);
            } else {
                if (valid && !currentNumber.empty()) {

                    sum += std::stoi(currentNumber);
                }
                currentNumber = "";
                valid = false;
            }
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
