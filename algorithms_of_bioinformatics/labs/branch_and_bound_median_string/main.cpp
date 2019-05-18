// Copyright 2018 Kirill Petrov

#include <common.h>

constexpr size_t FOUR = 4UL;
constexpr char CONVERT[FOUR] = { 'A', 'C', 'G', 'T' };

using typeText = std::vector<std::string>;

inline std::string ConvertToWord(const size_t s[], const size_t l) {
    std::string res(l, 'A');
    for (size_t i = 0u; i < l; i++) {
        res[i] = CONVERT[s[i]];
    }
    return res;
}

inline size_t Distance(const std::string s1, const std::string s2, const size_t l) {
    size_t distanse = 0u;
    for (size_t i = 0u; i < l; i++) {
        if (s1[i] != s2[i]) {
            distanse++;
        }
    }
    return distanse;
}

size_t TotalDistance(const typeText& dna, const std::string& word, const size_t t, const size_t n, const size_t l) {

    size_t dH = 0u;
    for (size_t i = 0u; i < t; i++) {
        size_t dHmin = -1;
        for (size_t j = 0u; j < n; j++) {
            dHmin = std::min(Distance(dna[i].substr(j, l), word, l), dHmin);
        }
        dH += dHmin;
    }
    return dH;
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

std::string BranchAndBoundMedianSearch(const typeText& dna, const size_t t, const size_t n, const size_t l) {
    std::string bestWord;
    size_t s[l] = { 0u };
    size_t minDistance = -1;
    long int i = 0u;
    while (i > -1) {
        if (i < l - 1) {
            const std::string prefix = ConvertToWord(s, i);
            const size_t optimisticDistance = TotalDistance(dna, prefix, t, n, i) + 0UL;
            i = optimisticDistance > minDistance ? ByPass(s, i, FOUR) : NextVertex(s, i, l, FOUR);
        } else {
            const std::string word = ConvertToWord(s, l);
            const size_t totalDistance = TotalDistance(dna, word, t, n, l);
            if (totalDistance < minDistance) {
                minDistance = totalDistance;
                bestWord = word;
            }
        }
        i = NextVertex(s, i, l, FOUR);
    }
    return bestWord;
}


int main() {
    std::ifstream input("input.txt", std::ios::in);
    size_t l;
    input >> l;
    std::string str;
    typeText text;
    std::getline(input, str);
    while (std::getline(input, str)) {
        text.push_back(str);
    }
    input.close();

    const size_t t = text.size();
    const size_t n = text[0].length() - l + 1u;

    const std::string bestWord = BranchAndBoundMedianSearch(text, t, n, l);

    std::ofstream output("output.txt", std::ios::out);
    output << bestWord << std::endl;
    output.close();
    return 0;
}
