#include <string>

std::pair<std::vector<int>, size_t> slowSearch(const std::string& source, const std::string& pattern) {
    int n = int(source.size());
    int m = int(pattern.size());
    std::vector<int> result = std::vector<int>();

    int count_of_comparisons = 0;

    for (int i = 0; i < n - m + 1; ++i) {
        int j = 0;
        for (; j < m && i + j < n; ++j) {
            count_of_comparisons++;
            if (pattern[j] == '?') {
                continue;
            }
            count_of_comparisons++;
            if (source[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            result.emplace_back(i);
        }
    }

    return {result, count_of_comparisons};
}