#include <string>
#include <vector>

void prefix_function(std::string pattern, std::vector<int> &pi, size_t &count_of_comparisons) {
    int m = int(pattern.size());

    pi[0] = 0;
    for (int i = 1; i < m; i++) {
        int j = pi[i - 1];
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
}

std::pair<std::vector<int>, size_t> standardKMPSearch(std::string source, std::string pattern) {
    std::vector<int> result = std::vector<int>();
    size_t count_of_comparisons = 0;
    int n = int(source.size());
    int m = int(pattern.size());

    if (m == 0 || m > n) {
        return {result, 0};
    }
    std::vector<int> pi(m);
    prefix_function(pattern, pi, count_of_comparisons);

    int i = 0;
    int j = 0;
    while (i < n) {
        count_of_comparisons++;
        if (source[i] == pattern[j]) {
            i++;
            j++;
        }
        if (j == m) {
            result.emplace_back(i - j);
            j = pi[j - 1];
        } else {
            count_of_comparisons++;
            if (i < n && source[i] != pattern[j]) {
                if (j != 0) {
                    j = pi[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
    return {result, count_of_comparisons};
}
