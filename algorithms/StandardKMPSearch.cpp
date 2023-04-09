#include <string>
#include <vector>

void prefix_function(std::string pattern, std::vector<int> &pi) {
    int m = int(pattern.size());
    pi[0] = 0;

    int j = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[j]) {
            pi[i] = j;
            i++;
            j++;
        } else if (j != 0) {
            j = pi[j - 1];
        } else {
            pi[i] = 0;
            i++;
        }
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

    std::vector<int> pi(m, 0);
    prefix_function(pattern, pi);

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