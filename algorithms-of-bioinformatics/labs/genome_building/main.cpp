// Copyright 2018 Kirill Petrov

#include "lemon_graph.h"
#include "graph.h"

int main() {
    std::ifstream input("input.txt", std::ios::in);

    std::string str; size_t k;
    GraphBruijn<std::string> graphBruijn;
    input >> k;
    std::getline(input, str);

    while (std::getline(input, str)) {
        graphBruijn.addEdge(str);
    }
    input.close();
    graphBruijn.build();

    adjacency_t<std::string> adj = graphBruijn.getAdjacency();

    cycleClosure<std::string>(adj);

    lemon::ListDigraph graph;
    lemon::ListDigraph::NodeMap<std::string> nodeMap(graph);

    fillGraph<std::string>(graph, nodeMap, adj);

    std::ofstream output("output.txt", std::ios::out);

    lemon::DiEulerIt<lemon::ListDigraph> enge(graph);
    output << nodeMap[graph.source(enge)];
    ++enge;

    for(; enge != lemon::INVALID; ++enge) {
            output << nodeMap[graph.source(enge)];
    }
    output << std::endl;
    output.close();

    return 0;
}
