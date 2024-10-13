#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

enum class TokenType {
    NUMBER,
    STRING_LITERAL,
    CHAR_LITERAL,
    PREPROCESSOR,
    COMMENT,
    KEYWORD,
    OPERATOR,
    DELIMITER,
    IDENTIFIER,
    UNKNOWN
};

struct Token {
    string value;
    TokenType type;
};

const vector<string> keywords = {"int", "return", "if", "else", "for", "while", "void"};

bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

TokenType identifyTokenType(const string& token) {
    if (regex_match(token, regex(R"(\d+(\.\d+)?([eE][+\-]?\d+)?)"))) return TokenType::NUMBER;
    if (regex_match(token, regex(R"(".*?")"))) return TokenType::STRING_LITERAL;
    if (regex_match(token, regex(R"('.*?')"))) return TokenType::CHAR_LITERAL;
    if (regex_match(token, regex(R"(#\w+)"))) return TokenType::PREPROCESSOR;
    if (regex_match(token, regex(R"//.*"))) return TokenType::COMMENT;  // Для однорядкових коментарів
    if (regex_match(token, regex(R"(/\*.*?\*/)", regex::icase))) return TokenType::COMMENT; // Для багаторядкових коментарів
    if (isKeyword(token)) return TokenType::KEYWORD;
    if (regex_match(token, regex(R"([\+\-\*\/=<>!&|]+)"))) return TokenType::OPERATOR;
    if (regex_match(token, regex(R"([{}()\[\];,])"))) return TokenType::DELIMITER;
    if (regex_match(token, regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) return TokenType::IDENTIFIER;

    return TokenType::UNKNOWN;
}

void analyzeCode(const string& code) {
    unordered_map<TokenType, vector<Token>> tokenTable;
    regex tokenRegex(R"(\s+|//.*|/\*.*?\*/|".*?"|'.*?'|[a-zA-Z_][a-zA-Z0-9_]*|\d+(\.\d+)?([eE][+\-]?\d+)?|[\+\-\*\/=<>!&|]+|[{}()\[\];,]|#\w+)");

    auto words_begin = sregex_iterator(code.begin(), code.end(), tokenRegex);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        string tokenValue = i->str();
        TokenType type = identifyTokenType(tokenValue);
        tokenTable[type].push_back({tokenValue, type});
    }

    for (const auto& pair : tokenTable) {
        cout << "Token Type: " << static_cast<int>(pair.first) << "\n";
        for (const Token& token : pair.second) {
            cout << "  Lexeme: " << token.value << "\n";
        }
    }
}

int main() {
    string sample_code =
        R"(#include <iostream>
        using namespace std;

        int main() {
            int x = 5; // Initialize
            cout << "Hello, World!" << endl; // Output
            return 0;
        })";

    analyzeCode(sample_code);
    return 0;
}
