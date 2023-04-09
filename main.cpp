// Федоров Артём Олегович БПИ217
// Clion для C++ и VS Code для python (.ipynb)

#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#include "Distributor.cpp"
#include "Generator.cpp"

#define COUNT_OF_REPETITIONS 5
#define COUNT_OF_GENERATIONS 2
#define COUNT_OF_TEXTS 2
#define COUNT_OF_PATTERNS 4
#define RANDOM_SEED 33

using func = std::pair<std::vector<int>, size_t> (*)(const std::string &source, const std::string &pattern);

void testOneSortFile(std::pair<std::vector<int>, size_t>(algorithm_func)(const std::string &source, const std::string &pattern)) {
    auto pair_str_pat = Generator::getStringsFromFile("../files/input.txt");

    std::string source = pair_str_pat.first;
    std::string pattern = pair_str_pat.second;

    uint64_t sum_of_times = 0;

    std::pair<std::vector<int>, size_t> answer;
    for (int i = 0; i < COUNT_OF_REPETITIONS; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        answer = algorithm_func(source, pattern);
        auto diff = std::chrono::high_resolution_clock::now() - start;
        sum_of_times += std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
    }
    std::ofstream file{"../files/output.txt"};

    std::string out_param =
            "Text: " + source + "\n" +
            "Pattern: " + pattern + "\n" +
            "Answer size: " + std::to_string(answer.first.size()) + "\n";
    for (auto item : answer.first) {
        out_param += std::to_string(item) + ", ";
    }
    out_param += "\nComparisons: " + std::to_string(answer.second) + "\n";
    Generator::printStr(out_param, std::cout);

    out_param = std::to_string(answer.first.size()) + " " + std::to_string(answer.second) + "\n";
    Generator::printStr(out_param, file);

    file.close();

    std::cout << "Time:" << std::round(sum_of_times / COUNT_OF_REPETITIONS) << " us\n";
}


void distributeCommands() {
    Distributor distributor = Distributor();

    std::cout << "Available commands:\n";
    std::cout << distributor;
    std::cout << " - <end>\n";

    std::string line;
    for (;;) {
        std::cout << ">>";
        std::cin >> line;
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (line == "end") {
            break;
        }

        if (distributor.containsKey(line)) {
            testOneSortFile(distributor.getFunc(line));
        } else {
            continue;
        }
    }
}


void testAllAlgorithms(const int current_size,
                       const int pattern_size,
                       const std::vector<std::string> &vec_of_func_names,
                       Distributor &distributor,
                       Generator &generator) {

    // { name -> { type_pattern -> { type_string -> < sum of times, count of operations > } } }
    std::map<std::string,
             std::map<std::string,
                      std::map<std::string,
                               std::pair<double,
                                         double>>>>
            map_of_times{};

    // Выбираем вариант генерации строки
    for (int mode_text = 1; mode_text <= COUNT_OF_TEXTS; mode_text++) {
        std::string type_string;
        std::string source;

        // Выбираем вариант генерации Паттерна для поиска в строке
        for (int mode_pattern = 0; mode_pattern <= COUNT_OF_PATTERNS; ++mode_pattern) {
            std::string type_pattern;
            std::string pattern;

            for (int gen = 0; gen < COUNT_OF_GENERATIONS; ++gen) {
                // Вызываем все функции для тестирования на одной строке
                for (const auto &func_name : vec_of_func_names) {
                    // Генерирую случайную строку выбранным генератором
                    if (mode_text == 1) {
                        source = generator.getRandomText(
                                current_size, {'A', 'B'});
                        type_string = "binary";
                    } else {
                        source = generator.getRandomText(
                                current_size, {'A', 'C', 'T', 'G'});
                        type_string = "quad";
                    }

                    // Получаю паттерн по строке
                    pattern = generator.getSimplePattern(source, pattern_size);
                    if (mode_pattern == 0) {
                        type_pattern = "simple";
                    } else {
                        generator.addSymbolsPattern(source, pattern_size);
                        type_pattern = "advanced" + std::to_string(mode_pattern);
                    }

                    if (distributor.containsKey(func_name)) {
                        auto algorithm = distributor.getFunc(func_name);
                        auto pair = algorithm(source, pattern);
                        std::vector<int> answer = pair.first;
                        size_t count_of_operations = pair.second;

                        uint64_t sum_of_times = 0;

                        for (int j = 0; j < COUNT_OF_REPETITIONS; ++j) {
                            // Вызываю функцию сортировки из map
                            auto start = std::chrono::high_resolution_clock::now();
                            algorithm(source, pattern);
                            auto diff = std::chrono::high_resolution_clock::now() - start;

                            sum_of_times += std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
                        }
//                        if (func_name == "slow" && type_string == "binary") {
//                            std::cout << sum_of_times << " || " << (double(sum_of_times) / COUNT_OF_REPETITIONS) / COUNT_OF_GENERATIONS << "\n";
//                        }

                        map_of_times[func_name][type_pattern][type_string].first += (double(sum_of_times) / COUNT_OF_REPETITIONS) / COUNT_OF_GENERATIONS;
                        map_of_times[func_name][type_pattern][type_string].second += double(count_of_operations) / COUNT_OF_GENERATIONS;
                    }
                }
            }
        }
    }

    // { name -> { type_pattern -> { type_string -> pair< time, operations > } } }
    // Сохранение всех результатов тестов в map map'ов map'ов
    for (const auto &name_level : map_of_times) {

        for (const auto &pattern_type_level : name_level.second) {

            for (const auto &text_type_level : pattern_type_level.second) {

                distributor.tests[name_level.first][pattern_type_level.first][text_type_level.first].emplace_back(
                        std::to_string(current_size) + ";" +
                        std::to_string(pattern_size) + ";" +
                        std::to_string(text_type_level.second.first) + ";" +
                        std::to_string(text_type_level.second.second));
            }
        }
    }
}


void callAllFuncs() {
    Distributor distributor = Distributor();
    auto generator = Generator(RANDOM_SEED);

    std::vector<std::string> vec_of_func_names = distributor.getNames();

    std::map<std::string, std::ofstream> outStreams{};

    int text_size = 10000;
    for (int pattern_size = 100; pattern_size <= 3000; pattern_size += 100) {
        testAllAlgorithms(text_size,
                          pattern_size,
                          vec_of_func_names,
                          distributor,
                          generator);
    }

    distributor.printTests(text_size, R"(..\tests\size10000\)");

    std::cout << "==========================================\n";

    distributor.clearTests();

    int text_size_long = 100000;
    for (int pattern_size = 100; pattern_size <= 3000; pattern_size += 100) {
        testAllAlgorithms(text_size_long,
                          pattern_size,
                          vec_of_func_names,
                          distributor,
                          generator);
    }

    distributor.printTests(text_size_long, R"(..\tests\size100000\)");

    distributor.clearTests();
}


/**
 * Точка входа в программу
 * @return Код-результат работы программы
 */
int main() {
    std::cout << "Test one algorithm (from file) - 1 \nTest all algorithms - else\n";
    std::string line = "2";
    std::cin >> line;

    if (line != "1") {
        // 120 ms = 2 seconds
        auto start = std::chrono::high_resolution_clock::now();
        callAllFuncs();
        auto diff = std::chrono::high_resolution_clock::now() - start;
        std::cout << "Time:" << double(std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()) << " ms\n";
    } else {
        distributeCommands();
    }

    return 0;
}
