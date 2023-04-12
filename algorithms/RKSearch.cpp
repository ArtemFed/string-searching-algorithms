#include <string>
#include <vector>

// Размер алфавита
#define ALPHABET_SIZE_RK 256

std::pair<std::vector<int>, size_t> RKSearch(const std::string &source, const std::string &pattern) {
    int n = int(source.size());
    int m = int(pattern.size());
    size_t count_of_comparisons = 0;
    std::vector<int> result;

    if (m == 0 || m > n) {
        return {result, 0};
    }

    // Вычисляем хеш для подстроки и первого окна текста
    int pat_hash = 0;
    int src_hash = 0;
    int h = 1;
    int prime = 9883;// простое число для хеширования
    for (int i = 0; i < m; i++) {
        pat_hash = (ALPHABET_SIZE_RK * pat_hash + pattern[i]) % prime;
        src_hash = (ALPHABET_SIZE_RK * src_hash + source[i]) % prime;
    }

    // Вычисляем h = ALPHABET_SIZE^(m-1)
    for (int i = 0; i < m - 1; i++) {
        h = (h * ALPHABET_SIZE_RK) % prime;
    }

    // Перебираем все окна в тексте
    for (int i = 0; i <= n - m; i++) {
        // Проверяем совпадение хешей текущего окна и подстроки
        count_of_comparisons++;
        if (pat_hash == src_hash) {
            // Если хеши совпадают, проверяем символы по одному
            int j;
            for (j = 0; j < m; j++) {
                count_of_comparisons++;
                if (source[i + j] != pattern[j]) {
                    break;
                }
            }
            // Если все символы совпали, печатаем позицию
            if (j == m) {
                result.emplace_back(i);
            }
        }

        // Вычисляем хеш следующего окна
        if (i < n - m) {
            src_hash = (ALPHABET_SIZE_RK * (src_hash - source[i] * h) + source[i + m]) % prime;
            if (src_hash < 0) {
                src_hash = (src_hash + prime);
            }
        }
    }

    return {result, count_of_comparisons};
}
