#define COUNT_OF_REPETITIONS_2 1


void testAllAlgorithmsOnMyTest(const std::string &filepath,
                               std::string &pattern,
                               const std::vector<std::string> &vec_of_func_names,
                               Distributor &distributor) {

    std::map<std::string,
             std::pair<double,
                       double>>
            map_of_times{};

    std::string text = Generator::readFile(filepath);

    int main_answer = -1;
    // Вызываем все функции для тестирования на одной строке
    for (const auto &func_name : vec_of_func_names) {
        if (distributor.containsKey(func_name)) {
            auto algorithm = distributor.getFunc(func_name);

            uint64_t sum_of_times = 0;
            uint64_t sum_of_comparisons = 0;

            int answer;
            for (int j = 0; j < COUNT_OF_REPETITIONS_2; ++j) {

                // Вызываю функцию сортировки из map
                auto start = std::chrono::high_resolution_clock::now();
                auto ans_pair = algorithm(text, pattern);
                auto diff = std::chrono::high_resolution_clock::now() - start;

                sum_of_times += std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
                sum_of_comparisons += ans_pair.second;
                answer = int(ans_pair.first.size());
            }
            if (main_answer == -1) {
                // При первом проходе между всеми функциями вставляю ответ
                main_answer = answer;
            }
            // !!! Если ответы не совпадут, то будет выведена ошибка
            if (main_answer != answer) {
                std::cout << "ERROR!!! " + func_name << "\t" << answer << " != " << main_answer << "\n";
            }

            map_of_times[func_name].first += (double(sum_of_times) / COUNT_OF_REPETITIONS_2);
            map_of_times[func_name].second += (double(sum_of_comparisons) / COUNT_OF_REPETITIONS_2);

            std::cout << func_name << " ended,\tanswer = " << answer << "\n";
        }
    }

    std::cout << "Answer: " << main_answer << "\n";
    for (const auto &name_level : map_of_times) {
        std::cout << name_level.first << ":\t" << name_level.second.first << " us\n";
    }
}


void callAllFuncsOnMyTest() {
    Distributor distributor = Distributor();
    std::vector<std::string> vec_of_func_names = distributor.getNames();
    std::map<std::string, std::ofstream> outStreams{};

    std::string filepath = "..\\files/Tolstoy Leo. War and Peace.txt";
    std::string pattern = "Pierre";

    testAllAlgorithmsOnMyTest(filepath,
                              pattern,
                              vec_of_func_names,
                              distributor);
}