// Copyright 2018 Kirill Petrov

#include <common.h>

#define IND_S(i, j) ((i)*(m) + (j))
#define IND_D(i, j) ((i)*(m - 1u) + (j))

enum class Dir {
    UP,
    LEFT,
    DIAG,
};

void LCS(std::string &res, const Dir* const direction, const std::string v, const size_t m, const size_t i, const size_t j) {
    if (direction[IND_D(i, j)] == Dir::DIAG) {
        res += v[i];
        if (i == 0u || j == 0u)
            return;
        LCS(res, direction, v, m, i - 1u, j - 1u);
    } else {
        if (i == 0u || j == 0u)
            return;
        if (direction[IND_D(i, j)] == Dir::UP) {
            LCS(res, direction, v, m, i - 1u, j);
        } else {
            LCS(res, direction, v, m, i, j - 1u);
        }
    }
}

std::string LCSubstr(const std::string v, const std::string w) {
    std::string res;
    const size_t n = v.length() + 1u;
    const size_t m = w.length() + 1u;

    Dir* const direction = new Dir[(n - 1u) * (m - 1u)];
    size_t* const s = new size_t[n * m];

    for (size_t i = 0u; i < n; i++)
        s[IND_S(i, 0u)] = 0u;

    for (size_t j = 1u; j < m; j++)
        s[IND_S(0u, j)] = 0u;

    for (size_t i = 1u; i < n; i++) {
        for (size_t j = 1u; j < m; j++) {
            s[IND_S(i, j)] = std::max(s[IND_S(i - 1u, j)], s[IND_S(i, j - 1u)]);
            if (v[i - 1] == w[j - 1])
                s[IND_S(i, j)] = std::max(s[IND_S(i, j)], s[IND_S(i - 1u, j - 1u)] + 1u);

            if (s[IND_S(i, j)] == s[IND_S(i - 1u, j - 1u)] + 1u)
                direction[IND_D(i - 1u, j - 1u)] = Dir::DIAG;
            else if (s[IND_S(i, j)] == s[IND_S(i - 1u, j)])
                direction[IND_D(i - 1u, j - 1u)] = Dir::UP;
            else
                direction[IND_D(i - 1u, j - 1u)] = Dir::LEFT;
        }
    }

    LCS(res, direction, v, m, n - 2u, m - 2u);
    std::reverse(res.begin(), res.end());

    delete[] s;
    delete[] direction;
    return res;
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    std::string v, w;
    input >> v >> w;
    input.close();

    const std::string str = LCSubstr(v, w);

    std::ofstream output("output.txt", std::ios::out);
    output << str << std::endl;
    output.close();
    return 0;
}
