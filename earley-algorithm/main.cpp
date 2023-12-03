#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <vector>

const char kEndl = '$';
const char kStart = '&';

struct Rule {
  char nonterminal;
  std::string derivation;
};

bool operator<(const Rule& rule1, const Rule& rule2) {
  return rule1.nonterminal < rule2.nonterminal ||
         (rule1.nonterminal == rule2.nonterminal &&
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
      : rule(rule), previous_position(previous_position), next_index(next_index) {}
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

  Grammar() = default;

  Grammar(char start, const std::vector<Rule>& rules)
      : start(start), rules(rules) {}
};

void Scan(const std::string& word, uint32_t position,
          std::vector<std::map<char, std::set<Situation>>>& situations) {
  for (const Situation& situation : situations[position][word[position]]) {
    uint32_t next_index = situation.next_index + 1;
    char next_chr = situation.rule.derivation[next_index];
    situations[position + 1][next_chr].insert(
        Situation(situation.rule, situation.previous_position, next_index)
    );
  }
}

bool Predict(const Grammar& grammar, uint32_t position,
             std::vector<std::map<char, std::set<Situation>>>& situations) {
  bool changes = false;

  if (!situations[position][kStart].empty()) {
    for (const Rule& rule : grammar.rules) {
      if (rule.nonterminal == kStart) {
        Situation new_situation(rule, position, 0);
        if (situations[position][rule.derivation[0]].count(new_situation)) {
          continue;
        }

        changes = true;
        situations[position][rule.derivation[0]].insert(new_situation);
      }
    }
  }

  for (char nonterminal = 'A'; nonterminal <= 'Z'; ++nonterminal) {
    if (!situations[position][nonterminal].empty()) {
      for (const Rule& rule : grammar.rules) {
        if (rule.nonterminal == nonterminal) {
          Situation new_situation(rule, position, 0);

          if (situations[position][rule.derivation[0]].count(new_situation)) {
            continue;
          }

          changes = true;
          situations[position][rule.derivation[0]].insert(new_situation);
        }
      }
    }
  }

  return changes;
}

bool Complete(uint32_t position,
              std::vector<std::map<char, std::set<Situation>>>& situations) {
  bool changes = false;

  for (const Situation& situation : situations[position][kEndl]) {
    for (const Situation& previous_situation
         : situations[situation.previous_position][situation.rule.nonterminal]) {
      uint32_t next_index = previous_situation.next_index + 1;
      char next_chr = previous_situation.rule.derivation[next_index];

      Situation new_situation(
          previous_situation.rule, previous_situation.previous_position, next_index
      );

      if (situations[position][next_chr].count(new_situation)) {
        continue;
      }

      changes = true;
      situations[position][next_chr].insert(new_situation);
    }
  }

  return changes;
}

bool Earley(const Grammar& grammar, const std::string& word) {
  std::vector<std::map<char, std::set<Situation>>> situations(word.length() + 1);
  // put (S' -> *S, 0) in D0[S']
  situations[0][grammar.start].insert(
      Situation(grammar.rules.back(), 0, 0)
  );

  bool changes = true;
  while (changes) {
    changes = false;
    changes |= Predict(grammar, 0, situations);
    changes |= Complete(0, situations);
  }

  for (uint32_t i = 0; i < word.length(); ++i) {
    Scan(word, i, situations);

    changes = true;
    while (changes) {
      changes = false;
      changes |= Predict(grammar, i + 1, situations);
      changes |= Complete(i + 1, situations);
    }
  }

  // check if (S' -> S*, len(word)) in Dlen(word)[S']
  Situation result_situation(
      grammar.rules.back(), 0, 1
  );

  return situations[word.length()][kEndl].count(result_situation);
}

class AlgorithmEarley {
 public:
  Grammar grammar;

  void fit(const Grammar& new_grammar) {
    grammar.start = new_grammar.start;

    grammar.rules.clear();
    for (const Rule& rule : new_grammar.rules) {
      grammar.rules.emplace_back(
          Rule{rule.nonterminal, rule.derivation + kEndl}
      );
    }

    grammar.rules.push_back(
        Rule{kStart, std::string(1, grammar.start) + kEndl}
    );
  }

  bool predict(const std::string& word) const {
    return Earley(grammar, word);
  }
};

int main() {
  uint32_t nonterminal_count;
  uint32_t terminal_count;
  uint32_t rules_count;
  std::cin >> nonterminal_count >> terminal_count >> rules_count;

  std::string nonterminals;
  std::cin >> nonterminals;
  for (char nonterminal : nonterminals) {
    if (!('A' <= nonterminal && nonterminal <= 'Z')) {
      throw std::runtime_error("Incorrect nonterminal");
    }
  }

  std::string terminals;
  std::cin >> terminals;
  for (char terminal : terminals) {
    if (('A' <= terminal && terminal <= 'Z') ||
        terminal == kEndl || terminal == kStart) {
      throw std::runtime_error("Incorrect terminal");
    }
  }

  Rule configured_rule;
  std::vector<Rule> rules;
  char rule_nonterminal;
  std::string space;
  std::string derivation;
  for (uint32_t i = 0; i < rules_count; ++i) {
    std::cin >> rule_nonterminal >> space;
    if (std::cin.peek() == '\n') {
      derivation = "";
    } else {
      std::cin >> derivation;
    }

    if (!('A' <= rule_nonterminal && rule_nonterminal <= 'Z')) {
      throw std::runtime_error("Incorrect nonterminal");
    }

    for (char chr : derivation) {
      if (chr == kEndl || chr == kStart) {
        throw std::runtime_error("Incorrect derivation");
      }
    }

    configured_rule = {rule_nonterminal, derivation};
    rules.push_back(configured_rule);
  }

  char start;
  std::cin >> start;
  if (!('A' <= start && start <= 'Z')) {
    throw std::runtime_error("Incorrect start");
  }

  Grammar grammar(start, rules);
  AlgorithmEarley earley;
  earley.fit(grammar);

  uint32_t request_count;
  std::string word;
  std::cin >> request_count;
  for (uint32_t i = 0; i < request_count; ++i) {
    std::cin >> word;

    for (char chr : word) {
      if (('A' <= chr && chr <= 'Z') ||
          chr == kEndl || chr == kStart) {
        throw std::runtime_error("Incorrect word");
      }
    }

    std::cout << (earley.predict(word) ? "YES" : "NO") << '\n';
  }
}
