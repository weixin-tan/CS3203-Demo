#include "ExpressionProcessor.h"

// Returns a Expr object from query string.
Expr ExpressionProcessor::stringToExpr(std::string query) {
    query = query + ";";
    Tokeniser tokeniser;
    std::queue<Token> tokenQueue = tokeniser.putInQueue(query);
    ExpressionProcessor ep = ExpressionProcessor();
    return ep.parseExpr(tokenQueue);
}

// Returns a Expr object.
// tokensQueue is a queue of Token objects.
Expr ExpressionProcessor::tokenQueueToExpr(std::queue<Token> tokenQueue) {
    return ExpressionProcessor::parseExpr(tokenQueue);
}

// Returns a Expr object.
// tokensQueue is a queue of Token objects.
// parseExpr takes inorder, returns reverse
Expr ExpressionProcessor::parseExpr(std::queue<Token>& tokensQueue) {
    std::stack<Token> exprStack;
    std::queue<Token> tempQueue;
    std::queue<Token> formattedQueue;
    while ((!tokensQueue.empty()) && (tokensQueue.front().getToken() != TokenType::SEMICOLON)) {
        tempQueue.push(tokensQueue.front());
        tokensQueue.pop();
    }
    formattedQueue = formatExpr(tempQueue);
    while (!formattedQueue.empty()) {
        exprStack.push(formattedQueue.front());
        formattedQueue.pop();
    }
    return ExpressionProcessor::parseExprRecursion(exprStack);
}

