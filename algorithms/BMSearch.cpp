#include <string>
#include <iostream>
#include <vector>
using namespace std;

vector<int> calculateLPS(string pattern, size_t &count_od_comparisons) {
    int m = int(pattern.size());
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

std::pair<size_t, size_t> BMSearch(string source, string pattern) {
    int m = int(pattern.size());
    int n = int(source.size());
    if (m == 0 || m > n) {
        return {0, 0};
    }

    size_t count = 0;
    size_t count_of_comparisons = 0;
    vector<int> lps = calculateLPS(pattern, count_of_comparisons);

    int i = 0, j = 0;
    while (i < n) {
        count_of_comparisons++;
        if (pattern[j] == source[i]) {
            j++;
            i++;
        }
        if (j == m) {
            count++;
            j = lps[j - 1];
        } else {
            count_of_comparisons++;
            if (i < n && pattern[j] != source[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
    return {count, count_of_comparisons};
}

