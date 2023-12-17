#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#ifndef EARLEY_H
#define EARLEY_H

const char kEndl = '$';
const char kStart = '&';

struct Rule {
  char nonterminal;
  std::string derivation;
};

bool operator<(const Rule&, const Rule&);
bool operator==(const Rule&, const Rule&);

struct Situation {
  Rule rule;
  uint32_t previous_position;
  uint32_t next_index;

  Situation(char, const std::string&, uint32_t, uint32_t);
  Situation(const Rule&, uint32_t, uint32_t);
};

bool operator<(const Situation&, const Situation&);

struct Grammar {
  char start;
  std::vector<Rule> rules;

  Grammar();
  Grammar(char, const std::vector<Rule>&);
};

class AlgorithmEarley {
 public:
  void fit(const Grammar&);
  bool predict(const std::string&) const;

 private:
  Grammar grammar;

  void Scan(const std::string&, uint32_t,
            std::vector<std::map<char, std::set<Situation>>>&) const;
  bool Predict(uint32_t,
               std::vector<std::map<char, std::set<Situation>>>&) const;
  bool Complete(uint32_t,
                std::vector<std::map<char, std::set<Situation>>>&) const;
  bool Earley(const std::string&) const;
};

AlgorithmEarley ReadGrammar();
void PredictRequests(const AlgorithmEarley&);

#endif
