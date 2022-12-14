#ifndef TOKENTYPE_H
#define TOKENTYPE_H

enum class TokenType {
    LETTER,
    DIGIT,
    NAME,
    SPECIALCHAR,
    SEMICOLON,
    RIGHT_CURLY,
    LEFT_CURLY,
    INTEGER,
    LEFT_BRACE,
    RIGHT_BRACE,
    READ_KEYWORD,
    PRINT_KEYWORD,
    CALL_KEYWORD,
    WHILE_KEYWORD,
    IF_KEYWORD,
    THEN_KEYWORD,
    ELSE_KEYWORD,
    PROCEDURE_KEYWORD,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    AND,
    OR,
    NOT,
    GREATER,
    GEQ,
    LESSER,
    LEQ,
    EQUAL,
    NOT_EQUAL,
    ASSIGN,
    NONE
};

#endif