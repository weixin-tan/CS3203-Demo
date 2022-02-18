//
// Created by Prerthan Munireternam on 05/02/2022.
//

#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>
#include "TokenType.h"

//enum
//enum TokenType{LETTER, DIGIT, NAME,  SPECIALCHAR, SEMICOLON, RIGHT_CURLY, LEFT_CURLY, READ, PRINT, CALL, WHILE, IF,
//        PROCEDURE, THEN, ELSE};

inline const char* ToString(TokenType v)
{
    switch (v)
    {
        case TokenType::LETTER:   return "LETTER";
        case TokenType::DIGIT:   return "DIGIT";
        case TokenType::NAME: return "NAME";
        case TokenType::SPECIALCHAR: return "SPECIALCHAR";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::RIGHT_CURLY: return "RIGHT_CURLY";
        case TokenType::LEFT_CURLY: return "LEFT_CURLY";
        case TokenType::READ_KEYWORD: return "READ_KEYWORD";
        case TokenType::PRINT_KEYWORD: return "PRINT_KEYWORD";
        case TokenType::CALL_KEYWORD: return "CALL_KEYWORD";
        case TokenType::WHILE_KEYWORD: return "WHILE_KEYWORD";
        case TokenType::IF_KEYWORD: return "IF_KEYWORD";
        case TokenType::PROCEDURE_KEYWORD: return "PROCEDURE_KEYWORD";
        case TokenType::THEN_KEYWORD: return "THEN_KEYWORD";
        case TokenType::ELSE_KEYWORD: return "ELSE_KEYWORD";
        default:      return "[Unknown TokenType]";
    }
}


class Token {
public:
    Token();

    //constructor
    Token(TokenType type, std::string id);

    //var
    TokenType type;
    std::string id;

    //method
    TokenType getToken();
    std::string getId();
};


#endif //SPA_TOKEN_H
