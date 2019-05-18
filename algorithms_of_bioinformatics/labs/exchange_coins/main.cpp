// Copyright 2018 Kirill Petrov

#include <common.h>

size_t DPChange(const std::vector<size_t>& pars, const size_t coins) {
    size_t* const bestNumCoins = new size_t[coins + 1u];
    size_t minPar = pars.back();
    for (size_t m = 0u; m <= coins; m++) {
        if (m < minPar) {
            bestNumCoins[m] = 0u;
            continue;
        }
        bestNumCoins[m] = -1;
        for (const size_t par : pars) {
            if (m >= par)
                bestNumCoins[m] = std::min(bestNumCoins[m], bestNumCoins[m - par] + 1u);
        }
    }

    const size_t res = bestNumCoins[coins];
    delete[] bestNumCoins;
    return res;
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    size_t coins;
    input >> coins;
    std::vector<size_t> pars;

    std::string par;
    while (std::getline(input, par, ',')) {
        pars.push_back(std::stoul(par));
    }
    input.close();

    const size_t numCoins = DPChange(pars, coins);

    std::ofstream output("output.txt", std::ios::out);
    output << numCoins << std::endl;
    output.close();
    return 0;
}
