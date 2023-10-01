#include "eps-transitions.h"

//0
//
//0
//2
//
//0 1 eps
//1 2 eps
//2 0 c
//2 1 a

int main() {
  std::vector<std::map<char, std::set<uint32_t>>> transitions;
  std::map<char, std::set<uint32_t>> map;

  std::set<uint32_t> v = {1};
  map.insert(std::pair(kEps, v));
  transitions.push_back(map);
  map.clear();

  v = {2};
  map.insert(std::pair(kEps, v));
  transitions.push_back(map);
  map.clear();

  v = {0};
  map.insert(std::pair('c', v));
  v = {1};
  map.insert(std::pair('a', v));
  transitions.push_back(map);
  map.clear();

  std::vector<uint32_t> terminal = {0, 2};
  NFA nfa(3, 0, terminal, transitions);

  nfa.RemoveEpsTransitions();
}

//0
//
//0
//1
//2
//
//0 0 c
//0 1 a
//1 0 c
//1 1 a
//2 0 c
//2 1 a