// Returns a Expr object.
// exprStack is a stack of Token objects.
// parseExprRecursion takes reverse, returns reverse
Expr ExpressionProcessor::parseExprRecursion(std::stack<Token>& exprStack) {
    Expr expr;
    std::queue<Token> termQueue;
    int closure = 0;

    if (exprStack.empty()) {
        throw std::invalid_argument("Empty expression.");
    }

    while (!exprStack.empty()) {
        if (exprStack.top().getToken() == TokenType::RIGHT_BRACE) {
            closure++;
        }
        if (exprStack.top().getToken() == TokenType::LEFT_BRACE) {
            closure--;
        }
        if ((closure == 0) && ((exprStack.top().getToken() == TokenType::ADD)
                || (exprStack.top().getToken() == TokenType::SUBTRACT))) {
            break;
        }
        termQueue.push(exprStack.top());
        exprStack.pop();
    }
    expr.setTerm(ExpressionProcessor::parseTerm(termQueue));

    if (!exprStack.empty()) {
        expr.setOperator(exprStack.top().getToken());
        exprStack.pop();
        Expr another_expr;
        try {
            another_expr = ExpressionProcessor::parseExprRecursion(exprStack);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
        std::shared_ptr<Expr> ex = std::make_shared<Expr>(another_expr);
        expr.setExpr(ex);
    }
    return expr;
}

// Returns a Term object.
// termQueue is a queue of Token objects.
// parseTerm takes reverse, returns reverse
Term ExpressionProcessor::parseTerm(std::queue<Token>& termQueue) {
    Term term;
    std::queue<Token> factorQueue;
    int closure = 0;

    if (termQueue.empty()) {
        throw std::invalid_argument("Empty term.");
    }

    while (!termQueue.empty()) {
        if (termQueue.front().getToken() == TokenType::RIGHT_BRACE) {
            closure++;
        }
        if (termQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure--;
        }
        if ((closure == 0) && ((termQueue.front().getToken() == TokenType::MULTIPLY)
                || (termQueue.front().getToken() == TokenType::DIVIDE)
                || (termQueue.front().getToken() == TokenType::MODULO))) {
            break;
        }
        factorQueue.push(termQueue.front());
        termQueue.pop();
    }
    term.setFactor(ExpressionProcessor::parseFactor(factorQueue));

    if (!termQueue.empty()) {
        term.setOperator(termQueue.front().getToken());
        termQueue.pop();
        Term another_term;
        try {
            another_term = ExpressionProcessor::parseTerm(termQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
        std::shared_ptr<Term> te = std::make_shared<Term>(another_term);
        term.setTerm(te);
    }
    return term;
}

// Returns a Factor object.
// factorQueue is a queue of Token objects.
// parseFactor takes reverse, returns reverse
Factor ExpressionProcessor::parseFactor(std::queue<Token>& factorQueue) {
    Factor factor;

    if (factorQueue.empty()) {
        throw std::invalid_argument("Empty factor.");
    }

    if (factorQueue.front().getToken() == TokenType::RIGHT_BRACE) {
        // remove right_brace
        factorQueue.pop();
        // intermediate stack
        std::stack<Token> factorExprStack;
        while (!factorQueue.empty()) {
            factorExprStack.push(factorQueue.front());
            factorQueue.pop();
        }
        // remove left_brace
        factorExprStack.pop();
        // output stack in reverse
        std::stack<Token> exprStack;
        while (!factorExprStack.empty()) {
            exprStack.push(factorExprStack.top());
            factorExprStack.pop();
        }

        Expr expr;
        try {
            expr = ExpressionProcessor::parseExprRecursion(exprStack);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
        std::shared_ptr<Expr> ex = std::make_shared<Expr>(expr);
        factor.setExpr(ex);
        factor.setType(FactorType::EXPR);
    } else if (factorQueue.front().getToken() == TokenType::NAME) {
        factor.setVarName(factorQueue.front());
        factor.setType(FactorType::VAR);
        factorQueue.pop();
    } else if ((factorQueue.front().getToken() == TokenType::INTEGER)
            || (factorQueue.front().getToken() == TokenType::DIGIT)) {
        factor.setConstValue(factorQueue.front());
        factor.setType(FactorType::CONST);
        factorQueue.pop();
    }
    return factor;
}

// Checks if exp1 is a subset of exp2. 
bool ExpressionProcessor::fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) const {
    if (expressionIndicator == ExpressionIndicator::FULL_MATCH) {
        return areIdenticalExpr(&exp1, &exp2);
    } else {
        //There may be a case where the expression is a SHELL. (meaning it contains a term only)
        //need to strip the terms out of the empty expressions out
        if (exp1.hasOperator() && exp2.hasOperator()) {
            return isSubtree(&exp1, &exp2);
        }
        if (!exp1.hasOperator() && exp2.hasOperator()) {
            Term* term1 = exp1.getTermPtr();
            return isSubtree(term1, &exp2);
        }
        if (!exp2.hasOperator() && exp1.hasOperator()) {
            Term* term2 = exp2.getTermPtr();
            return isSubtree(&exp1, term2);
        } else {
            Term* term1 = exp1.getTermPtr();
            Term* term2 = exp2.getTermPtr();
            return isSubtree(term1, term2);
        }
    }
}

// We can only check whether expressions are similar. 
// returns True or False if the expressions are similar. 
bool ExpressionProcessor::areIdenticalExpr(Expr* root1, Expr* root2) const {
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }

    return root1->getOperator() == root2->getOperator() &&
            areIdenticalExpr(root1->getExprPtr(), root2->getExprPtr()) &&
            areIdenticalTerm(root1->getTermPtr(), root2->getTermPtr());
}

bool ExpressionProcessor::areIdenticalTerm(Term* root1, Term* root2) const {
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }
    return root1->getOperator() == root2->getOperator() &&
            areIdenticalFactor(root1->getFactorPtr(), root2->getFactorPtr()) &&
            areIdenticalTerm(root1->getTermPtr(), root2->getTermPtr());

}

bool ExpressionProcessor::areIdenticalFactor(Factor* root1, Factor* root2) const {
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }
    return root1->getType() == root2->getType() &&
            root1->getVarName().getId() == root2->getVarName().getId() &&
            root1->getConstValue().getId() == root2->getConstValue().getId() &&
            root1->getType() == root2->getType() &&
            areIdenticalExpr(root1->getExpr().get(), root2->getExpr().get());
}

// Checking if they are partial match or not. 
// returns TRUE if root 1 is a subtree of root2 
bool ExpressionProcessor::isSubtree(Expr* root1, Expr* root2) const {
    bool res = false;
    if (root1 == nullptr) {
        return true;
    }
    if (root2 == nullptr) {
        return false;
    }
    if (areIdenticalExpr(root1, root2)) {
        return true;
    }

    // This is for the case if the expression operator of the root2 is EMPTY,
    // BUT contains a term. WE would then have to check if that term
    // is a subtree of root1.
    return isSubtree(root1, root2->getExprPtr()) ||
            isSubtree(root1, root2->getTermPtr());

}

bool ExpressionProcessor::isSubtree(Term* term1, Term* term2) const {
    bool res = false;
    if (term1 == nullptr) {
        return true;
    }
    if (term2 == nullptr) {
        return false;
    }
    if (areIdenticalTerm(term1, term2)) {
        return true;
    }
    if (term1->hasFactor() && term2->hasFactor() && areIdenticalFactor(term1->getFactorPtr(), term2->getFactorPtr())
            && !term1->hasTerm()) {
        return true;
    }

    return isSubtree(term1, term2->getTermPtr()) || isSubtree(term1, this->getNestedExpr(term2).get());
}

