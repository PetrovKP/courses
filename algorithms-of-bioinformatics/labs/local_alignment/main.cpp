// Copyright 2018 Kirill Petrov

#include <common.h>

#define IND(i, j) ((i)*(m) + (j))

static const int MULCT = 5;

using Args = std::tuple<int, std::string, std::string>;

void LCS(std::string &res1, std::string &res2, const int* const s, const std::string &v,
    const std::string &w, const size_t m, const size_t i, const size_t j) {

    if (s[IND(i - 1, j - 1)] >= s[IND(i - 1, j)] && s[IND(i - 1, j - 1)] >= s[IND(i, j - 1)]) {
        if (s[IND(i - 1, j - 1)] == 0)
            return;
        res1 += v[i - 2];
        res2 += w[j - 2];
        if (i == 0u) {
            res1 += '-';
            return;
        } else if (j == 0u) {
            res2 += '-';
            return;
        if (s[IND(i - 1, j - 1)] == 0)
            return;
        }
        LCS(res1, res2, s, v, w, m, i - 1u, j - 1u);
    } else {
        if (i == 0u || j == 0u)
            return;
        if (s[IND(i - 1, j)] >= s[IND(i, j- 1)]) {
            res1 += v[i - 2];
            res2 += '-';
            LCS(res1, res2, s, v, w, m, i - 1u, j);
        } else {
            res1 += '-';
            res2 += w[j - 2];
            LCS(res1, res2, s, v, w, m, i, j - 1u);
        }
    }
}

Args LCSubstr(const std::string& v, const std::string& w) {
    const size_t n = v.length() + 1u;
    const size_t m = w.length() + 1u;

    int* const s = new int[n * m];

    for (size_t i = 0u; i < n; i++)
        s[IND(i, 0u)] = 0;

    for (size_t j = 1u; j < m; j++)
        s[IND(0u, j)] = 0;

    for (size_t i = 1u; i < n; i++) {
        for (size_t j = 1u; j < m; j++) {
            s[IND(i, j)] = std::max(s[IND(i - 1u, j)], s[IND(i, j - 1u)]) - MULCT;
            const size_t vi = CONVERT_AMINO_ACIDS.at(v[i - 1]);
            const size_t wj = CONVERT_AMINO_ACIDS.at(w[j - 1]);
            s[IND(i, j)] = std::max(s[IND(i, j)], s[IND(i - 1u, j - 1u)] + PAM250[vi][wj]);
            s[IND(i, j)] = std::max(s[IND(i, j)], 0);
            // printf("%d ", s[IND(i, j)]);
        }
    // printf("\n");
    }

    std::string res1, res2;

    LCS(res1, res2, s, v, w, m, n, m);

    std::reverse(res1.begin(), res1.end());
    std::reverse(res2.begin(), res2.end());

    const int maxScore = s[IND(n - 1, m - 1)];

    delete[] s;
    return std::make_tuple(maxScore, res1, res2);
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    std::string v, w;
    input >> v >> w;
    input.close();

    const Args res = LCSubstr(v, w);

    std::ofstream output("output.txt", std::ios::out);
    output << std::get<0u>(res) << std::endl << std::get<1u>(res) << std::endl << std::get<2u>(res) << std::endl;
    output.close();
    return 0;
}
