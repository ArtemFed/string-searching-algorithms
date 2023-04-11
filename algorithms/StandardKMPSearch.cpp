#include <string>
#include <vector>

std::pair<std::vector<int>, size_t> standardKMPSearch(const std::string &source, const std::string &pattern) {
    std::vector<int> result = std::vector<int>();
    size_t count_of_comparisons = 0;

    int n = int(source.length());
    int m = int(pattern.length());

    if (m == 0 || m > n) {
        return {result, 0};
    }

    std::vector<int> br(m);

    br[0] = 0;
    for (int i = 1, j = 0; i < m; i++) {
        count_of_comparisons++;
        while (j > 0 && pattern[i] != pattern[j]) {
            count_of_comparisons++;
            j = br[j - 1];
        }
        count_of_comparisons++;
        if (pattern[i] == pattern[j]) {
            j++;
        }
        br[i] = j;
    }

    for (int i = 0, j = 0; i < n; i++) {
        count_of_comparisons++;
        while (j > 0 && source[i] != pattern[j]) {
            count_of_comparisons++;
            j = br[j - 1];
        }
        count_of_comparisons++;
        if (source[i] == pattern[j]) {
            j++;
        }
        if (j == m) {
            result.emplace_back(i - m + 1);
        }
    }

    return {result, count_of_comparisons};
}