bool ExpressionProcessor::isSubtree(Term* term1, Expr* root2) const {
    if (term1 == nullptr) {
        std::cout << "it shouldnt activate";
        return true;
    }
    if (root2 == nullptr) {
        return false;
    }
    return isSubtree(term1, root2->getExprPtr()) || isSubtree(term1, root2->getTermPtr());
}

bool ExpressionProcessor::isSubtree(Expr* root1, Term* term2) const {
    if (term2 == nullptr) {
        return false;
    }
    bool termRes = false;
    //Recursively checking the term inside.
    if (term2->hasTerm()) {
        termRes = isSubtree(root1, term2->getTermPtr());
    }

    return termRes || isSubtree(root1, this->getNestedExpr(term2).get());

}

// To get the nested expression from the root. 
std::shared_ptr<Expr> ExpressionProcessor::getNestedExpr(Term* root2) const {
    if (root2->getFactorPtr() != nullptr) {
        Factor* factor = root2->getFactorPtr();
        if (factor->hasExpr()) {
            return factor->getExpr();
        }
    }

    return std::shared_ptr<Expr>(nullptr);;
}

// Formats the expression, removes redundant braces.
std::queue<Token> ExpressionProcessor::formatExpr(std::queue<Token> tokensQueue) {
    std::stack<Token> st;
    std::queue<Token> result;

    while (!tokensQueue.empty()) {
        Token token = tokensQueue.front();
        TokenType tokenType = token.getToken();
        tokensQueue.pop();

        if ((tokenType == TokenType::LETTER) || (tokenType == TokenType::NAME) || (tokenType == TokenType::DIGIT)
                || (tokenType == TokenType::INTEGER)) {
            result.push(token);
        } else if (tokenType == TokenType::LEFT_BRACE) {
            st.push(token);
        } else if (tokenType == TokenType::RIGHT_BRACE) {
            while ((!st.empty()) && (st.top().getToken() != TokenType::LEFT_BRACE)) {
                result.push(st.top());
                st.pop();
            }
            if (!st.empty()) {
                st.pop();
            }
        } else {
            while ((!st.empty()) && (getPriority(token) <= getPriority(st.top()))) {
                result.push(st.top());
                st.pop();
            }
            st.push(token);
        }
    }

    while (!st.empty()) {
        result.push(st.top());
        st.pop();
    }

    std::stack<std::vector<Token>> st2;

    while (!result.empty()) {
        Token token = result.front();
        TokenType tokenType = token.getToken();
        std::vector<Token> tokenVector;
        tokenVector.push_back(token);
        result.pop();

        if ((tokenType == TokenType::LETTER) || (tokenType == TokenType::NAME) || (tokenType == TokenType::DIGIT)
                || (tokenType == TokenType::INTEGER)) {
            st2.push(tokenVector);
        } else {
            if (st2.empty()) {
                throw std::invalid_argument("Invalid expression.");
            }
            std::vector<Token> op1 = st2.top();
            st2.pop();
            if (st2.empty()) {
                throw std::invalid_argument("Invalid expression.");
            }
            std::vector<Token> op2 = st2.top();
            st2.pop();
            Token leftBrace = Token(TokenType::LEFT_BRACE, "(");
            std::vector<Token> leftBraceVector;
            leftBraceVector.push_back(leftBrace);
            Token rightBrace = Token(TokenType::RIGHT_BRACE, ")");
            std::vector<Token> rightBraceVector;
            rightBraceVector.push_back(rightBrace);

            std::vector<Token> resultVector;
            resultVector.insert(resultVector.end(), leftBraceVector.begin(), leftBraceVector.end());
            resultVector.insert(resultVector.end(), op2.begin(), op2.end());
            resultVector.insert(resultVector.end(), tokenVector.begin(), tokenVector.end());
            resultVector.insert(resultVector.end(), op1.begin(), op1.end());
            resultVector.insert(resultVector.end(), rightBraceVector.begin(), rightBraceVector.end());
            st2.push(resultVector);
        }
    }

    std::vector<Token> resultFinalVector = st2.top();
    std::queue<Token, std::deque<Token>>
            resultFinal(std::deque<Token>(resultFinalVector.begin(), resultFinalVector.end()));
    return resultFinal;
}

int ExpressionProcessor::getPriority(Token token) {
    TokenType tokenType = token.getToken();
    if ((tokenType == TokenType::MULTIPLY) || (tokenType == TokenType::DIVIDE) || (tokenType == TokenType::MODULO)) {
        return 2;
    } else if ((tokenType == TokenType::ADD) || (tokenType == TokenType::SUBTRACT)) {
        return 1;
    } else {
        return -1;
    }
}