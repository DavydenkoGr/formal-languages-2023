#include "earley/earley.h"

int main() {
  AlgorithmEarley earley = ReadGrammar();
  PredictRequests(earley);
}
