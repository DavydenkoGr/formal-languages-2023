#include <cstdint>
#include <iostream>
#include <vector>
#include <map>
#include <set>

const char kEps = '#';
const char kEndl = '$';
const char kStart = '&';

struct Rule {
  char nonterminal;
  std::string derivation;
};

bool operator<(const Rule& rule1, const Rule& rule2) {
  return rule1.nonterminal < rule2.nonterminal ||
         (rule1.nonterminal < rule2.nonterminal &&
          rule1.derivation < rule2.derivation);
}

bool operator==(const Rule& rule1, const Rule& rule2) {
  return !(rule1 < rule2 || rule2 < rule1);
}

class Situation {
 public:
  Rule rule;
  uint32_t previous_position;
  uint32_t next_index;

  Situation(char nonterminal, const std::string& derivation,
            uint32_t previous_position, uint32_t next_index)
      : rule{nonterminal, derivation}, previous_position(previous_position),
        next_index(next_index) {}

  Situation(const Rule& rule, uint32_t previous_position, uint32_t next_index)
      : rule{rule.nonterminal, rule.derivation}, previous_position(previous_position),
        next_index(next_index) {}
};

bool operator<(const Situation& situation1, const Situation& situation2) {
  return situation1.rule < situation2.rule ||
         (situation1.rule == situation2.rule &&
          situation1.previous_position < situation2.previous_position) ||
         (situation1.rule == situation2.rule &&
          situation1.previous_position == situation2.previous_position &&
          situation1.next_index < situation2.next_index);
}

class Grammar {
 public:
  char start;
  std::vector<Rule> rules;

  Grammar(char start, std::vector<Rule> rules) : start(start), rules(rules) {}
};


void Scan(const std::string& word, uint32_t position,
          std::vector<std::map<char, std::set<Situation>>>& situations) {
  for (const Situation& situation : situations[position][word[position]]) {
    uint32_t next_index = situation.next_index + 1;
    char next_chr = situation.rule.derivation[next_index];
    situations[position + 1][next_chr].insert(
        Situation(situation.rule.nonterminal, situation.rule.derivation, position, next_index)
    );
  }
}

void Predict(const Grammar& grammar, uint32_t position,
             std::vector<std::map<char, std::set<Situation>>>& situations) {
  for (char nonterminal = 'A'; nonterminal <= 'Z'; ++nonterminal) {
    for (const auto& _ : situations[position][nonterminal]) { // ???
      for (const Rule& rule : grammar.rules) {
        if (rule.nonterminal == nonterminal) {
          situations[position][rule.derivation[0]].insert(
              Situation(rule, position, 0)
          );
        }
      }
    }
  }
}

void Complete(uint32_t position,
              std::vector<std::map<char, std::set<Situation>>>& situations) {
  for (const Situation& situation : situations[position][kEndl]) {
    for (const Situation& previous_situation : situations[situation.previous_position][situation.rule.nonterminal]) {
      uint32_t next_index = previous_situation.next_index + 1;
      char next_chr = previous_situation.rule.derivation[next_index];
      situations[position][next_chr].insert(
          Situation(previous_situation.rule, position, next_index)
      );
    }
  }
}

bool Earley(const Grammar& grammar, const std::string& word) {
  std::vector<std::map<char, std::set<Situation>>> situations(word.length() + 1);
  // (S' -> *S, 0) in D0[S]
  situations[0][grammar.start].insert(
      Situation(kStart, std::string(1, grammar.start), 0, 0)
  );

  bool changes = true;
    while (changes) {
      uint32_t size = situations[0].size();
      Predict(grammar, 0, situations);
      Complete(0, situations);

      changes = false;
      if (situations[0].size() != size) {
        changes = true;
      }
    }

  for (uint32_t i = 0; i < word.length(); ++i) {
    Scan(word, i, situations);

    changes = true;
    while (changes) {
      uint32_t size = situations[i + 1].size();
      Predict(grammar, i + 1, situations);
      Complete(i + 1, situations);

      changes = false;
      if (situations[i + 1].size() != size) {
        changes = true;
      }
    }
  }

  // check if (S' -> S*, len(word)) in Dlen(word)[S]
  Situation result_situation(kStart, {1, grammar.start}, 0, word.length());
  return situations[word.length() - 1][kStart].count(result_situation);
}

int main() {
  return 0;
}
