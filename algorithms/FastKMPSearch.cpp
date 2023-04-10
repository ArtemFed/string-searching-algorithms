#include <string>
#include <vector>


std::pair<std::vector<int>, size_t> fastKMPSearch(std::string source, std::string pattern) {
    std::vector<int> result = std::vector<int>();
    size_t count_of_comparisons = 0;

    int n = int(source.length());
    int m = int(pattern.length());

    if (m == 0 || m > n) {
        return {result, 0};
    }

    std::vector<int> pi(m);

    pi[0] = 0;
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            count_of_comparisons++;
            j = pi[j - 1];
        }
        count_of_comparisons++;
        if (pattern[i] == pattern[j]) {
            j++;
        }
        pi[i] = j;
    }

    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && source[i] != pattern[j]) {
            count_of_comparisons++;
            j = pi[j - 1];
        }
        count_of_comparisons++;
        if (source[i] == pattern[j]) {
            j++;
        }
        if (j == m) {
            result.emplace_back(i - m + 1);
            j = pi[j - 1];
        }
    }

    return {result, count_of_comparisons};
}
