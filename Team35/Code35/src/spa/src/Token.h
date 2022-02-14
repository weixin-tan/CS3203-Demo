//
// Created by Prerthan Munireternam on 05/02/2022.
//

#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>

//enum
enum TokenType{LETTER, DIGIT, NAME,  SPECIALCHAR, SEMICOLON, RIGHT_CURLY, LEFT_CURLY};

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
