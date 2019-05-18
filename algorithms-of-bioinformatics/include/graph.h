#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

template<typename Type>
class GraphBruijn {
private:
    using adjacency_t = std::map<Type, std::list<Type>>;

public:
    GraphBruijn() = default;
    ~GraphBruijn() = default;

    void addEdge(const Type & enge);
    void build();

    void read(std::ifstream& stream);
    void save(std::ostream& stream) const;

    adjacency_t getAdjacency() const;

protected:
    adjacency_t _adj;
    std::list<Type> _inputs;
};


template<>
void GraphBruijn<std::string>::addEdge(const std::string& str) {
            // std::cout << str << std::endl;

    _adj[str.substr(1)] = std::list<std::string>();
    _adj[str.substr(0, str.length() - 1)] = std::list<std::string>();
    _inputs.push_back(str);
}


template<>
void GraphBruijn<std::string>::build() {
    for (auto& str : _inputs) {
        // std::cout << str << std::endl;
        _adj[str.substr(0, str.length() - 1)].push_back(str.substr(1));
    }
    for (auto pairIter = _adj.begin(); pairIter != _adj.end(); ++pairIter) {
        if (pairIter->second.empty()) {
            _adj.erase(pairIter);
        }
    }
}

template<typename Type>
void GraphBruijn<Type>::read(std::ifstream& stream) {
    std::string str;
    while (std::getline(stream, str)) {
        std::stringstream ss(str);
        Type vertex;
        std::string trash;

        ss >> vertex >> trash;
        Type enge;
        char tmp;
        std::list<Type> w;
        while(ss >> enge) {
            w.push_back(enge);
            ss >> tmp;
        }
        _adj[vertex] = w;
    }
}

template<typename Type>
void GraphBruijn<Type>::save(std::ostream& stream) const {
    for (auto& vertex : _adj) {
        auto edge = vertex.second;
        auto i = edge.begin();
        stream << vertex.first << " -> " << *i;
        ++i;
        for (; i != edge.end(); ++i) {
          stream << "," << *i;
        }
        stream  << std::endl;
    }
}

template<typename Type>
std::map<Type, std::list<Type>> GraphBruijn<Type>::getAdjacency() const {
    return _adj;
}

