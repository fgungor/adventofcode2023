#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string_view>
#include <string>
#include <stack>
#include <chrono>

// A DAWG structure to parse digits in word and number form
struct digitDAWG {

    struct node {
        char token = 0;
        int value = -1;
        std::vector<node> next;
    };

    node root;

    digitDAWG() {
        insert("zero", 0);
        insert("one", 1);
        insert("two", 2);
        insert("three", 3);
        insert("four", 4);
        insert("five", 5);
        insert("six", 6);
        insert("seven", 7);
        insert("eight", 8);
        insert("nine", 9);


        insert("0", 0);
        insert("1", 1);
        insert("2", 2);
        insert("3", 3);
        insert("4", 4);
        insert("5", 5);
        insert("6", 6);
        insert("7", 7);
        insert("8", 8);
        insert("9", 9);

    }

    void insert(std::string_view str, int value) {

        node *cur = &root;

        for (const char c: str) {
            auto it = std::find_if(cur->next.begin(), cur->next.end(),
                                   [&c](const auto &n) -> bool { return n.token == c; });

            if (it == cur->next.end()) {
                cur = &(cur->next.emplace_back(c, -1));
            } else { cur = &(*it); }
        }

        assert(cur->value < 0);
        cur->value = value;
    }

    int get(std::string_view source, int offset = 0) {
        node *cur = &root;

        auto str = source.substr(offset);
        for (const char c: str) {
            auto it = std::find_if(cur->next.begin(), cur->next.end(),
                                   [&c](const auto &n) -> bool { return n.token == c; });

            if (it == cur->next.end()) { return -1; }
            if (it->value >= 0) { return it->value; }
            cur = &(*it);
        }

        return -1;
    }

    template<typename Container>
    void getAll(std::string_view str, Container& container) {
        std::back_insert_iterator out(container);

        for (int i = 0; i < str.length(); ++i) {
            auto val = get(str, i);
            if (val > -1){
                (*out) = val;
                ++out;
            }
        }
    }
};

int main() {
    // Open the file
    std::ifstream file("../input_01.txt");

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

    digitDAWG yoDawg;

    auto start = std::chrono::high_resolution_clock::now();

    // do stuff
    int sum = 0;
    for (const auto& line : lines)
    {
        int firstNum = -1;
        int lastNum = -1;

        values.clear();
        yoDawg.getAll(line, values);
        firstNum = values.front();
        lastNum = values.back();

        assert(firstNum >= 0 && firstNum <= 9);
        assert(lastNum >= 0 && lastNum <= 9);
        sum += firstNum * 10 + lastNum;
    }

    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Result: " << sum << std::endl;
    std::cout << "Duration: " << duration << std::endl;

    // Close the file (not strictly necessary as the destructor will do this)
    file.close();

    return 0;
}
