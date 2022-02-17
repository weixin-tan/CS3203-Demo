//
// Created by Prerthan Munireternam on 05/02/2022.
//

#include "Tokeniser.h"


Tokeniser::Tokeniser() {

}

bool Tokeniser::isNumber(const std::string &str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

bool Tokeniser::isAlphabet(const std::string &str) {
    for (char const &c : str) {
        if (std::isalpha(c) == 0) return false;
    }
    return true;
}

Token Tokeniser::tokeniser(std::string input) {
    if(input.length()==1){
        if(isNumber(input)){
            Token t = Token(TokenType::DIGIT, input);
            return t;
        } else if(isAlphabet(input)){
            Token t = Token(TokenType::LETTER, input);
            return t;
        } else if (input == ";"){
            Token t = Token(TokenType::SEMICOLON, input);
            return t;
        } else if (input == "}") {
            Token t = Token(TokenType::RIGHT_CURLY, input);
            return t;
        } else if (input == "{") {
            Token t = Token(TokenType::LEFT_CURLY, input);
            return t;
        } else {
            Token t = Token(TokenType::SPECIALCHAR, input);
            return t;
        }
    } else {
        Token t = Token(TokenType::NAME, input);
        return t;
    }
}

std::queue<Token> Tokeniser::putInQueue(std::string input) {
    std::queue<Token> tqueue;
    std::stringstream checker(input);
    std::string temp;

    while(getline(checker, temp, ' ')){
        Token t = tokeniser(temp);
        tqueue.push(t);
    }
    return tqueue;
}

/*
std::string Tokeniser::tokenPrinter(Tokeniser t) {
    cout << "(" << t.getType() << " , " << t.getId() << ")"<< endl;
} */


