#include <cstring>
#include <string>
#include <vector>

using namespace std;

#define NO_OF_CHARS 256

void badCharHeuristic(string str, int size, std::vector<int> badchar) {
    int i;
    for (i = 0; i < NO_OF_CHARS; i++) {
        badchar[i] = -1;
    }
    for (i = 0; i < size; i++) {
        badchar[(int) str[i]] = i;
    }
}

std::pair<std::vector<int>, size_t> BMSearch(std::string text, std::string pattern) {
    size_t count_of_comparisons = 0;
    std::vector<int> result;

    int m = pattern.size();
    int n = text.size();

    std::vector<int> badchar(NO_OF_CHARS);

    badCharHeuristic(pattern, m, badchar);

    int s = 0;
    while (s <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j]) {
            j--;
        }

        if (j < 0) {
            result.emplace_back(s);
            s += (s + m < n) ? m - badchar[text[s + m]] : 1;
        } else {
            s += max(1, j - badchar[text[s + j]]);
        }
    }
    return {result, count_of_comparisons};
}

std::pair<std::vector<int>, size_t> BM2Search(std::string text, std::string pattern) {
    int n = int(text.size());
    int m = int(pattern.size());
    size_t count_of_comparisons = 0;
    std::vector<int> result;

    if (m == 0 || m > n) {
        return {result, 0};
    }

    // Инициализация таблицы несоответствия
    vector<int> bad_char(256, m);
    for (int i = 0; i < m - 1; i++) {
        bad_char[pattern[i]] = m - i - 1;
    }

    // Вычисление таблицы суффиксов
    vector<int> suffix(m + 1), good_suffix(m + 1);
    suffix[m] = m + 1;
    int j = m + 1;
    for (int i = m; i >= 0; i--) {
        if (i == 0 || suffix[i] == i + 1) {
            while (j <= m && pattern[j - 1] != pattern[i - 1]) {
                count_of_comparisons++;
                if (good_suffix[j] == 0) {
                    good_suffix[j] = j - i;
                }
                j = suffix[j];
            }
            suffix[i] = j--;
        }
    }

    // Заполнение оставшихся ячеек таблицы суффиксов
    j = suffix[0];
    for (int i = 0; i <= m; i++) {
        if (good_suffix[i] == 0) {
            good_suffix[i] = j;
        }
        if (i == j) {
            j = suffix[j];
        }
    }

    // Поиск подстроки
    int i = m - 1;
    j = m - 1;
    while (i < n) {
        count_of_comparisons++;
        if (pattern[j] == text[i]) {
            if (j == 0) {
                result.push_back(i);
                i++;
                j = m - 1;
            } else {
                i--;
                j--;
            }
        } else {
            i += max(good_suffix[j], bad_char[text[i]]);
            j = m - 1;
        }
    }

    return {result, count_of_comparisons};
}
