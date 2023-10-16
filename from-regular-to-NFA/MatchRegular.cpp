#include "functions.h"

bool MatchRegular(const std::string& regular, char chr, uint32_t count) {
  return ParseRegular(regular).FindSubword(chr, count);
}