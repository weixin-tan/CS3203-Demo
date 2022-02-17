//
// Created by Prerthan Munireternam on 05/02/2022.
//

#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>

//enum
enum TokenType{LETTER, DIGIT, NAME,  SPECIALCHAR, SEMICOLON, RIGHT_CURLY, LEFT_CURLY, READ, PRINT, CALL, WHILE, IF,
        PROCEDURE, THEN, ELSE};

inline const char* ToString(TokenType v)
{
    switch (v)
    {
        case LETTER:   return "LETTER";
        case DIGIT:   return "DIGIT";
        case NAME: return "NAME";
        case SPECIALCHAR: return "SPECIALCHAR";
        case SEMICOLON: return "SEMICOLON";
        case RIGHT_CURLY: return "RIGHT_CURLY";
        case LEFT_CURLY: return "LEFT_CURLY";
        case READ: return "READ";
        case PRINT: return "PRINT";
        case CALL: return "CALL";
        case WHILE: return "WHILE";
        case IF: return "IF";
        case PROCEDURE: return "PROCEDURE";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        default:      return "[Unknown TokenType]";
    }
}

class Token {
public:
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
