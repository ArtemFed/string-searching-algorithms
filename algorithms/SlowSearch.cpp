#include <string>

std::pair<size_t, size_t> simpleSearch(const std::string& source, const std::string& pattern) {
    int count_ans = 0;
    int count_comparisons = 0;

    for (int i = 0; i < source.length() - pattern.length() + 1; ++i) {
        int j = 0;
        int k = i;
        for (; j < pattern.length() && k < source.length(); ++j, ++k) {
            count_comparisons++;
            if (pattern[j] == '?') {
                continue;
            }
            if (source[k] != pattern[j]) {
                break;
            }
        }
        if (j == pattern.length()) {
            count_ans++;
        }
    }

    return {count_ans, count_comparisons};
}