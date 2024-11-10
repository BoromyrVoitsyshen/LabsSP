#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <vector>
#include <regex>

// Перевіряє, чи є слово зарезервованим
bool isKeyword(const std::string& str) {
    static const std::vector<std::string> keywords = {
        "if", "else", "while", "do", "break", "continue", "int", 
        "double", "float", "return", "string", "char", "case", "sizeof", 
        "long", "short", "typedef", "switch", "unsigned", "void", 
        "static", "struct", "goto"
    };
    return std::find(keywords.begin(), keywords.end(), str) != keywords.end();
}

// Перевіряє, чи є слово оператором
bool isOperator(const std::string& str) {
    static const std::vector<std::string> operators = {
        "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", "::",
        ">=", "<=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%="
    };
    return std::find(operators.begin(), operators.end(), str) != operators.end();
}

// Перевіряє, чи є символ розділовим знаком
bool isDelimiter(char ch) {
    return ch == ',' || ch == ';' || ch == '(' || ch == ')' || 
           ch == '{' || ch == '}' || ch == '[' || ch == ']';
}

bool isWhiteSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

// Перевіряє, чи є число десятковим, шістнадцятковим або з плаваючою крапкою
std::string getNextNumber(const std::string& str) {
    std::regex integerRegex("^\\d+$");
    std::regex floatRegex("^\\d*\\.\\d+$");
    std::regex hexRegex("^0[xX][0-9a-fA-F]+$");

    if (std::regex_match(str, integerRegex)) return "INTEGER NUMBER";
    if (std::regex_match(str, floatRegex)) return "FLOATING POINT NUMBER";
    if (std::regex_match(str, hexRegex)) return "HEXADECIMAL NUMBER";
    return "INVALID NUMBER";
}

// Перевірка рядкових та символьних констант
std::string getNextWord(const std::string& str) {
    std::regex stringRegex("^\".*\"$");
    std::regex charRegex("^'.'$");

    if (std::regex_match(str, stringRegex)) return "STRING CONSTANT";
    if (std::regex_match(str, charRegex)) return "CHARACTER CONSTANT";
    return "INVALID CONSTANT";
}

// Аналізує рядок вхідного коду
void parse(const std::string& str) {
    int len = str.length();
    int i = 0;

    while (i < len) {
        if (str[i] == '#') {  // Директива препроцесора
            std::string directive;
            while ((i < len) && !(str[i] == '/' && (i + 1 < len) && (str[i + 1] == '/' || str[i + 1] == '*')) && !(str[i] == '\n')) directive += str[i++];
            std::cout << "'" << directive << "' IS A PREPROCESSOR DIRECTIVE\n";
        } else if (str[i] == '/' && (i + 1 < len) && (str[i + 1] == '/' || str[i + 1] == '*')) {  // Коментарі
            if (str[i + 1] == '/') {  // Однорядковий коментар
                std::string comment;
                while (i < len && str[i] != '\n') comment += str[i++];
                std::cout << "'" << comment << "' IS A SINGLE-LINE COMMENT\n";
            } else {  // Багаторядковий коментар
                std::string comment = "/*";
                i += 2;
                while (i < len - 1 && !(str[i] == '*' && str[i + 1] == '/')) comment += str[i++];
                comment += "*/";
                i += 2;
                std::cout << "'" << comment << "' IS A MULTI-LINE COMMENT\n";
            }
        } else if (std::isalpha(str[i]) || str[i] == '_') {  // Ідентифікатор чи ключове слово
            std::string token;
            while (i < len && (std::isalnum(str[i]) || str[i] == '_')) token += str[i++];
            if (isKeyword(token)) {
                std::cout << "'" << token << "' IS A KEYWORD\n";
            } else {
                std::cout << "'" << token << "' IS AN IDENTIFIER\n";
            }
        } else if (std::isdigit(str[i])) {  // Число
            std::string number;
            while (i < len && (std::isalnum(str[i]) || str[i] == '.')) number += str[i++];
            std::cout << "'" << number << "' IS " << getNextNumber(number) << "\n";
        } else if (str[i] == '\"' || str[i] == '\'') {  // Рядкові та символьні константи
            std::string constant;
            char quoteType = str[i++];
            constant += quoteType;
            while (i < len && str[i] != quoteType) constant += str[i++];
            constant += str[i++];
            std::cout << "'" << constant << "' IS " << getNextWord(constant) << "\n";
        } else if (i + 1 < len && isOperator(str.substr(i, 2))) { // Перевірка на двосимвольний оператор
            std::string op = str.substr(i, 2);
            std::cout << "'" << op << "' IS AN OPERATOR\n";
            i += 2;
        } else if (isOperator(std::string(1, str[i]))) { // Перевірка на односимвольний оператор
            std::string op(1, str[i]);
            std::cout << "'" << op << "' IS AN OPERATOR\n";
            i++;
        } else if (isDelimiter(str[i])) {  // Розділові знаки
            std::cout << "'" << str[i++] << "' IS A DELIMITER\n";
        } else if(isWhiteSpace(str[i])) { // Пропуски
            i++;
        } else {  // Невпізнаний символ
            std::cout << "'" << str[i++] << "' IS AN UNRECOGNIZED SYMBOL\n";
        }
    }
}

// Головна функція
int main() {
    std::ifstream source;

    std::cout<<"Enter path to source file\n";
    std::string sourcePath;
    std::cin>>sourcePath;
    source.open(sourcePath);
    
    if(source.is_open()){
        std::string sourceCode;
        std::string tmp;
        while (std::getline(source, tmp) )
        {
            sourceCode += tmp;
            sourceCode += "\n";
        }
        source.close();
        parse(sourceCode);
    } else {
        std::cout<<"Can't open file\n";
    }

    return 0;
}