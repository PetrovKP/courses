// Copyright 2018 Kirill Petrov

#include "lemon_graph.h"

int main() {
    adjacency_t<size_t> adj;
    std::ifstream input("input.txt", std::ios::in);
    read<size_t>(input, adj);

    lemon::ListDigraph graph;
    lemon::ListDigraph::NodeMap<size_t> nodeMap(graph);

    fillGraph<size_t>(graph, nodeMap, adj);

    std::ofstream output("output.txt", std::ios::out);
    lemon::DiEulerIt<lemon::ListDigraph> enge(graph);
    size_t target;
    for(; enge != lemon::INVALID; ++enge) {
        output << nodeMap[graph.source(enge)] << "->";
        target = nodeMap[graph.target(enge)];
    }
    output << target << std::endl;
    output.close();

    return 0;
}
