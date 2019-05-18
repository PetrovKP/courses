// Copyright 2018 Kirill Petrov

#include <list>
#include <map>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <fstream>

using adjacency_t = std::unordered_map<std::string, std::pair<std::string, std::list<std::string>>>;

void addAdjacency(const std::string& node, adjacency_t& _adj) {
    const std::string prefix = node.substr(0, node.length() - 1);
    const std::string suffix = node.substr(1);
    _adj[suffix] = std::make_pair(node, std::list<std::string>());
}

void build(adjacency_t& _adj) {
    for (auto& vertex : _adj) {
        const std::string node = vertex.second.first;
        const std::string prefix = node.substr(0, node.length() - 1);
        if (_adj.count(prefix)) {
            _adj[prefix].second.push_back(node);
        }
    }
}

void save(std::ofstream& stream, const adjacency_t& _adj) {
    for (auto& vertex : _adj) {
        if ( !vertex.second.second.empty() ) {
            auto edge = vertex.second.second;
            auto i = edge.begin();
            stream << vertex.second.first << " -> " << *i;
            ++i;
            for (; i != edge.end(); ++i) {
              stream << "," << *i;
            }
            stream  << std::endl;
        }
    }
}

int main() {
    std::ifstream input("input.txt", std::ios::in);
    std::string str;
    adjacency_t adj;

    while (std::getline(input, str)) {
        addAdjacency(str, adj);
    }

    input.close();
    build(adj);

    std::ofstream output("output.txt", std::ios::out);
    save(output, adj);
    output.close();

    return 0;
}
