// Copyright 2018 Kirill Petrov

#include "graph.h"

int main() {
    std::ifstream input("input.txt", std::ios::in);
    std::string str;
    GraphBruijn<std::string> graph;

    while (std::getline(input, str)) {
        graph.addEdge(str);
    }
    input.close();
    graph.build();

    std::ofstream output("output.txt", std::ios::out);
    graph.save(output);
    output.close();

    return 0;
}
