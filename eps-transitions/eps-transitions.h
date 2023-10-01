#include <cstdint>
#include <map>
#include <set>
#include <vector>

#ifndef EPS_TRANSITIONS_H
#define EPS_TRANSITIONS_H

const char kEps = '1';

class NFA {
 protected:
  struct Node {
    std::map<char, std::set<uint32_t>> edges;
    bool terminal;
    Node() : terminal(false) {}
  };

  uint32_t start_;
  std::vector<Node> nodes_;

 public:
  // 0-indexation
  explicit NFA(size_t size, uint32_t start, const std::vector<uint32_t>& terminal,
      const std::vector<std::map<char, std::set<uint32_t>>>& edges_array)
      : start_(start) {
    for (uint32_t i = 0; i < size; ++i) {
      nodes_.emplace_back();
    }

    for (uint32_t terminal_index : terminal) {
      nodes_[terminal_index].terminal = true;
    }

    for (uint32_t i = 0; i < size; ++i) {
      nodes_[i].edges = edges_array[i];
    }
  }

  void RemoveEpsTransitions() {
    std::vector<std::set<uint32_t>> achievable_by_eps(nodes_.size());
    std::vector<std::set<uint32_t>> temp(nodes_.size());

    // zero iteration of transitive closure
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      for (uint32_t eps_neighbour : nodes_[i].edges[kEps]) {
        achievable_by_eps[i].insert(eps_neighbour);
      }
    }

    // others iterations
    for (uint32_t _ = 0; _ < nodes_.size(); ++_) {
      for (uint32_t i = 0; i < nodes_.size(); ++i) {
        temp[i].clear();

        for (uint32_t eps_neighbour : achievable_by_eps[i]) {
          temp[i].insert(achievable_by_eps[eps_neighbour].begin(),
                         achievable_by_eps[eps_neighbour].end());
        }
      }

      for (uint32_t i = 0; i < nodes_.size(); ++i) {
        nodes_[i].edges[kEps].insert(temp[i].begin(), temp[i].end());
        achievable_by_eps[i] = temp[i];
      }
    }

    // new terminals
    for (Node& node : nodes_) {
      for (uint32_t eps_neighbour : node.edges[kEps]) {
        if (nodes_[eps_neighbour].terminal) {
          node.terminal = true;
        }
      }
    }

    // edges to accessible by eps + letter
    for (Node& node : nodes_) {
      for (uint32_t eps_neighbour : node.edges[kEps]) {
        for (const auto& pair : nodes_[eps_neighbour].edges) {
          if (pair.first == kEps) {
            continue;
          }

          node.edges[pair.first].insert(pair.second.begin(), pair.second.end());
        }
      }
    }

    // remove eps edges
    for (Node& node : nodes_) {
      node.edges.erase(kEps);
    }
  }
};

#endif
