#include "functions.h"

const std::set<char> kAlphabet = {'a', 'b', 'c', '1'};

NFA ParseRegular(const std::string& regular) {
  std::vector<NFA> stack;

  for (char chr : regular) {
    if (kAlphabet.count(chr)) {
      stack.push_back(
          NFA(2, 0, {1}, {{std::make_pair(chr, std::set<uint32_t>{1})}, {}})
      );

      continue;
    }

    switch (chr) {
      case '.':
        stack[stack.size() - 2].Concatenate(stack.back());
        stack.pop_back();
        continue;
      case '+':
        stack[stack.size() - 2].Add(stack.back());
        stack.pop_back();
        continue;
      default:;
        stack.back().Kleene();
        continue;
    }
  }

  return stack.back();
}
