#include <cstring>
#include <string>
#include <vector>

// Размер алфавита
#define ALPHABET_SIZE 256

std::pair<std::vector<int>, size_t> RKSearch(std::string text, std::string pattern) {
    int n = int(text.size());
    int m = int(pattern.size());
    size_t count_of_comparisons = 0;
    std::vector<int> result;

    if (m == 0 || m > n) {
        return {result, 0};
    }

    int p_len = int(pattern.length());
    int t_len = int(text.length());

    // Вычисляем хеш для подстроки и первого окна текста
    int p_hash = 0;
    int t_hash = 0;
    int h = 1;
    int prime = 7793;// простое число для хеширования
    for (int i = 0; i < p_len; i++) {
        p_hash = (ALPHABET_SIZE * p_hash + pattern[i]) % prime;
        t_hash = (ALPHABET_SIZE * t_hash + text[i]) % prime;
    }

    // Вычисляем h = ALPHABET_SIZE^(p_len-1)
    for (int i = 0; i < p_len - 1; i++) {
        h = (h * ALPHABET_SIZE) % prime;
    }

    // Перебираем все окна в тексте
    for (int i = 0; i <= t_len - p_len; i++) {
        // Проверяем совпадение хешей текущего окна и подстроки
        count_of_comparisons++;
        if (p_hash == t_hash) {
            // Если хеши совпадают, проверяем символы по одному
            int j;
            for (j = 0; j < p_len; j++) {
                count_of_comparisons++;
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            // Если все символы совпали, печатаем позицию
            if (j == p_len) {
                result.emplace_back(i);
            }
        }

        // Вычисляем хеш следующего окна
        if (i < t_len - p_len) {
            t_hash = (ALPHABET_SIZE * (t_hash - text[i] * h) + text[i + p_len]) % prime;
            if (t_hash < 0) {
                t_hash = (t_hash + prime);
            }
        }
    }

    return {result, count_of_comparisons};
}
