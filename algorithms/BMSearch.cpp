#include <string>
#include <vector>


#define ALPHABET_SIZE 256

void preprocessBadChar(const std::string &pattern, std::vector<int> &badChar) {
    int m = int(pattern.size());

    for (int i = 0; i < m; i++) {
        badChar[pattern[i]] = i;
    }
}

std::pair<std::vector<int>, size_t> BMSearch(std::string source, std::string pattern) {
    int n = int(source.size());
    int m = int(pattern.size());
    size_t count_of_comparisons = 0;
    std::vector<int> result;
    std::vector<int> badChar(ALPHABET_SIZE, -1);

    preprocessBadChar(pattern, badChar);

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;

        count_of_comparisons++;
        while (j >= 0 && pattern[j] == source[s + j]) {
            count_of_comparisons++;
            j--;
        }

        if (j < 0) {
            result.push_back(s);

            if (s + m < n) {
                s += m - badChar[source[s + m]];
            } else {
                s++;
            }
        } else {
            s += std::max(1, j - badChar[source[s + j]]);
        }
    }

    return {result, count_of_comparisons};
}
