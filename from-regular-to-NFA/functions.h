#include <cstdint>
#include <iostream>
#include <set>
#include <vector>

#include "NFA.h"

NFA ParseRegular(const std::string& regular);

bool MatchRegular(const std::string regular, char chr, uint32_t count);