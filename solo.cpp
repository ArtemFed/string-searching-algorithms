#include "consts.cpp"

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