// АиСД-2, 2023, КДЗ-2
// Федоров Артём Олегович БПИ217
// Windows: Clion для C++ и VS Code для python (.ipynb)

#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include "Distributor.cpp"
#include "Generator.cpp"

#define COUNT_OF_TEXTS 2
#define COUNT_OF_PATTERNS 4
#define COUNT_OF_GENERATIONS 3
#define COUNT_OF_REPETITIONS 20
#define RANDOM_SEED 42


using func = std::pair<std::vector<int>, size_t> (*)(const std::string &source, std::string &pattern);

void testOneAlgorithmFile(std::pair<std::vector<int>, size_t>(algorithm_func)(const std::string &source, std::string &pattern)) {
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

    std::cout << "Time:" << std::round(sum_of_times / COUNT_OF_REPETITIONS) << " nanoseconds\n";
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
            testOneAlgorithmFile(distributor.getFunc(line));
        } else {
            continue;
        }
    }
}


void testAllAlgorithmsOnOneSize(const int current_size,
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
        std::string type_text;
        std::string source;

        // Выбираем вариант генерации Паттерна для поиска в строке
        for (int mode_pattern = 0; mode_pattern <= COUNT_OF_PATTERNS; ++mode_pattern) {
            std::string type_pattern;
            std::string pattern;

            // Генерируем строку COUNT_OF_GENERATIONS раз
            for (int gen = 0; gen < COUNT_OF_GENERATIONS; ++gen) {
                // Генерирую случайную строку выбранным генератором
                if (mode_text == 1) {
                    source = generator.getRandomText(current_size, {'A', 'B'});
                    type_text = "binary";
                } else {
                    source = generator.getRandomText(current_size, {'A', 'C', 'T', 'G'});
                    type_text = "quad";
                }

                std::vector<std::string> patterns;

                // Получаю паттерн(ы) по строке
                pattern = generator.getSimplePattern(source, pattern_size);
                if (mode_pattern == 0) {
                    patterns = {pattern};
                    type_pattern = "simple";
                } else {
                    // advanced паттерн 1 - 4
                    // Добавляем в паттерн символы "?"
                    generator.addSymbolsPattern(pattern, mode_pattern);

                    if (type_text == "binary") {
                        patterns = generator.insertAllSymbolsPattern(pattern, {'A', 'B'});
                    } else {
                        patterns = generator.insertAllSymbolsPattern(pattern, {'A', 'C', 'T', 'G'});
                    }

                    type_pattern = "advanced" + std::to_string(mode_pattern);
                }

                int main_answer = -1;

                // Вызываем все функции для тестирования на одной строке
                for (const auto &func_name : vec_of_func_names) {
                    if (distributor.containsKey(func_name)) {
                        auto algorithm = distributor.getFunc(func_name);

                        uint64_t sum_of_times = 0;
                        uint64_t sum_of_comparisons = 0;
                        int answer;
                        for (int j = 0; j < COUNT_OF_REPETITIONS; ++j) {
                            answer = 0;
                            for (auto &cur_pattern : patterns) {
                                // Вызываю функцию сортировки из map
                                auto start = std::chrono::high_resolution_clock::now();
                                auto ans_pair = algorithm(source, cur_pattern);
                                auto diff = std::chrono::high_resolution_clock::now() - start;

                                sum_of_times += std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
                                sum_of_comparisons += ans_pair.second;
                                answer += int(ans_pair.first.size());
                            }
                        }
                        if (main_answer == -1) {
                            // При первом проходе между всеми функциями вставляю ответ
                            main_answer = answer;
                        }
                        // !!! Если ответы не совпадут, то будет выведена ошибка
                        if (main_answer != answer) {
                            std::cout << "ERROR!!! " + func_name << "\t" << type_pattern << "\t" << type_text << "\t= " << answer << "!=" << main_answer << "\n";
                        }
                        if (gen == 0) {
                            // Вывожу информацию только при первой генерации строк в подходе.
                            //                            std::cout << "text=" << current_size << " pattern=" << pattern_size << " | " << func_name << "\t" << type_pattern << "\t" << type_text << "\t= " << answer << "\n";
                        }

                        map_of_times[func_name][type_pattern][type_text].first += (double(sum_of_times) / COUNT_OF_REPETITIONS) / COUNT_OF_GENERATIONS;
                        map_of_times[func_name][type_pattern][type_text].second += (double(sum_of_comparisons) / COUNT_OF_REPETITIONS) / COUNT_OF_GENERATIONS;
                    }
                }

                //                std::cout << "\n";
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

    std::cout << std::round(pattern_size / 3000.0 * 100)
              << "% / 100 %"
              << "\n";
}


void callAllFuncs() {
    Distributor distributor = Distributor();
    auto generator = Generator(RANDOM_SEED);
    std::vector<std::string> vec_of_func_names = distributor.getNames();
    std::map<std::string, std::ofstream> outStreams{};

    int text_size = 10000;
    std::cout << "size=" << text_size << " started\n";
    for (int pattern_size = 100; pattern_size <= 3000; pattern_size += 100) {
        testAllAlgorithmsOnOneSize(text_size,
                                   pattern_size,
                                   vec_of_func_names,
                                   distributor,
                                   generator);
    }
    distributor.printTests(text_size, R"(..\tests\size10000\)");
    distributor.clearTests();
    std::cout << "size=" << text_size << " finished\n";


    text_size = 100000;
    std::cout << "size=" << text_size << " started\n";
    for (int pattern_size = 100; pattern_size <= 3000; pattern_size += 100) {
        testAllAlgorithmsOnOneSize(text_size,
                                   pattern_size,
                                   vec_of_func_names,
                                   distributor,
                                   generator);
    }
    distributor.printTests(text_size, R"(..\tests\size100000\)");
    distributor.clearTests();
    std::cout << "size=" << text_size << " finished\n";
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
        auto ms = double(std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
        std::cout << "Time:" << ms << " ms == " << ms / 1000 / 60 << "m\n";
    } else {
        distributeCommands();
    }

    return 0;
}
