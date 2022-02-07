//
// Created by Prerthan Munireternam on 05/02/2022.
//

#ifndef SPA_TOKENISER_H
#define SPA_TOKENISER_H

#include <string>
#include <queue>
#include <sstream>
#include "Token.h"

class Tokeniser {
public:
    //constructor
    Tokeniser();

    //methods
    bool isNumber(const std::string& str);
    bool isAlphabet(const std::string& str);
    Token tokeniser(std::string input);
    //std::string tokenPrinter(Tokeniser t);
    std::queue<Token> putInQueue (std::string input);

};


#endif //SPA_TOKENISER_H