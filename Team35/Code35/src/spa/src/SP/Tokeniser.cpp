#include "Tokeniser.h"

Tokeniser::Tokeniser() {

    // Forbidden flag is a variable to tell the tokeniser that the NEXT value would be a
    // NAME, instead of a procedure.
    // It will be activated when there is a keyword (print/call/if/ while/read) 
    // OR a + = - / * < etc. 
    // but it will be deactivated the moment we find a ) } or ; to signify the end of
    // the statement/statement container/while for loop/ condition

    // NOTE that ( will not change anything so we never touch forbidden flag

    // When the forbiddenflag is activated, any subsequent tokens that are keywords will
    // be recognised as names. 

    forbiddenFlag = false;
}

bool Tokeniser::isNumber(const std::string& str) {
    if (str.length() == 1) {
        return std::isdigit(str[0]);
    } else {
        std::regex intRegex("^[1-9][0-9]*$");
        return std::regex_match(str, intRegex);
    }
}

bool Tokeniser::isAlphabet(const std::string& str) {
    for (char const& c : str) {
        if (std::isalpha(c) == 0) return false;
    }
    return true;
}

Token Tokeniser::tokeniser(std::string input) {
    if (input.length() == 1) {
        if (isNumber(input)) {
            Token t = Token(TokenType::DIGIT, input);
            return t;
        } else if (isAlphabet(input)) {
            Token t = Token(TokenType::NAME, input);
            return t;
        } else if (input == ";") {
            Token t = Token(TokenType::SEMICOLON, input);
            forbiddenFlag = false;
            return t;
        } else if (input == "}") {
            Token t = Token(TokenType::RIGHT_CURLY, input);
            forbiddenFlag = false;
            return t;
        } else if (input == "{") {
            Token t = Token(TokenType::LEFT_CURLY, input);
            forbiddenFlag = false;
            return t;
        } else if (input == "(") {
            Token t = Token(TokenType::LEFT_BRACE, input);
            return t;
        } else if (input == ")") {
            Token t = Token(TokenType::RIGHT_BRACE, input);
            forbiddenFlag = false;
            return t;
        } else if (input == "!") {
            Token t = Token(TokenType::NOT, input);
            return t;
        } else if (input == "=") {
            Token t = Token(TokenType::ASSIGN, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "+") {
            Token t = Token(TokenType::ADD, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "-") {
            Token t = Token(TokenType::SUBTRACT, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "*") {
            Token t = Token(TokenType::MULTIPLY, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "/") {
            Token t = Token(TokenType::DIVIDE, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "%") {
            Token t = Token(TokenType::MODULO, input);
            forbiddenFlag = true;
            return t;
        } else if (input == ">") {
            Token t = Token(TokenType::GREATER, input);
            forbiddenFlag = true;
            return t;
        } else if (input == "<") {
            Token t = Token(TokenType::LESSER, input);
            forbiddenFlag = true;
            return t;
        } else {
            Token t = Token(TokenType::SPECIALCHAR, input);
            return t;
        }
        //to prevent forbidden naming
    } else if (input == "DIGIT" || input == "LETTER" || input == "SEMICOLON" ||
            input == "RIGHT_CURLY" || input == "LEFT_CURLY" || input == "SPECIALCHAR") {
        Token t = Token(TokenType::NAME, input);
        return t;
    } else if (input == "read") {
        Token t = Token(TokenType::READ_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "print") {
        Token t = Token(TokenType::PRINT_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "call") {
        Token t = Token(TokenType::CALL_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "while") {
        Token t = Token(TokenType::WHILE_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "if") {
        Token t = Token(TokenType::IF_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "procedure") {
        Token t = Token(TokenType::PROCEDURE_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        forbiddenFlag = true;
        return t;
    } else if (input == "then") {
        Token t = Token(TokenType::THEN_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        return t;
    } else if (input == "else") {
        Token t = Token(TokenType::ELSE_KEYWORD, input);
        if (forbiddenFlag) {
            t = Token(TokenType::NAME, input);
        }
        return t;
    } else if (input == "&&") {
        Token t = Token(TokenType::AND, input);
        forbiddenFlag = true;
        return t;
    } else if (input == "||") {
        Token t = Token(TokenType::OR, input);
        forbiddenFlag = true;
        return t;
    } else if (input == "!=") {
        Token t = Token(TokenType::NOT_EQUAL, input);
        forbiddenFlag = true;
        return t;
    } else if (input == ">=") {
        Token t = Token(TokenType::GEQ, input);
        forbiddenFlag = true;
        return t;
    } else if (input == "<=") {
        Token t = Token(TokenType::LEQ, input);
        forbiddenFlag = true;
        return t;
    } else if (input == "==") {
        Token t = Token(TokenType::EQUAL, input);
        forbiddenFlag = true;
        return t;
    } else if (isNumber(input)) {
        Token t = Token(TokenType::DIGIT, input);
        return t;
    } else if (isName(input)) {
        Token t = Token(TokenType::NAME, input);
        return t;
    } else {
        Token t = Token(TokenType::SPECIALCHAR, input);
        return t;
    }
}

std::string Tokeniser::printToken(Token token) {
    std::string string_type = ToString(token.getToken());
    std::string string_id = token.getId();
    std::string output = "(" + string_type + ", " + string_id + ")";
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

std::vector<std::string> Spacer(std::string input) {
    std::stringstream ss;
    ss.str(input);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(ss, segment, '=')) {
        seglist.push_back(segment);
    }
    return seglist;
}

std::vector<std::string> SpacerSemicolon(std::string input) {
    std::stringstream ss;
    ss.str(input);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(ss, segment, ';')) {
        seglist.push_back(segment);
    }
    return seglist;
}

std::string Convertor(std::vector<std::string> vectorString) {
    std::ostringstream vts;
    std::vector<std::string> vec;

    if (!vec.empty()) {
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end() - 1,
                  std::ostream_iterator<std::string>(vts, ", "));
        // Now add the last element with no delimiter
        vts << vec.back();
    }
    return vts.str();
}

std::queue<Token> Tokeniser::forbiddenWord(std::queue<Token> inputQueue) {
    std::vector<Token> tempVec;
    std::queue<Token> outputQueue;
    while (!inputQueue.empty()) {
        tempVec.push_back(inputQueue.front());
        inputQueue.pop();
    }

    for (int i = 0; i < tempVec.size() - 1; i++) {
        if (tempVec[i].type == TokenType::CALL_KEYWORD
                //|| tempVec[i].type == TokenType::PROCEDURE_KEYWORD
                || tempVec[i].type == TokenType::THEN_KEYWORD || tempVec[i].type == TokenType::ELSE_KEYWORD
                || tempVec[i].type == TokenType::WHILE_KEYWORD || tempVec[i].type == TokenType::IF_KEYWORD) {
            if (tempVec[i + 1].type == TokenType::EQUAL || tempVec[i + 1].type == TokenType::ADD
                    || tempVec[i + 1].type == TokenType::SUBTRACT || tempVec[i + 1].type == TokenType::MULTIPLY
                    || tempVec[i + 1].type == TokenType::MODULO || tempVec[i + 1].type == TokenType::DIVIDE
                    || tempVec[i + 1].type == TokenType::AND || tempVec[i + 1].type == TokenType::OR
                    || tempVec[i + 1].type == TokenType::NOT || tempVec[i + 1].type == TokenType::GREATER
                    || tempVec[i + 1].type == TokenType::GEQ || tempVec[i + 1].type == TokenType::LESSER
                    || tempVec[i + 1].type == TokenType::LEQ || tempVec[i + 1].type == TokenType::NOT_EQUAL
                    || tempVec[i + 1].type == TokenType::ASSIGN) {
                tempVec[i].type = TokenType::NAME;
            }
        } else if (tempVec[i].type == TokenType::PRINT_KEYWORD || tempVec[i].type == TokenType::READ_KEYWORD
                || tempVec[i].type == TokenType::PROCEDURE_KEYWORD) {
            if (tempVec[i + 1].type == TokenType::EQUAL || tempVec[i + 1].type == TokenType::ADD
                    || tempVec[i + 1].type == TokenType::SUBTRACT || tempVec[i + 1].type == TokenType::MULTIPLY
                    || tempVec[i + 1].type == TokenType::MODULO || tempVec[i + 1].type == TokenType::DIVIDE
                    || tempVec[i + 1].type == TokenType::AND || tempVec[i + 1].type == TokenType::OR
                    || tempVec[i + 1].type == TokenType::NOT || tempVec[i + 1].type == TokenType::GREATER
                    || tempVec[i + 1].type == TokenType::GEQ || tempVec[i + 1].type == TokenType::LESSER
                    || tempVec[i + 1].type == TokenType::LEQ || tempVec[i + 1].type == TokenType::NOT_EQUAL
                    || tempVec[i + 1].type == TokenType::ASSIGN) {
                tempVec[i].type = TokenType::NAME;
            } else if (tempVec[i + 1].type == TokenType::PRINT_KEYWORD
                    || tempVec[i + 1].type == TokenType::READ_KEYWORD
                    || tempVec[i + 1].type == TokenType::CALL_KEYWORD
                    || tempVec[i + 1].type == TokenType::PROCEDURE_KEYWORD
                    || tempVec[i + 1].type == TokenType::THEN_KEYWORD || tempVec[i + 1].type == TokenType::ELSE_KEYWORD
                    || tempVec[i + 1].type == TokenType::WHILE_KEYWORD
                    || tempVec[i + 1].type == TokenType::IF_KEYWORD) {
                tempVec[i + 1].type = TokenType::NAME;
            }
        }
    }

    for (Token t : tempVec) {
        outputQueue.push(t);
    }
    return outputQueue;
}

std::queue<Token> Tokeniser::putInQueue(std::string input) {
    //std::queue<Token> ansQueue;
    std::queue<Token> tqueue;
    std::stringstream checker(input);
    std::string line;
    while (getline(checker, line, '\n')) {
        std::string trimmedLine = trim(line);
        std::string spacedTrimmedLine = addSpace(line);
        std::stringstream phraseChecker(spacedTrimmedLine);
        std::string phrase1;
        std::string phrase;
        while (getline(phraseChecker, phrase1, ' ')) {
            std::stringstream tabChecker(phrase1);
            while (getline(tabChecker, phrase, '\t')) {
                if (phrase == "") {
                    continue;
                } else {
                    Token t = tokeniser(phrase);
                    tqueue.push(t);
                }
            }
        }
    }
    std::queue<Token> ansQueue = forbiddenWord(tqueue);
    //return tqueue;
    return ansQueue;

}

std::string Tokeniser::lTrim(std::string s) {
    size_t start = s.find_first_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string Tokeniser::rTrim(std::string s) {
    size_t end = s.find_last_not_of(" \t");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string Tokeniser::trim(std::string s) {
    return lTrim(rTrim(s));
}

std::string Tokeniser::addSpace(std::string s) {
    std::vector<char> spaceList = {';', '(', ')', '{', '}', '/', '*', '%', '-', '+', '!', '=', '>', '<', '&', '|'};

    std::string returnString = "";
    for (int i = 0; i < s.length(); i++) {
        char check = s[i];
        bool edit = false;
        for (const auto& spaceToken : spaceList) {
            if (s[i] == spaceToken && i <= s.length()) { //todo: check for last position
                // TODO: RESOLVE PYRAMID OF DOOM!
                char next = s[i + 1];
                if ((s[i] == '!' || s[i] == '=' || s[i] == '>' || s[i] == '<') && next == '=') {
                    returnString = returnString + " " + check + next + " ";
                    i++;
                    edit = true;
                    break;

                }
                if (s[i] == '&' && next == '&') {
                    returnString = returnString + " " + check + next + " ";
                    i++;
                    edit = true;
                    break;

                }
                if (s[i] == '|' && next == '|') {
                    returnString = returnString + " " + check + next + " ";
                    i++;
                    edit = true;
                    break;
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

bool Tokeniser::isName(const std::string& str) {
    std::regex nameRegex("^[a-zA-Z][A-Za-z0-9]*$");
    return std::regex_match(str, nameRegex);
}
