#include "functions.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    return 1;
  }

  NFA regular_nfa = ParseRegular(argv[1]);

  regular_nfa.RemoveEpsTransitions();
  regular_nfa.RebuildToSubwordNFA();

  std::cout << regular_nfa.FindSubword(argv[2][0], atoi(argv[3])) << '\n';
}
