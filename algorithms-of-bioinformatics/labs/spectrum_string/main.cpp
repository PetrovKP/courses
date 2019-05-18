// Copyright 2018 Kirill Petrov

#include <common.h>

std::list<std::string> Spectrum(const size_t k, const std::string& str) {
    std::list<std::string> strings;
    for (size_t i = 0u; i < str.length() - k + 1; i++) {
        strings.push_back(str.substr(i, k));
    }
    return strings;
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    size_t k;
    std::string str;
    input >> k >> str;
    input.close();

    const std::list<std::string> res = Spectrum(k, str);

    std::ofstream output("output.txt", std::ios::out);
    for (const std::string& s : res)
        output << s << std::endl;
    output.close();
    return 0;
}
