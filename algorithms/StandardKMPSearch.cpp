#include <string>
#include <vector>

using namespace std;

void prefix_function(string pattern, vector<int>& pi, size_t &count_of_comparisons) {
    int n = int(pattern.size());
    pi.reserve(n);
    pi[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j] && pattern[i] != '?' && pattern[j] != '?') {
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j] || pattern[i] == '?' || pattern[j] == '?') {
            j++;
        }
        pi[i] = j;
    }
}

std::pair<size_t, size_t>standardKMPSearch(string source, string pattern) {
    size_t answer = 0;
    size_t count_of_comparisons = 0;

    int n = int(source.size());
    int m = int(pattern.size());

    if (m > n) {
        return {answer, count_of_comparisons};
    }

    vector<int> pi;
    prefix_function(pattern, pi, count_of_comparisons);

    int i = 0;
    int j = 0;
    while (i < n) {
        count_of_comparisons++;
        if (source[i] == pattern[j] || pattern[j] == '?') {
            i++;
            j++;
        }
        if (j == m) {
            answer++;
            j = pi[j - 1];
        } else {
            count_of_comparisons++;
            if (i < n && source[i] != pattern[j] && pattern[j] != '?') {
                if (j != 0) {
                    j = pi[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
    return {answer, count_of_comparisons};
}