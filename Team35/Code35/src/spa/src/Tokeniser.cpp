//
// Created by Prerthan Munireternam on 05/02/2022.
//

#include "Tokeniser.h"
#include <iostream>

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
    if(input.length()==1) {
        if (isNumber(input)) {
            Token t = Token(TokenType::DIGIT, input);
            return t;
        } else if (isAlphabet(input)) {
            Token t = Token(TokenType::NAME, input);
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
        } else if (input == "(") {
            Token t = Token(TokenType::LEFT_BRACE, input);
            return t;
        } else if (input == ")") {
            Token t = Token(TokenType::RIGHT_BRACE, input);
            return t;
        } else if (input == "!") {
            Token t = Token(TokenType::NOT, input);
            return t;
        } else if (input == "=") {
            Token t = Token(TokenType::EQUAL, input);
            return t;
        } else if (input == "+") {
            Token t = Token(TokenType::ADD, input);
            return t;
        } else if (input == "-") {
            Token t = Token(TokenType::SUBTRACT, input);
            return t;
        } else if (input == "*") {
            Token t = Token(TokenType::MULTIPLY, input);
            return t;
        } else if (input == "/") {
            Token t = Token(TokenType::DIVIDE, input);
            return t;
        } else if (input == "%") {
            Token t = Token(TokenType::MODULO, input);
            return t;
        } else if (input == ">") {
            Token t = Token(TokenType::GREATER, input);
            return t;
        } else if (input == "<") {
            Token t = Token(TokenType::LESSER, input);
            return t;
        } else {
            Token t = Token(TokenType::SPECIALCHAR, input);
            return t;
        }
        //to prevent forbidden naming
    } else if(input == "DIGIT" || input == "LETTER" || input == "SEMICOLON" ||
              input == "RIGHT_CURLY" || input == "LEFT_CURLY" || input == "SPECIALCHAR"){
        Token t = Token(TokenType::NAME, input);
        return t;
    }else if (input == "read"){
        Token t = Token(TokenType::READ_KEYWORD, input);
        return t;
    }else if (input == "print"){
        Token t = Token(TokenType::PRINT_KEYWORD, input);
        return t;
    }else if (input == "call"){
        Token t = Token(TokenType::CALL_KEYWORD, input);
        return t;
    }else if (input == "while"){
        Token t = Token(TokenType::WHILE_KEYWORD, input);
        return t;
    }else if (input == "if"){
        Token t = Token(TokenType::IF_KEYWORD, input);
        return t;
    }else if (input == "procedure"){
        Token t = Token(TokenType::PROCEDURE_KEYWORD, input);
        return t;
    }else if (input == "then"){
        Token t = Token(TokenType::THEN_KEYWORD, input);
        return t;
    }else if (input == "else") {
        Token t = Token(TokenType::ELSE_KEYWORD, input);
        return t;
    }else if (input == "&&") {
        Token t = Token(TokenType::AND, input);
        return t;
    }else if (input == "||") {
        Token t = Token(TokenType::OR, input);
        return t;
    }else if (input == "!=") {
        Token t = Token(TokenType::NOT_EQUAL, input);
        return t;
    }else if (input == ">=") {
        Token t = Token(TokenType::GEQ, input);
        return t;
    }else if (input == "<=") {
        Token t = Token(TokenType::LEQ, input);
        return t;
    }else if (isNumber(input)) {
        Token t = Token(TokenType::DIGIT, input);
        return t;
    }else{
        Token t = Token(TokenType::NAME, input);
        return t;
    }
}


std::string Tokeniser::printToken(Token token){
    std::string string_type = ToString(token.getToken());
    std::string string_id   = token.getId();
    std::string output      = "(" + string_type + ", " + string_id + ")";
    return output;
}

/*
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
 */

std::vector<std::string> Spacer(std::string input){
    std::stringstream ss;
    ss.str(input);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(ss, segment, '=')){
        seglist.push_back(segment);
    }
    return seglist;
}

std::vector<std::string> SpacerSemicolon(std::string input){
    std::stringstream ss;
    ss.str(input);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(ss, segment, ';')){
        seglist.push_back(segment);
    }
    return seglist;
}

std::string Convertor(std::vector<std::string> vectorString){
    std::ostringstream vts;
    std::vector<std::string> vec;

    if (!vec.empty()){
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end()-1,
                  std::ostream_iterator<std::string>(vts, ", "));
        // Now add the last element with no delimiter
        vts << vec.back();
    }
    return vts.str();
}

std::queue<Token> Tokeniser ::putInQueue(std::string input) {
    std::queue<Token> tqueue;
    std::stringstream checker(input);
    std::string line;
    while (getline(checker, line, '\n')) {
        std::string trimmedLine = trim(line);
        std::string spacedTrimmedLine = addSpace(line);
        std::stringstream phraseChecker(spacedTrimmedLine);
        std::string phrase;
        while (getline(phraseChecker, phrase, ' ')) {
            if (phrase == "") {
                continue;
            } else {
                Token t = tokeniser(phrase);
                tqueue.push(t);
            }
        }
    }
    return tqueue;
}

std::string Tokeniser::lTrim(std::string s) {
    size_t start = s.find_first_not_of(" ");
    return (start == std::string::npos) ? "" : s.substr(start) ;
}

std::string Tokeniser::rTrim(std::string s) {
    size_t end = s.find_last_not_of(" ");
    return (end ==  std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string Tokeniser::trim(std::string s) {
    return lTrim(rTrim(s));
}

std::string Tokeniser::addSpace(std::string s) {
    std::vector<char> spaceList;
    spaceList.push_back(';');
    spaceList.push_back('(');
    spaceList.push_back(')');
    spaceList.push_back('{');
    spaceList.push_back('}');
    spaceList.push_back('/');
    spaceList.push_back('*');
    spaceList.push_back('%');
    spaceList.push_back('-');
    spaceList.push_back('+');
    spaceList.push_back('!');
    spaceList.push_back('=');
    spaceList.push_back('>');
    spaceList.push_back('<');
    spaceList.push_back('&');
    spaceList.push_back('|');

    std::string returnString = "";
    for (int i = 0; i < s.length(); i++) {
        char check = s[i];
        bool edit = false;
        for (const auto& spaceToken : spaceList) {
            if (s[i] == spaceToken) {
                char next = s[i + 1];
                if (s[i] == '!' || s[i] == '=' || s[i] == '>' || s[i] == '<') {
                    if (next == '=') {
                        returnString = returnString + " " + check + next + " ";
                        i++;
                        edit = true;
                        break;
                    }
                }
                if (s[i] == '&') {
                    if (next == '&') {
                        returnString = returnString + " " + check + next + " ";
                        i++;
                        edit = true;
                        break;
                    }
                }
                if (s[i] == '|') {
                    if (next == '|') {
                        returnString = returnString + " " + check + next + " ";
                        i++;
                        edit = true;
                        break;
                    }
                }
                returnString = returnString + " " + check + " ";
                edit = true;
                break;
            }
        }
        if (!edit) {
            returnString = returnString + check;
        }
    }
    return returnString;
}
