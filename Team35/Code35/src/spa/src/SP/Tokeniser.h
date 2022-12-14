#ifndef SPA_TOKENISER_H
#define SPA_TOKENISER_H

#include <string>
#include <set>
#include <queue>
#include <sstream>
#include <iostream>
#include <regex>
#include "Token.h"
#include "TokenType.h"

class Tokeniser {
public:
    //constructor
    Tokeniser();

    bool forbiddenFlag;

    //methods
    bool isNumber(const std::string& str);
    bool isAlphabet(const std::string& str);
    bool isName(const std::string& str);
    Token tokeniser(std::string input);
    //std::string tokenPrinter(Tokeniser t);
    std::queue<Token> putInQueue(std::string input);
    std::string printToken(Token token);
    std::string lTrim(std::string s);
    std::string rTrim(std::string s);
    std::string trim(std::string s);
    std::string addSpace(std::string s);
    std::queue<Token> forbiddenWord(std::queue<Token> inputQueue);

};

#endif //SPA_TOKENISER_H
