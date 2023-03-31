#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
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
#include <random>

using ChristmasTree = size_t;

struct TreeProblem {
  int max_group_size;
  std::vector<uint64_t> gifts;
  std::vector<std::pair<ChristmasTree, ChristmasTree>> connections;
};

#endif

uint64_t solve(const TreeProblem & problem) {
    std::unordered_map<ChristmasTree, std::vector<ChristmasTree>> map; //adjacency list
    std::unordered_map<ChristmasTree, uint64_t> prevResult;
    std::unordered_map<ChristmasTree, uint64_t> type1; //memoization hashmap when picking root and grandchildren
    std::unordered_map<ChristmasTree, uint64_t> type2; //when not picking root, but picking children
    std::unordered_map<ChristmasTree, uint64_t> type3; //when utilizing max-group-size = 2

    //vector and queue are used to store the vertices into a suitable iteration format
    std::deque<std::pair<ChristmasTree, ChristmasTree>> reverseOrder; //used to iterate over vertices in a specific order
    std::queue<std::pair<ChristmasTree, ChristmasTree>> queue; //queue of vertices and their parents

    //rewrite connections as adjacency list
    for (auto i : problem.connections) {
        map[i.first].push_back(i.second);
        map[i.second].push_back(i.first);
    }

    //add tree root as first in queue - since graph is undirected we can always take node 0 - it's parent is itself.
    queue.emplace(0, 0);
    reverseOrder.emplace_front(0, 0);
    while (!queue.empty()) {
        auto next = queue.front();
        queue.pop();

        //all the node's children that are not the parent of the node itself - avoid infinite loops since graph is undirected
        for (auto i : map[next.first]) {
            if (i != next.second) {
                queue.emplace(i, next.first);
                reverseOrder.emplace_front(i, next.first);
            }
        }
    }
    
    //compute the maximums for each possible option bottom-up iteratively
    for (auto vertexPair : reverseOrder) {
        uint64_t opt1 = problem.gifts[vertexPair.first];
        uint64_t opt2 = 0;
        uint64_t opt3 = problem.gifts[vertexPair.first];

        //check most efficient solutions for all children of the node in vertexpair
        for (auto i: map[vertexPair.first]) {
            if (i != vertexPair.second) { //.second is the parent, do not check it
                //if max group size is 2, check the maximums between the 3rd type as well
                if (problem.max_group_size == 2) {
                    opt2 += std::max(prevResult[i], type3[i]);
                } else { //else just take the maximum previous result
                    opt2 += prevResult[i];
                }
                
                //now check the grandchildren of the children in vertex pair
                for (auto j: map[i]) {
                    if (j != vertexPair.first) {
                        if (problem.max_group_size == 2) {
                            opt1 += std::max(prevResult[j], type3[j]);
                        } else {
                            opt1 += prevResult[j];
                        }
                    }
                }
            }
        }
	
	//check if its best to choose the node and all grandchildren, all children, or the node and one child
        if (problem.max_group_size == 2) {
            auto & tmpVec = map[vertexPair.first];
            std::sort(tmpVec.begin(), tmpVec.end(), [&](auto lhs, auto rhs){
                if (lhs == vertexPair.second) {
                    return true;
                }
                else if (rhs == vertexPair.second) {
                    return false;
                }
                else {
                    return ((long long int)type1[lhs] - (long long int)type2[lhs] <  (long long int)type1[rhs] - (long long int)type2[rhs]);
                }
            });

            for (size_t i = 0; i < tmpVec.size(); i++) {
                if (tmpVec[i] != vertexPair.second && i != tmpVec.size() - 1) {
                    opt3 += type2[tmpVec[i]];
                }
                else if (tmpVec[i] != vertexPair.second) {
                    opt3 += type1[tmpVec[i]];
                }
            }
            type1[vertexPair.first] = opt1;
            type2[vertexPair.first] = opt2;
            type3[vertexPair.first] = opt3;
        }

        prevResult[vertexPair.first] = std::max(opt1, opt2);
    }

    return problem.max_group_size == 2 ? std::max(prevResult[0], type3[0]) : prevResult[0];
}

#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>;

