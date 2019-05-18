// Copyright 2018 Kirill Petrov

#include <common.h>

std::set<std::string> FrequentWords(const std::string& text, const size_t k) {
    const size_t len = text.length();
    size_t* counts = new size_t[len - k];
    for (size_t i = 0u; i < len - k; i++) {
        std::string sub = text.substr(i, k);
        std::string str = text.substr(i, len - 1u);
        counts[i] = countSubstring(str, sub);
    }
    size_t max = *std::max_element(counts, counts + len - k);

    std::set<std::string> patterns;

    for (size_t i = 0u; i < len - k; i++) {
        if (counts[i] == max) {
            patterns.insert(text.substr(i, k));
        }
    }

    delete[] counts;
    return patterns;
}


int main() {
    std::ifstream input("input.txt", std::ios::in);
    std::string text;
    input >> text;
    size_t kmers;
    input >> kmers;
    input.close();

    std::set<std::string> res = FrequentWords(text, kmers);

    std::ofstream output("output.txt", std::ios::out);

    for (const auto& str : res) {
        output << str << " ";
    }
    output << std::endl;
    output.close();
    return 0;
}
