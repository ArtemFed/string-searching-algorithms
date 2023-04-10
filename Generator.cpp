// АиСД-2, 2023, задание 5
// Федоров Артём Олегович БПИ217
// Clion для C++ и VS Code для python (.ipynb)
// Сделано: всё

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>

/**
 * Генератор строк
 */
class Generator {
    const int random_seed;
    int start_index;

public:
    explicit Generator(int seed) : random_seed(seed) {
        start_index = rand() % 6001;
    }

    std::string getSimplePattern(const std::string &str, int target_size) {
        int index = start_index % (int(str.size()) - target_size);
        std::string pat = str.substr(index, target_size);
        return pat;
    }


    std::vector<std::string> patts;
    std::vector<int> indexes;

    void addGeneratedPattern(const std::string &pat, const std::vector<char> &alphabet, int index) {
        if (index >= indexes.size()) {
            patts.emplace_back(pat);
            return;
        }
        for (char symb : alphabet) {
            std::string new_pat = pat;
            new_pat[indexes[index]] = symb;
            addGeneratedPattern(new_pat, alphabet, index + 1);
        }
    }

    std::vector<std::string> insertAllSymbolsPattern(const std::string &pat, const std::vector<char> &alphabet) {
        patts = std::vector<std::string>();
        indexes = std::vector<int>();

        char ch = '?';
        int pos = -1;
        while ((pos = int(pat.find(ch, pos + 1))) != string::npos) {
            indexes.emplace_back(pos);
        }

        addGeneratedPattern(pat, alphabet, 0);
        return patts;
    }

    void addSymbolsPattern(std::string &pat, int symbols_count) {
        for (int i = 0; i < symbols_count; i++) {
            int symbol_index = rand() % pat.size();
            pat[symbol_index] = '?';
        }
    }


    /**
     * Генератор строки по алфавиту
     * @param str_size Размер строки
     * @param alphabet Алфавит
     */
    std::string getRandomText(int str_size, const std::vector<char> &alphabet) const {
        std::string str;
        int alphabet_size = int(alphabet.size());
        srand(random_seed);
        for (int i = 0; i < str_size; ++i) {
            str += alphabet[rand() % alphabet_size];
        }
        return str;
    }


    /**
     * Прочить строки из файла по пути по строркам
     * @param filepath Путь к файлу от .exe
     * @return Пара из строки и шаблона
     */
    static std::pair<std::string, std::string> getStringsFromFile(const std::string &filepath) {
        try {
            std::ifstream infile(filepath);
            std::string line1;
            std::string line2;

            std::getline(infile, line1);
            std::getline(infile, line2);
            return {line1, line2};
        } catch (...) {
            return {"", ""};
        }
    }

    /**
     * Напечатать строку в поток
     * @param str Строка
     * @param so Поток
     */
    static void printStr(std::string &str, std::ostream &so) {
        so << str;
    }
};