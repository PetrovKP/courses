#include <lemon/euler.h>
#include <lemon/list_graph.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <array>

template <typename Type>
using adjacency_t = std::map<Type, std::list<Type>>;

template <typename Type>
void read(std::ifstream& stream, adjacency_t<Type>& adj) {
    std::string str;
    size_t index = 0u;
    while (std::getline(stream, str)) {
        std::stringstream ss(str);
        Type vertex;
        std::string trash;
        ss >> vertex >> trash;
        Type enge;
        char tmp;
        std::list<Type> engelist;
        while(ss >> enge) {
            engelist.push_back(enge);
            ss >> tmp;
        }
        adj[vertex] = engelist;
    }
}

template <typename Type>
void cycleClosure(adjacency_t<Type>& adj) {
    Type vertexAdded, engeAdded;
    for (auto adjIter = adj.begin(); adjIter != adj.end(); ++adjIter) {
        const auto vertex = adjIter->first;
        size_t countVertex = 0u;
        for (auto adjIter2 = adj.begin(); adjIter2 != adj.end(); ++adjIter2) {
            countVertex += std::count(adjIter2->second.begin(), adjIter2->second.end(), vertex);
        }
        if (countVertex != adjIter->second.size()) engeAdded = vertex;

        const auto engelist = adjIter->second;
        for (auto engeIter = engelist.begin(); engeIter != engelist.end(); ++engeIter) {
            if (adj.find(*engeIter) == adj.end()) {
                vertexAdded = *engeIter;
            }
        }
    }
    adj[vertexAdded] = { engeAdded };
}

template <typename Type>
lemon::ListDigraph::Node addVertex(lemon::ListDigraph& graph, lemon::ListDigraph::NodeMap<Type>& nodeMap, const Type value) {
    for (lemon::ListDigraph::NodeIt node(graph); node != lemon::INVALID; ++node) {
        if (nodeMap[node] == value)
            return node;
    }
    const lemon::ListDigraph::Node currentNode = graph.addNode();
    nodeMap[currentNode] = value;
    return currentNode;
}

template <typename Type>
void fillGraph(lemon::ListDigraph& graph, lemon::ListDigraph::NodeMap<Type>& nodeMap, const adjacency_t<Type>& adj) {
    for (auto adjIter = adj.begin(); adjIter != adj.end(); ++adjIter) {
        const lemon::ListDigraph::Node sourceNode = addVertex(graph, nodeMap, adjIter->first);
        const auto engelist = adjIter->second;
        for (auto engeIter = engelist.begin(); engeIter != engelist.end(); ++engeIter) {
            const lemon::ListDigraph::Node targetNode = addVertex(graph, nodeMap, *engeIter);
            graph.addArc(sourceNode, targetNode);
        }
    }
}

template <typename Type>
void printGraph(const lemon::ListDigraph& graph, const lemon::ListDigraph::NodeMap<Type>& nodeMap) {
    std::cout << "Edges of the tree: " << std::endl;
    for(lemon::ListDigraph::ArcIt arc(graph); arc != lemon::INVALID; ++arc) {
        std::cout << "(" << nodeMap[graph.source(arc)] << "," << nodeMap[graph.target(arc)] << "); ";
    }
    std::cout << std::endl;
}
