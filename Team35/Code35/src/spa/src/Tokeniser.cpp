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
            Token t = Token(DIGIT, input);
            return t;
        } else if(isAlphabet(input)){
            Token t = Token(LETTER, input);
            return t;
        } else if (input == ";"){
            Token t = Token(SEMICOLON, input);
            return t;
        } else if (input == "}") {
            Token t = Token(RIGHT_CURLY, input);
            return t;
        } else if (input == "{") {
            Token t = Token(LEFT_CURLY, input);
            return t;
        } else {
            Token t = Token(SPECIALCHAR, input);
            return t;
        }
    } else {
        Token t = Token(NAME, input);
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
    std::string temp;
    while(getline(checker, temp, ' ')){
        if(temp == ToString(SEMICOLON)){
            break;
        } else {
            Token t = tokeniser(temp);
            tqueue.push(t);
        }
    }
    return tqueue;

}




/*
std::string Tokeniser::tokenPrinter(Tokeniser t) {
    cout << "(" << t.getType() << " , " << t.getId() << ")"<< endl;
} */


