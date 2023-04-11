// АиСД-2, 2023, КДЗ-2
// Федоров Артём Олегович БПИ217
// Windows: Clion для C++ и VS Code для python (.ipynb)

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
    std::vector<int> symbs_index;


public:
    explicit Generator(int seed) : random_seed(seed) {
        start_index = rand() % 6001;
        while (symbs_index.size() < 5) {
            int index = rand() % 100;
            if (std::find(symbs_index.begin(), symbs_index.end(), index) == symbs_index.end()) {
                symbs_index.emplace_back(index);
            }
        }
    }

    [[nodiscard]] std::string getSimplePattern(const std::string &str, int target_size) const {
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
        for (char symbol : alphabet) {
            std::string new_pat = pat;
            new_pat[indexes[index]] = symbol;
            addGeneratedPattern(new_pat, alphabet, index + 1);
        }
    }

    std::vector<std::string> insertAllSymbolsPattern(const std::string &pat, const std::vector<char> &alphabet) {
        patts = std::vector<std::string>();
        indexes = std::vector<int>();

        char ch = '?';
        int pos = -1;
        while ((pos = int(pat.find(ch, pos + 1))) != std::string::npos) {
            indexes.emplace_back(pos);
        }

        addGeneratedPattern(pat, alphabet, 0);
        return patts;
    }

    void addSymbolsPattern(std::string &pat, int symbols_count) {
        for (int i = 0; i < symbols_count; i++) {
            pat[symbs_index[symbols_count]] = '?';
        }
    }


    /**
     * Генератор строки по алфавиту
     * @param str_size Размер строки
     * @param alphabet Алфавит
     */
    [[nodiscard]] std::string getRandomText(int str_size, const std::vector<char> &alphabet) const {
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