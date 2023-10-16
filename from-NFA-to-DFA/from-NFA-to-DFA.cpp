#include <queue>

#include "../eps-transitions/eps-transitions.h"

const uint32_t kAlphabetSize = 26;
const char kFirstCharacter = 'a';

class DFA {
 protected:
  struct Node {
    std::map<char, uint32_t> edges;
    bool terminal;
    Node() : terminal(false) {}
  };

  bool complete_ = false;
  // start: 0
  std::vector<Node> nodes_;

 public:
  explicit DFA(const NFA& nfa) {
    BuildDFA(nfa.start_, nfa.nodes_);
  }

  void CompleteDFA() {
    if (complete_) {
      return;
    }

    nodes_.emplace_back();
    for (Node& node : nodes_) {
      for (char chr = kFirstCharacter; chr < kFirstCharacter + kAlphabetSize; ++chr) {
        if (!node.edges.count(chr)) {
          node.edges[chr] = nodes_.size() - 1;
        }
      }
    }

    complete_ = true;
  }

  DFA GetComplement() {
    CompleteDFA();
    DFA complement = *this;

    for (Node& node : complement.nodes_) {
      node.terminal = !node.terminal;
    }

    return complement;
  }

 private:
  void BuildDFA(uint32_t nfa_start, const std::vector<NFA::Node>& nfa_nodes) {
    // sets of node index in new array, which connected with sets of NFA nodes
    std::map<std::set<uint32_t>, uint32_t> index_by_set;
    std::map<uint32_t, std::set<uint32_t>> set_by_index;

    bool terminal;
    uint32_t node_index;
    std::set<uint32_t> new_node;
    std::queue<uint32_t> queue;

    new_node.insert(nfa_start);
    index_by_set.insert(std::pair(new_node, 0));
    set_by_index.insert(std::pair(0, new_node));
    queue.push(0);

    nodes_.emplace_back();
    if (nfa_nodes[nfa_start].terminal) {
      nodes_[nodes_.size() - 1].terminal = true;
    }

    while (!queue.empty()) {
      node_index = queue.back();
      queue.pop();

      for (char chr = kFirstCharacter; chr < kFirstCharacter + kAlphabetSize; ++chr) {
        new_node.clear();
        terminal = false;

        for (uint32_t nfa_node : set_by_index[node_index]) {
          if (!nfa_nodes[nfa_node].edges.count(chr)) {
            continue;
          }

          new_node.insert(nfa_nodes[nfa_node].edges.at(chr).begin(),
                          nfa_nodes[nfa_node].edges.at(chr).end());
          if (nfa_nodes[nfa_node].terminal) {
            terminal = true;
          }
        }

        if (!index_by_set.count(new_node)) {
          index_by_set.insert(std::pair(new_node, nodes_.size()));
          set_by_index.insert(std::pair(nodes_.size(), new_node));
          queue.push(nodes_.size());

          nodes_.emplace_back();
          if (terminal) {
            nodes_[nodes_.size() - 1].terminal = true;
          }
        }
        nodes_[node_index].edges[chr] = index_by_set[new_node];
      }
    }
  }
};
