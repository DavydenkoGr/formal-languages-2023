#include <cstdint>
#include <iostream>
#include <vector>
#include <map>

const char kEps = '#';
const char kEndl = '$';
const char kStart = '&';
const char kGrammarStart = 'S';

struct Rule {
  char nonterminal;
  std::string derivation;
};

struct Situation {
  Rule rule;
  uint32_t previous_position;

  Situation() = default;

  Situation(char nonterminal, const std::string& derivation, uint32_t previous_position)
      : rule{nonterminal, derivation}, previous_position(previous_position) {}
};

void Scan(const std::vector<Rule>& grammar, const std::string& word, uint32_t position, std::vector<std::map<char, Situation>>& situations) {

}

bool Predict(const std::vector<Rule>& grammar, const std::string& word, uint32_t position, std::vector<std::map<char, Situation>>& situations) {

}

bool Complete(const std::vector<Rule>& grammar, const std::string& word, uint32_t position, std::vector<std::map<char, Situation>>& situations) {

}

bool Earley(std::vector<Rule> grammar, const std::string& word) {
  std::vector<std::map<char, Situation>> situations(word.length() + 1);
  // (S' -> *S, 0) in D0[S]
  situations[0][kGrammarStart] = Situation(kStart, {1, kGrammarStart}, 0);
  bool changes = true;
  while (changes) {
    changes = false;
    changes |= Predict(grammar, word, 0, situations);
    changes |= Complete(grammar, word, 0, situations);
  }

  for (uint32_t i = 1; i < word.length() + 1; ++i) {
    Scan(grammar, word, 0, situations);
    changes = true;
    while (changes) {
      changes = false;
      changes |= Predict(grammar, word, 0, situations);
      changes |= Complete(grammar, word, 0, situations);
    }
  }

  // TODO: check if (S' -> S*, len(word)) in Dlen(word)[S]
}

int main() {
  return 0;
}