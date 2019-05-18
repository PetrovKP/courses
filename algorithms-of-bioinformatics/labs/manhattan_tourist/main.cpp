// Copyright 2018 Kirill Petrov

#include <common.h>

#define IND_S(i, j) ((i)*(m + 1u) + (j))
#define IND_W(i, j) ((i)*(m) + (j))

size_t ManhattanTourist(const size_t* const southernWeights, const size_t* const orientalWeights, const size_t n, const size_t m) {
    size_t* const s = new size_t[(n + 1u) * (m + 1u)];
    s[IND_S(0u, 0u)] = 0u;

    for (size_t i = 1u; i < n + 1u; i++)
        s[IND_S(i, 0u)] = s[IND_S(i - 1u, 0u)] + southernWeights[IND_S(i - 1u, 0u)];

    for (size_t j = 1u; j < m + 1u; j++)
        s[IND_S(0u, j)] = s[IND_S(0u, j - 1u)] + orientalWeights[IND_W(0u, j - 1u)];

    for (size_t i = 1u; i < n + 1u; i++)
        for (size_t j = 1u; j < m + 1u; j++)
            s[IND_S(i, j)] = std::max(s[IND_S(i - 1u, j)] + southernWeights[IND_S(i - 1, j)],
                                      s[IND_S(i, j - 1u)] + orientalWeights[IND_W(i, j - 1u)]);

    const size_t res = s[IND_S(n, m)];
    delete[] s;
    return res;
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    size_t n, m;
    input >> n >> m;
    size_t* const southernWeights = new size_t[n * (m + 1u)];
    size_t* const orientalWeights = new size_t[(n + 1u) * m];

    for (size_t i = 0u; i < n; i++)
        for (size_t j = 0u; j < m + 1u; j++)
            input >> southernWeights[IND_S(i, j)];

    char tmp;
    input >> tmp;

    for (size_t i = 0u; i < n + 1u; i++)
        for (size_t j = 0u; j < m; j++)
            input >> orientalWeights[IND_W(i, j)];

    input.close();

    const size_t distanse = ManhattanTourist(southernWeights, orientalWeights, n, m);

    std::ofstream output("output.txt", std::ios::out);
    output << distanse << std::endl;
    output.close();

    delete[] southernWeights;
    delete[] orientalWeights;
    return 0;
}