const std::vector<TestCase> BASIC_TESTS = {
  ///0
  { 3, { 1, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},

  ///1
  { 4, { 1, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},

  ///2
  { 57, { 1, {
    17, 11, 5, 13, 8, 12, 7, 4, 2, 8,
  },{
    {1, 4}, {6, 1}, {2, 1}, {3, 8}, {8, 0}, {6, 0}, {5, 6}, {7, 2}, {0, 9},
  }}},

  ///3
  { 85, { 1, {
    10, 16, 13, 4, 19, 8, 18, 17, 18, 19, 10,
  }, {
    {9, 7}, {9, 6}, {10, 4}, {4, 9}, {7, 1}, {0, 2}, {9, 2}, {3, 8}, {2, 3}, {5, 4},
  }}},

  ///4
  { 79, { 1, {
    8, 14, 11, 8, 1, 13, 9, 14, 15, 12, 1, 11,
  }, {
    {9, 1}, {1, 2}, {1, 4}, {5, 10}, {7, 8}, {3, 7}, {11, 3}, {11, 10}, {6, 8}, {0, 1}, {0, 3},
  }}},

  ///5
  { 102, { 1, {
    15, 10, 18, 18, 3, 4, 18, 12, 6, 19, 9, 19, 10,
  }, {
    {10, 2}, {11, 10}, {6, 3}, {10, 8}, {5, 3}, {11, 1}, {9, 5}, {0, 4}, {12, 3}, {9, 7}, {11, 9}, {4, 12},
  }}},

  ///6
  { 93, { 1, {
    1, 7, 6, 18, 15, 2, 14, 15, 18, 8, 15, 1, 5, 6,
  }, {
    {0, 13}, {6, 12}, {0, 12}, {7, 8}, {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2, 6}, {6, 9}, {12, 7}, {0, 4}, {0, 5},
  }}},
};

const std::vector<TestCase> BONUS_TESTS = {
  { 3, { 2, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
  { 5, { 2, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
  { 85, { 2, {
                     8, 14, 11, 8, 1, 13, 9, 14, 15, 12, 1, 11,
             }, {
                        {9, 1}, {1, 2}, {1, 4}, {5, 10}, {7, 8}, {3, 7}, {11, 3}, {11, 10}, {6, 8}, {0, 1}, {0, 3},
                }}},
  { 121, { 2, {    // 0   1   2  3   4  5   6   7   8   9  10
                     10, 16, 13, 4, 19, 8, 18, 17, 18, 19, 10,
             }, {
                        {9, 7}, {9, 6}, {10, 4}, {4, 9}, {7, 1}, {0, 2}, {9, 2}, {3, 8}, {2, 3}, {5, 4},
                }}},
  {57, {2, {25, 10, 5, 7, 2, 13, 10}, {
                                              {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {1, 6}
                                      }}},
  {90, {2, {
      25, 10, 5, 7, 2, 13, 10, 10, 10, 10, 10
  }, {
                        {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 7}, {2, 8}, {3, 9}, {3,10}
  }}},
  {115, {2, {
                   25, 10, 5, 7, 2, 13, 10, 10, 10, 10, 10, 25
           }, {
                      {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 7}, {2, 8}, {3, 9}, {3,10}, {10, 11}
              }}},
  {185, {2, {
                   25, 10, 5, 7, 2, 13, 10, 10, 10, 10, 10, 25, 70
           }, {
                      {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 7}, {2, 8}, {3, 9}, {3,10}, {10, 11}, {0, 12}
              }}},
  {110, {2, {
                    10, 20, 70, 10, 1, 5, 10
            }, {
                                   {0, 1}, {0, 2}, {1, 3}, {1, 4}, {3, 5}, {4, 6}
               }}},
  {196, {2, {
                    100, 20, 70, 10, 1, 5, 10
            }, {
                       {0, 1}, {0, 2}, {1, 3}, {1, 4}, {3, 5}, {4, 6}
               }}},
  {400, {2, {
                    100, 1, 1, 100, 100, 100
            }, {
                                   {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}
               }}},
  {33, {2, {
                    10,0,11,0,12
            }, {
                       {0, 1}, {1, 2}, {2, 3}, {3, 4}
               }}},
  {230, {2, {
                   3,20,23,13,2,29,23,17,30,10,21,4,2,11,1,14,29,18,4
           }, {
                      {0,1},{0,2},{0,3},{1,4},{1,5},{2,6},{2,7},{3,8},{3,9},
                      {6,10},{6,11},{7,12},{7,13},{9,14},{11,15},{11,16},
                      {12,17},{12,18}
              }}},
  {219, {2, {
                    24,20,14,1,7,27,30,20,4,24,23,24,8,8,3,1,24,3
            }, {
                       {0,1},{0,2},{0,3},{1,4},{1,5},{2,6},{2,7},{3,8},{3,9},{6,10},{7,11},{8,12},{9,13},
                       {10,14},{11,15},{12,16},{13,17}
               }}},
  {20, {2, {
      10, 10
  }, {
                        {0, 1}
  }}}
};

void test(const std::vector<TestCase>& T) {
  int i = 0;
  for (auto &[s, t] : T) {
    if (s != solve(t))
      std::cout << "Error in " << i << " (returned " << solve(t) << ")"<< std::endl;
    else
        std::cout << solve(t) << " for " << i << std::endl;
    i++;
  }
  std::cout << "Finished" << std::endl;
}

int main() {
    test(BASIC_TESTS);
    test(BONUS_TESTS);
    //std::cout << solve(BONUS_TESTS[4].second) << std::endl;
}

#endif

