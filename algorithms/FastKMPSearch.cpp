#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::pair<size_t, size_t> fastKMPSearch(std::string source, string pattern) {
    int n = int(source.length());
    int m = int(pattern.length());
    std::vector<int> pi;
    pi.reserve(m);
    size_t count_of_comparisons = 0;

    pi[0] = 0;
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        pi[i] = j;
    }

    std::vector<int> matches;
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
            matches.emplace_back(i - m + 1);
            j = pi[j - 1];
        }
    }

    return {matches.size(), count_of_comparisons};
}
