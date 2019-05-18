// Copyright 2018 Kirill Petrov

#include <common.h>
#include <cmath>

static const size_t FOUR = 4UL;
static std::map<char, size_t> CONVERT = { { 'A', 0u }, { 'C', 1u }, { 'G', 2u }, { 'T', 3u } };

size_t Score(const std::string* const dna, const size_t t, const size_t l, const size_t s[]) {
    size_t profile[l][FOUR] = { 0u };

    for (size_t i = 0u; i < t; i++) {
        for (size_t j = 0u; j < l; j++) {
            profile[j][CONVERT[dna[i][j + s[i]]]]++;
        }
    }

    size_t score = 0u;
    for (size_t i = 0u; i < l; i++) {
        score += *std::max_element(profile[i], profile[i] + FOUR);
    }
    return score;
}

long int ByPass(size_t s[], const long int i, const size_t k) {
    for (size_t j = i - 1u; j >= 0; j--) {
        if (s[j] < k - 1u) {
            s[j]++;
            return j;
        }
    }
    return -1;
}

long int NextVertex(size_t s[], const long int i, const size_t l, const size_t k) {
    if (i < l) {
        s[i + 1] = 0u;
        return i + 1u;
    }
    for (size_t j = l - 1u; j >= 0u; j--) {
        if (s[j] < k - 1u) {
            s[j]++;
            return j;
        }
    }
    return -1;
}

void BranchAndBoundMotifSearch(size_t bestS[], const std::string* const dna, const size_t t, const size_t n, const size_t l) {
    size_t s[t] = { 0u };
    size_t bestScore = 0u;
    long int i = 0u;
    while (i > -1) {
        if (i < t) {
            const size_t optimisticStore = Score(dna, i, l, s) + (t - i)*l;
            i = optimisticStore < bestScore ? ByPass(s, i, n) : NextVertex(s, i, t, n);
        } else {
            const size_t score = Score(dna, t, l, s);
            if (score >= bestScore) {
                bestScore = score;
                std::copy(s, s + t, bestS);
            }
            i = NextVertex(s, i, t, n);
        }
    }
    return;
}


int main() {
    size_t t, l;
    std::ifstream input("input.txt", std::ios::in);
    input >> l >> t;

    std::string* text = new std::string[t];
    for (size_t i = 0u; i < t; i++) {
        input >> text[i];
    }
    input.close();

    const size_t n = text[0].length() - l + 1u;
    size_t bestS[t] = { 0u };

    BranchAndBoundMotifSearch(bestS, text, t, n, l);

    std::ofstream output("output.txt", std::ios::out);
    for (size_t i = 0; i < t; i++) {
        output << text[i].substr(bestS[i], l) << std::endl;
    }
    output.close();

    delete[] text;
    return 0;
}
