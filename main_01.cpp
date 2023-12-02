#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

int main() {
    // Open the file
    std::ifstream file("../input_01.txt");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string line;

    // Read the file line by line
    int sum = 0;

    while (std::getline(file, line)) {
        int firstNum = -1;
        int lastNum = -1;

        // Process each line as needed
        for (auto c: line) {
            if (c >= '0' && c <= '9') {
                if (firstNum < 0) {
                    firstNum = c - 48;
                    lastNum = firstNum;
                } else {
                    lastNum = c - 48;
                }
            }
        }

        assert(firstNum >= 0 && firstNum <= 9);
        assert(lastNum >= 0 && lastNum <= 9);
        sum += firstNum * 10 + lastNum;
    }

    std::cout << sum << std::endl;

    // Close the file (not strictly necessary as the destructor will do this)
    file.close();

    return 0;
}
