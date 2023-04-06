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

public:
    explicit Generator(int seed) : random_seed(seed) {
    }

    std::string getSimplePattern(const std::string &str, int target_size) {
        int index = rand() % (str.size() - target_size);
        std::string pat = str.substr(index, target_size);
        return pat;
    }

    std::string getAdvancedPattern(const std::string &str, int target_size) {
        std::string pat = getSimplePattern(str, target_size);

        int countOfSymbols = 1 + rand() % 4;
        for (int i = 0; i < countOfSymbols; i++) {
            int symbolIndex = rand() % pat.size();
            pat[symbolIndex] = '?';
        }

        return pat;
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