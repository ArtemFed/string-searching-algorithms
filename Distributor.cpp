// АиСД-2, 2023, задание 5
// Федоров Артём Олегович БПИ217
// Clion для C++ и VS Code для python (.ipynb)
// Сделано: всё

#include "algorithms/SlowSearch.cpp"
#include "algorithms/StandardKMPSearch.cpp"
#include "algorithms/FastKMPSearch.cpp"
#include "algorithms/BMSearch.cpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <unordered_map>


using func = std::pair<size_t, size_t> (*)(const std::string &source, const std::string &pattern);

class Distributor {
    static std::pair<size_t, size_t> adapterSimpleSearch(const std::string &source, const std::string &pattern) {
        auto pair = simpleSearch(source, pattern);
        size_t count_ans = pair.first;
        size_t count_comparisons = pair.second;
        return {count_ans, count_comparisons};
    }

    static std::pair<size_t, size_t> adapterKMPSearch(const std::string &source, const std::string &pattern) {
        auto pair = standardKMPSearch(source, pattern);
        size_t count_ans = pair.first;
        size_t count_comparisons = pair.second;
        return {count_ans, count_comparisons};
    }

    static std::pair<size_t, size_t> adapterFastKMPSearch(const std::string &source, const std::string &pattern) {
        auto pair = fastKMPSearch(source, pattern);
        size_t count_ans = pair.first;
        size_t count_comparisons = pair.second;
        return {count_ans, count_comparisons};
    }

    static std::pair<size_t, size_t> adapterBMSearch(const std::string &source, const std::string &pattern) {
        auto pair = BMSearch(source, pattern);
        size_t count_ans = pair.first;
        size_t count_comparisons = pair.second;
        return {count_ans, count_comparisons};
    }


    void initializeMap() {
        map["slow"] = adapterSimpleSearch;
        map["kmp1"] = adapterKMPSearch;
        map["kmp2"] = adapterFastKMPSearch;
        map["bm"] = adapterBMSearch;

        func_names = std::vector<std::string>();
        func_names.reserve(map.size());
        for (auto const &item : map) {
            func_names.emplace_back(item.first);
        }
    }

    std::map<std::string, func> map = std::map<std::string, func>();
    std::vector<std::string> func_names;

public:
    Distributor() {
        initializeMap();
    }

    explicit Distributor(const std::map<std::string, func> &map) : map(map) {
        func_names = std::vector<std::string>(map.size());
        for (auto const &item : map) {
            func_names.push_back(item.first);
        }
    }

    func getFunc(const std::string &key) const {
        return map.at(key);
    }

    bool containsKey(const std::string &key) const {
        return map.find(key) != map.end();
    }

    friend std::ostream &operator<<(std::ostream &_stream, Distributor const &dist) {
        for (const auto &key : dist.func_names) {
            _stream << " - <" << key << ">" << std::endl;
        }
        return _stream;
    }

    std::vector<std::string> getNames() const {
        return func_names;
    }

    int getSize() const {
        return int(func_names.size());
    }

    void printTests(int num, const std::string &path) {
        std::string line, file_name;
        // Получаем пару { название функции, map с различными видами тестов }
        for (auto &func_item : tests) {

            // Получаем пару { название типа генерации паттерна, map с различными типами генерации текстов }
            for (auto &pattern_type_item : func_item.second) {

                // Получаем пару { название типа генерации текста, vector с различными замерами }
                for (auto &text_type_item : pattern_type_item.second) {
                    file_name = func_item.first + "_" + text_type_item.first + "_" + pattern_type_item.first + "_" + std::to_string(num);
                    std::cout << file_name << "\n";

                    std::ofstream file{path + file_name + ".csv"};
                    file << "text-size;pat-size;time;comparisons\n";

                    for (auto &testItem : text_type_item.second) {
                        file << testItem + "\n";
                    }
                    file << "\n";
                    file.close();
                }
            }
        }
    }

    void clearTests() {
        tests.clear();
    }

    // { name -> { type_pattern -> { type_string -> result } } }
    std::map<std::string, std::map<std::string, std::map<std::string, std::vector<std::string>>>> tests{};
};
