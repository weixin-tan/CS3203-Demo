//
// Created by Prerthan Munireternam on 05/02/2022.
//

#include "Token.h"

Token::Token(TokenType type, std::string id) {
    Token::type = type;
    Token::id = id;
}

TokenType Token::getToken() {
    return type;
}

std::string Token::getId() {
    return id;
}