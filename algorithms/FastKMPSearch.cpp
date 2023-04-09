#include <string>
#include <vector>

void prefix_function_fast(std::vector<int>& pi, std::vector<int>& pi2) {
    int m = int(pi.size());
    for (size_t i = 0; i < m; ++i) {
        if (i + 1 == m || pi[i] >= pi[i + 1]) {
            pi2[i] = pi[i];
        }
    }
}

std::pair<std::vector<int>, size_t> fastKMPSearch(std::string source, std::string pattern) {
    std::vector<int> result = std::vector<int>();

    size_t count_of_comparisons = 0;

    int n = int(source.size());
    int m = int(pattern.size());

    if (m == 0 || m > n) {
        return {result, 0};
    }

    std::vector<int> pi(m, 0);
    prefix_function(pattern, pi);
    std::vector<int> pi2(m, 0);
    prefix_function_fast(pi, pi2);

    int i = 0;
    int j = 0;
    while (i < n) {
        count_of_comparisons++;
        if (source[i] == pattern[j]) {
            j++;
            i++;
        }

        if (j == m) {
            result.emplace_back(i - j);
            j = pi2[j - 1];
        } else {
            count_of_comparisons++;
            if (i < n && source[i] != pattern[j]) {
                if (j != 0) {
                    j = pi2[j - 1];
                } else {
                    i++;
                }
            }
        }
    }

    return {result, count_of_comparisons};
}
