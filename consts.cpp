#define COUNT_OF_TEXTS 2
#define COUNT_OF_PATTERNS 4
#define COUNT_OF_GENERATIONS 3
#define COUNT_OF_REPETITIONS 20
#define RANDOM_SEED 42

using func = std::pair<std::vector<int>, size_t> (*)(const std::string &source, std::string &pattern);
