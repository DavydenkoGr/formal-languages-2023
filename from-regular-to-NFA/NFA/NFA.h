#include <cstdint>
#include <map>
#include <set>
#include <vector>

#ifndef NFA_H
#define NFA_H

const char kEps = '1';

class NFA {
 private:
  struct Node {
    std::map<char, std::set<uint32_t>> edges;
    bool terminal;
    Node() : terminal(false) {}
  };

  uint32_t start_;
  uint32_t terminal_;
  std::vector<Node> nodes_;

  void MergeNodes(const NFA& nfa);

 public:
  explicit NFA(size_t size, uint32_t start, const std::vector<uint32_t>& terminal,
               const std::vector<std::map<char, std::set<uint32_t>>>& edges_array);

  void Kleene();

  void Concatenate(const NFA& nfa);

  void Add(const NFA& nfa);

  void RemoveEpsTransitions();

  void RebuildToSubwordNFA();

  bool FindSubword(char chr, uint32_t count);
};

#endif
