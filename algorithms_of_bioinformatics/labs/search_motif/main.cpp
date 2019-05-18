// Copyright 2018 Kirill Petrov

#include <common.h>
#include <cmath>

size_t* bestS;
size_t maxScore;
std::string* text;
size_t n, t, l;

inline size_t convert(const char c) {
    size_t res;
    if (c == 'A') {
        res = 0u;
    } else if (c == 'C') {
        res = 1u;
    } else if (c == 'G') {
        res = 2u;
    } else {
        res = 3u;
    }
    return res;
}

size_t Score(const size_t s[]) {
    size_t profile[l][4u] = { 0u };

    size_t score = 0u;
    for (size_t i = 0u; i < t; i++) {
        for (size_t j = 0u; j < l; j++) {
            profile[j][convert(text[i][j + s[i]])]++;
        }
    }

    for (size_t i = 0u; i < l; i++) {
        score += *std::max_element(profile[i], profile[i] + 4u);
    }
    return score;
}

void Brute(const size_t index, size_t s[]) {
    if (index == t) {
        const size_t score = Score(s);
        if (score >= maxScore) {
            maxScore = score;
            std::copy(s, s + t, bestS);
        }
    } else {
        for (size_t i = 0u; i < n; i++) {
            s[index] = i;
            Brute(index + 1u, s);
        }
    }
}

int main() {
//    INIT
    std::ifstream input("input.txt", std::ios::in);
    maxScore = 0u;
    input >> l >> t;
    text = new std::string[t];
    for (size_t i = 0u; i < t; i++) {
        input >> text[i];
    }
    input.close();

    n = text[0].length() - l + 1;

    size_t s[t] = {0u};
    bestS = new size_t[t];
//    ***********************

    Brute(0u, s);

    std::ofstream output("output.txt", std::ios::out);
    for (size_t i = 0; i < t; i++) {
        output << text[i].substr(bestS[i], l) << std::endl;
    }
    output.close();

    delete[] bestS;
    delete[] text;

    return 0;
}
