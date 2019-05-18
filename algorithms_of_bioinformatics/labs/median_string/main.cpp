// Copyright 2018 Kirill Petrov

#include <common.h>

const size_t FOUR = 4;
const char CONVERT[FOUR] = { 'A', 'C', 'G', 'T' };

std::string bestWord;
size_t minDistance;
std::vector<std::string> text;
size_t n, t, l;


inline size_t Distance(const std::string s1, const std::string s2) {
    size_t distanse = 0u;
    for (size_t i = 0u; i < l; i++) {
        if (s1[i] != s2[i]) {
            distanse++;
        }
    }
    return distanse;
}

size_t TotalDistance(const std::string& word) {

    size_t dH = 0u;
    for (size_t i = 0u; i < t; i++) {
        size_t dHmin = -1;
        for (size_t j = 0u; j < n; j++) {
            dHmin = std::min(Distance(text[i].substr(j, l), word), dHmin);
        }
        dH += dHmin;
    }
    return dH;
}

void Brute(const size_t index, size_t woldInd[]) {
    if (index == l) {
        std::string word(l, 'A');
        for (size_t i = 0u; i < l; i++) {
            word[i] = CONVERT[woldInd[i]];
        }
        const size_t totalDistance = TotalDistance(word);
        if (totalDistance < minDistance) {
            minDistance = totalDistance;
            bestWord = word;
        }
    } else {
        for (size_t i = 0u; i < FOUR; i++) {
            woldInd[index] = i;
            Brute(index + 1u, woldInd);
        }
    }
}

int main() {
//    INIT
    std::ifstream input("input.txt", std::ios::in);
    minDistance = -1;
    input >> l;
    std::string str;
    std::getline(input, str);
    while (std::getline(input, str)) {
        text.push_back(str);
    }
    t = text.size();
    input.close();

    n = text[0].length() - l + 1;

    size_t s[FOUR] = { 0u };

//    ***********************

    Brute(0u, s);

    std::ofstream output("output.txt", std::ios::out);
    output << bestWord << std::endl;
    output.close();

    return 0;
}
