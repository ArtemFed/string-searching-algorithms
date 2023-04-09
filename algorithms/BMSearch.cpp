#include <cstring>
#include <string>
#include <vector>

using namespace std;

// Максимальный размер алфавита (ASCII)
#define ALPHABET_SIZE 256

// Функция для вычисления длины максимального суффикса pattern, являющегося суффиксом pattern[0..pos]
int suffix_len(std::string pattern, int pos) {
    int len = 0;
    for (int i = pos, j = pattern.length() - 1; i >= 0 && pattern[i] == pattern[j]; i--, j--) {
        len++;
    }
    return len;
}

// Функция для вывода индексов всех вхождений pattern в тексте source
std::pair<std::vector<int>, size_t> BMSearch(std::string text, std::string pattern) {
    int text_len = text.length();
    int pattern_len = pattern.length();

    size_t count_of_comparisons = 0;
    std::vector<int> result;

    // Создание таблицы смещений для каждого символа в pattern
    vector<int> bad_char(ALPHABET_SIZE, pattern_len);
    for (int i = 0; i < pattern_len - 1; i++) {
        bad_char[pattern[i]] = pattern_len - i - 1;
    }

    // Создание таблицы суффиксов для pattern
    vector<int> suffix(pattern_len, 0);
    int last_prefix = pattern_len;
    for (int i = pattern_len - 1; i >= 0; i--) {
        if (pattern[i] == pattern[last_prefix - 1]) {
            last_prefix = i;
        }
        suffix[pattern_len - i - 1] = last_prefix - i + pattern_len - 1;
    }
    for (int i = 0, j = 1; i < pattern_len - 1; i++, j++) {
        int len = suffix_len(pattern, i);
        suffix[len] = pattern_len - j + len;
    }

    // Поиск вхождений pattern в text
    int i = pattern_len - 1;
    while (i < text_len) {
        int j = pattern_len - 1;
        while (j >= 0 && text[i] == pattern[j]) {
            i--;
            j--;
        }
        if (j < 0) {
            result.emplace_back(i);
            i += pattern_len - suffix[0];
        }
        else {
            i += max(bad_char[(int)text[i]], suffix[pattern_len - j - 1]);
        }
    }
    return {result, count_of_comparisons};
}


std::pair<std::vector<int>, size_t> BM1Search(std::string text, std::string pattern) {
    int n = int(text.size());
    int m = int(pattern.size());
    size_t count_of_comparisons = 0;
    std::vector<int> result;

    if (m == 0 || m > n) {
        return {result, 0};
    }

    int i = m - 1, j = m - 1;
    while (i < n) {
        if (text[i] == pattern[j] || pattern[j] == '?') {
            if (j == 0) {
                result.push_back(i);
                i += m - std::min(j, 1 + j);
                j = m - 1;
            } else {
                --i;
                --j;
            }
        } else {
            int k = i;
            while (k < n && text[k] != pattern[j] && pattern[j] != '?') {
                k += m - std::min(j, 1 + j - k);
            }
            i = k;
            j = m - 1;
        }
    }

    return {result, count_of_comparisons};
}
