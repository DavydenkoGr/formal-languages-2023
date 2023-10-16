#include <cstdint>
#include <map>
#include <set>
#include <vector>

#ifndef NFA_H
#define NFA_H

const char kEps = '1';

class NFA {
 protected:
  struct Node {
    std::map<char, std::set<uint32_t>> edges;
    bool terminal;
    Node() : terminal(false) {}
  };

  uint32_t start_;
  uint32_t terminal_;
  std::vector<Node> nodes_;

 private:
  void MergeNodes(const NFA& nfa) {
    uint32_t size = nodes_.size();

    for (const Node& node : nfa.nodes_) {
      Node copy_node;

      for (char chr = 0; chr < 26; ++chr) {
        chr += 'a';

        if (!node.edges.count(chr)) {
          continue;
        }

        for (uint32_t edge : node.edges.at(chr)) {
          copy_node.edges[chr].insert(edge + size);
        }
      }

      if (node.edges.count(kEps)) {
        for (uint32_t edge: node.edges.at(kEps)) {
          copy_node.edges[kEps].insert(edge + size);
        }
      }

      nodes_.push_back(copy_node);
    }
  }

 public:
  // 0-indexation
  explicit NFA(size_t size, uint32_t start, const std::vector<uint32_t>& terminal,
               const std::vector<std::map<char, std::set<uint32_t>>>& edges_array)
      : start_(start) {
    for (uint32_t i = 0; i < size; ++i) {
      nodes_.emplace_back();
    }

    for (uint32_t i = 0; i < size; ++i) {
      nodes_[i].edges = edges_array[i];
    }

    // Add single terminal node
    terminal_ = nodes_.size();
    for (uint32_t terminal_index : terminal) {
      nodes_[terminal_index].edges[kEps].insert(nodes_.size());
    }
    nodes_.emplace_back();
    nodes_.back().terminal = true;
  }

  void Kleene() {
    // New start node
    Node new_start;

    new_start.edges[kEps].insert(start_);

    start_ = nodes_.size();
    nodes_.push_back(new_start);

    // Eps transitions to new single terminal from new start and old terminal
    nodes_[terminal_].edges[kEps].insert(nodes_.size());
    nodes_[start_].edges[kEps].insert(nodes_.size());

    // New terminal node
    nodes_[terminal_].terminal = false;

    terminal_ = nodes_.size();
    nodes_.emplace_back();
    nodes_[nodes_.size() - 1].terminal = true;
  }

  void Concatenate(const NFA& nfa) {
    // Merge nodes
    uint32_t size = nodes_.size();
    MergeNodes(nfa);

    // Edge to second nfa
    nodes_[terminal_].edges[kEps].insert(nfa.start_ + size);

    // New terminal
    nodes_[terminal_].terminal = false;
    terminal_ = size + nfa.terminal_;
    nodes_[terminal_].terminal = true;
  }

  void Add(const NFA& nfa) {
    // Merge nodes
    uint32_t size = nodes_.size();
    MergeNodes(nfa);

    // New start node
    Node new_start;

    new_start.edges[kEps].insert(start_);
    new_start.edges[kEps].insert(nfa.start_ + size);

    start_ = nodes_.size();
    nodes_.push_back(new_start);

    // New terminal node
    nodes_[terminal_].terminal = false;

    nodes_[terminal_].edges[kEps].insert(nodes_.size());
    nodes_[nfa.terminal_ + size].edges[kEps].insert(nodes_.size());

    terminal_ = nodes_.size();
    nodes_.emplace_back();
    nodes_[nodes_.size() - 1].terminal = true;
  }
};

#endif
