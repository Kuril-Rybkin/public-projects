#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t;

struct Map {
    size_t places;
    Place start, end;
    std::vector<std::pair<Place, Place>> connections;
    std::vector<std::vector<Place>> items;
};

template<typename F, typename S>
struct std::hash<std::pair<F, S>> {
    std::size_t operator()(const std::pair<F, S> &p) const noexcept {
        // something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

#endif

void adjacenyList(std::vector<std::pair<Place, Place>> const & connections, std::vector<std::vector<Place>> & adj) { //create an adjecency list from an edge list
    for (auto & i : connections) { //cycle through all connections
        adj[i.first].push_back(i.second); //place 1 exists, modify adjacency list

        adj[i.second].push_back(i.first); //place 1 exists, modify adjacency list
    }
}

bool getItemMap(std::vector<std::vector<Place>> const & items, std::vector<size_t> & nodeWeights) { //create a map of places and their items
    for (size_t i = 0; i < items.size(); i++) { //for every type of item
        if (items[i].empty()) { //if it is empty, this item is not contained in any room
            return false; //impossible to find path, return false
        }

        for (auto & j : items[i]) { //for every room j that item i is contained in
            nodeWeights[j] |= 1 << i; //modify existing key or add new key with operator []
        }
    }
    return true;
}

std::list<Place> find_path(const Map &map) {
    std::vector<std::vector<size_t>> adjList(map.places); //initialize adjacency list
    adjacenyList(map.connections, adjList); //get adjacency list

    std::vector<size_t> nodeWeights(map.places); //initialize map of rooms to contained items
    if (!getItemMap(map.items, nodeWeights) || adjList[map.end].empty()) { //create place -> items map and find out if a path is possible
        return std::list<Place>{}; //path is not possible because finish is disconnected
    }

    std::vector<std::pair<size_t, std::pair<Place, size_t>>> nodeHistory; //initialise vector of visited nodes
    std::vector<std::unordered_set<size_t>> nodePrevWeights(map.places); //which nodes we were able to visit before with what number of items collected

    size_t targetWeight = 0; //target weight that the end node should have in order to be valid
    //weight is stored as a binary number, with each item having its own power of 2
    size_t currentWeight = 0; //weight of current path being explored
    Place node; //current place being explored

    if (map.items.size() != 0) { //check if there are enough items to calculate a weight
        targetWeight = (1 << map.items.size()) - 1; //calculate weight of the items in total
    }

    nodeHistory.push_back({map.start, {0, nodeWeights[map.start]}});
    nodePrevWeights[map.start].insert(nodeHistory[0].second.second);

    //if we reached the end node with all the items collected
    if (nodeHistory[0].first == map.end && nodeHistory[0].second.second == targetWeight) {
        return std::list<Place>{map.start};
    }
    
    //BFS for current node
    for (size_t i = 0; i < nodeHistory.size(); i++) {
        node = nodeHistory[i].first; //get node value
        currentWeight = nodeHistory[i].second.second;
        auto & neighbours = adjList[node];

        if (neighbours.empty()) {
            continue;
        }

        for (auto & neighbour : neighbours) {
            auto & neighbourWeights = nodePrevWeights[neighbour];

	    //if we never visited this node with this item collected.
	    //if we did visit it before, then there already exists a shorter path
            if (neighbourWeights.find(currentWeight) == neighbourWeights.end()) {
                size_t totalWeight = nodeWeights[neighbour] | currentWeight;
                nodeHistory.push_back({neighbour, {i, totalWeight}}); //record path
                nodePrevWeights[neighbour].insert(totalWeight);

                //if we have reached the end and the target weight
                if (neighbour == map.end && nodeHistory.back().second.second == targetWeight) {
                    size_t j = nodeHistory.size() - 1;
                    std::list<Place> path;
                    currentWeight = 0;

                    //reconstruct the path
                    while (nodeHistory[j].first != map.start || currentWeight != targetWeight) {
                        currentWeight |= nodeWeights[nodeHistory[j].first];
                        path.push_front(nodeHistory[j].first);
                        j = nodeHistory[j].second.first;
                    }

                    if (path.front() != map.start) {
                        path.push_front(map.start);
                    }

                    return path;
                }
            }
        }
    }

    return std::list<Place>{};
}

#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
        TestCase{1, Map{2, 0, 0, //0
                        {{0, 1}},
                        {{0}}
        }},

        TestCase{3, Map{2, 0, 0, //1
                        {{0, 1}},
                        {{1}}
        }},

        TestCase{3, Map{4, 0, 1, //2
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {}
        }},
        TestCase{4, Map{4, 0, 1, //3
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {{2}}
        }},

        TestCase{0, Map{4, 0, 1, //4
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {{2}, {}}
        }},

        TestCase{0, Map{4, 0, 1, //5
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {{2}, {}}
        }},

        TestCase {5, Map{13, 1, 11, //6
                         {{1, 2}, {1, 3}, {1, 4}, {2, 6}, {2, 5}, {4, 5}, {4, 7}, {4, 8}, {5, 7}, {5, 9}, {5, 10}, {7, 11}, {7, 12}, {9, 11}},
                         {{4, 8, 3}, {5, 10}, {7}}}},

        TestCase {5, Map{5, 0, 4, //7
                         {{0, 1}, {1, 2}, {1, 3}, {1, 4}},
                         {{2}, {1}}}},
        TestCase {0, Map{6, 0, 4, //8
                         {{0, 1}, {1, 2}, {1, 3}, {1, 4}},
                         {{2}, {1}, {5}}}},

        TestCase {0, Map{5, 0, 5, //9
                         {{0, 1}, {1, 2}, {1, 3}, {1, 4}},
                         {{2}, {1}}}},

        TestCase {5, Map{5, 0, 4, //10
                         {{0, 1}, {1, 2}, {1, 3}, {1, 4}},
                         {{2}, {1}}}},

        TestCase {6, Map{13, 1, 1, //11
                         {{1, 2}, {1, 3}, {1, 4}, {2, 6}, {2, 5}, {4, 5}, {4, 7}, {4, 8}, {5, 7}, {5, 9}, {5, 10}, {7, 11}, {7, 12}, {9, 11}},
                         {{4, 8, 3}, {5, 10}, {7}}}},

        TestCase {1, Map{2, 0, 0, //12
                         {{0, 1}},
                         {{0}}}},

        TestCase{18, Map{19,8,9, //13
                         {{1, 2}, {1, 7}, {2, 3}, {2, 5}, {2, 6}, {3, 4}, {3, 6}, {3, 8}, {4, 10}, {5, 6}, {5, 11}, {5, 12}, {6, 9}, {9, 14}, {12, 13}, {13, 15}, {14, 16}, {15, 18}, {16, 18}, {16, 17}},
                         {{7},{6},{17},{13}}}},

        TestCase{1, Map{2, 0, 0, //14
                        {{0, 1}},
                        {{0}, {0}, {0}, {0}}
        }},
        TestCase{0, Map{3, 2, 0, //15
                        {{0, 1}},
                        {{0}, {0}, {0}, {0}}
        }},
};

int main() {
    int fail = 0;
    for (size_t i = 0; i < examples.size(); i++) {
        auto sol = find_path(examples[i].second);
        if (sol.size() != examples[i].first) {
            std::cout << "Wrong answer for map " << i << std::endl;
            fail++;
        }
        else {
            std::cout << "Correct answer for map " << i << std::endl;
        }

        std::cout << "ANSWER = [";
        for (auto j : sol) {
            std::cout << j << ", ";
        }
        std::cout << "]" << std::endl;
    }

    if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
    else std::cout << "All tests completed" << std::endl;

    return 0;
}

#endif
