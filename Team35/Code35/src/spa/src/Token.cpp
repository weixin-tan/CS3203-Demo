#include "Token.h"

Token::Token() {

}

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
