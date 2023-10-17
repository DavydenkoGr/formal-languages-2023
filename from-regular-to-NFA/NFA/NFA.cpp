#include "NFA.h"

NFA::Node::Node() : terminal(false) {}

NFA::NFA(size_t size, uint32_t start, const std::vector<uint32_t>& terminal,
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

void NFA::MergeNodes(const NFA& nfa) {
  uint32_t size = nodes_.size();

  for (const Node& node : nfa.nodes_) {
    Node copy_node;

    for (char chr = 'a'; chr < 'a' + 26; ++chr) {
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

void NFA::Kleene() {
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
  nodes_[terminal_].edges[kEps].insert(start_);
}

void NFA::Concatenate(const NFA& nfa) {
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

void NFA::Add(const NFA& nfa) {
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

void NFA::RemoveEpsTransitions() {
  std::vector<std::set<uint32_t>> eps_achievable(nodes_.size());
  std::vector<std::set<uint32_t>> temp(nodes_.size());

  // zero iteration of transitive closure
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    eps_achievable[i].insert(i);
  }

  // others iterations
  for (uint32_t _ = 0; _ < nodes_.size(); ++_) {
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      temp[i].clear();

      for (uint32_t eps_neighbour : eps_achievable[i]) {
        temp[i].insert(nodes_[eps_neighbour].edges[kEps].begin(),
                       nodes_[eps_neighbour].edges[kEps].end());
      }
    }

    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      nodes_[i].edges[kEps].insert(temp[i].begin(), temp[i].end());
      eps_achievable[i] = temp[i];
    }
  }

  // new terminals
  for (Node& node : nodes_) {
    for (uint32_t eps_neighbour : node.edges[kEps]) {
      if (nodes_[eps_neighbour].terminal) {
        node.terminal = true;
        break;
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

void NFA::RebuildToSubwordNFA() {
  RemoveEpsTransitions();

  // New start node
  Node new_start;

  new_start.edges[kEps].insert(start_);

  start_ = nodes_.size();
  nodes_.push_back(new_start);

  // All nodes terminal + eps transition from new start node
  for (uint32_t i = 0; i < nodes_.size() - 1; ++i) {
    nodes_[start_].edges[kEps].insert(i);
    nodes_[i].terminal = true;
  }

  RemoveEpsTransitions();
}

bool NFA::FindSubword(char chr, uint32_t count) {
  std::vector<std::set<uint32_t>> achievable(nodes_.size());
  std::vector<std::set<uint32_t>> temp(nodes_.size());

  // zero iteration of transitive closure
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    achievable[i].insert(i);
  }

  // others iterations
  for (uint32_t _ = 0; _ < count; ++_) {
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      temp[i].clear();

      for (uint32_t chr_neighbour : achievable[i]) {
        temp[i].insert(nodes_[chr_neighbour].edges[chr].begin(),
                       nodes_[chr_neighbour].edges[chr].end());
      }
    }

    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      achievable[i] = temp[i];
    }
  }

  return !achievable[start_].empty();
}
