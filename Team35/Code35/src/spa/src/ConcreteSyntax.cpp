#include "ConcreteSyntax.h"

ConcreteSyntax::ConcreteSyntax() {

}

Program ConcreteSyntax::parseProgram(std::queue<Token> &tokensQueue) {
	Program program;
	ProcedureLst procedureLst;

	int stmt_count = 0;

	while (tokensQueue.size() != 0) {
		Procedure temp_procedure = ConcreteSyntax::parseProcedure(tokensQueue);
		stmt_count += temp_procedure.getSize();
		procedureLst.setNextProcedure(temp_procedure);
	}

	program.setProcedureLst(procedureLst);
	return program;
}

Procedure ConcreteSyntax::parseProcedure(std::queue<Token> &tokensQueue) {
	// procedure_keyword
	tokensQueue.pop();
	// procedure name
	tokensQueue.pop();
	// left_curly
	tokensQueue.pop();

	Procedure procedure;
	StmtLst stmtLst;

	int stmt_count = 2;

	while (tokensQueue.front().type != RIGHT_CURLY) {
		Statement temp_statement = ConcreteSyntaxBasic::parseStmt(tokensQueue);
		temp_statement.stmt_no = stmt_count;
		stmtLst.setNextStmt(temp_statement);
		stmt_count++;
	}

	stmtLst.setContainerType(kprocedure);
	procedure.setStmtLst(stmtLst);
	procedure.setSize(stmt_count);
	return procedure;
}

Statement ConcreteSyntax::parseStmt(std::queue<Token> &tokensQueue) {

	if (tokensQueue.front().type == READ_KEYWORD) {
		return ConcreteSyntax::parseRead(tokensQueue);
	}
	else if (tokensQueue.front().type == PRINT_KEYWORD) {
		return ConcreteSyntax::parsePrint(tokensQueue);
	}
	else if (tokensQueue.front().type == CALL_KEYWORD) {
		return ConcreteSyntax::parseCall(tokensQueue);
	}
	else if (tokensQueue.front().type == WHILE_KEYWORD) {
		return ConcreteSyntax::parseWhile(tokensQueue);
	}
	else if (tokensQueue.front().type == IF_KEYWORD) {
		return ConcreteSyntax::parseIf(tokensQueue);
	}
	else if (tokensQueue.front().type == NAME) {
		return ConcreteSyntax::parseAssign(tokensQueue);
	}
}

// parseAssign
Statement ConcreteSyntaxBasic::parseAssign(std::queue<Token> &tokensQueue) {
	Statement assignStmt;
	assignStmt.statement_type = kassign_stmt;
	assignStmt.var_name = tokensQueue.front();
	// var_name
	tokensQueue.pop();
	// equals sign
	tokensQueue.pop();
	assignStmt.expr = ConcreteSyntaxBasic::parseExpr(tokensQueue);
	// semicolon
	tokensQueue.pop();
	return assignStmt;
}

// parseExpr takes inorder, returns reverse
Expr ConcreteSyntax::parseExpr(std::queue<Token> &tokensQueue) {
	std::stack<Token> exprStack;
	while (tokensQueue.front().type != SEMICOLON) {
		exprStack.push(tokensQueue.front());
		tokensQueue.pop();
	}
	return ConcreteSyntax::parseExprRecursion(exprStack);
}

// parseExprRecursion takes reverse, returns reverse
Expr ConcreteSyntax::parseExprRecursion(std::stack<Token> &exprStack) {
	Expr expr;
	std::queue<Token> termQueue;
	int closure = 0;
	while (!exprStack.empty()) {
		if (exprStack.top().type == RIGHT_BRACE) {
			closure++;
		}
		if (exprStack.top().type == LEFT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((exprStack.top().type == PLUS) || (exprStack.top().type == MINUS))) {
			break;
		}
		termQueue.push(exprStack.top());
		exprStack.pop();
	}
	expr.setTerm(ConcreteSyntax::parseTerm(termQueue));

	if (!exprStack.empty()) {
		expr.setOperator(exprStack.top());
		exprStack.pop();
		expr.setExpr(ConcreteSyntax::parseExprRecursion(exprStack));
	}
	return expr;
}

// parseTerm takes reverse, returns reverse
Term ConcreteSyntax::parseTerm(std::queue<Token> &termQueue) {
	Term term;
	std::queue<Token> factorQueue;
	int closure = 0;
	while (!termQueue.empty()) {
		if (termQueue.front().type == RIGHT_BRACE) {
			closure++;
		}
		if (termQueue.front().type == LEFT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((termQueue.front().type == MULTIPLY) || (termQueue.front().type == DIVIDE) || (termQueue.front().type == MODULO))) {
			break;
		}
		factorQueue.push(termQueue.front());
		termQueue.pop();
	}
	term.setFactor(ConcreteSyntax::parseFactor(factorQueue));
	
	if (!termQueue.empty()) {
		term.setOperator(termQueue.front());
		termQueue.pop();
		term.setTerm(ConcreteSyntax::parseTerm(termQueue));
	}
	return term;
}

// parseFactor takes reverse, returns reverse
Factor ConcreteSyntax::parseFactor(std::queue<Token> factorQueue) {
	Factor factor;
	if (factorQueue.front().type == RIGHT_BRACE) {
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

		factor.setExpr(parseExprRecursion(exprStack));
		factor.setType(expr);
	}
	else if (tokensQueue.front().type == NAME) {
		factor.setVarName(tokensQueue.front());
		factor.setType(varName);
		tokensQueue.pop();
	}
	else if (tokensQueue.front().type == INTEGER) {
		factor.setConstValue(tokensQueue.front());
		factor.setType(constValue);
		tokensQueue.pop();
	}
	return factor;
}

// end of chaining parsers for parsing Assign
// done



// for parsing While
WhileStmt ConcreteSyntax::parseWhile(std::queue tokensQueue) {
	WhileStmt whileStmt();
	// while_keyword
	tokensQueue.pop();
	whileStmt.setCondExpr(ConcreteSyntax::parseCondExpr(tokensQueue));
	// left_curly
	tokensQueue.pop();

	StmtLst stmtLst();
	while (tokensQueue.front() != RIGHT_CURLY) {
		stmtLst.setNextStmt(ConcreteSyntax::parseStmt(tokensQueue));
	}
	whileStmt.setStmtLst(stmtLst);
}

CondExpr ConcreteSyntax::parseCondExpr(std::queue tokensQueue) {
	std::stack<Token> condExprStack;
	// left_brace
	tokensQueue.pop();
	while (tokensQueue.front() != RIGHT_BRACE) {
		condExprStack.push(tokensQueue.front());
		tokensQueue.pop();
	}
	return parseCondExprRecursion(condExprStack);
}

CondExpr ConcreteSyntax::parseCondExprRecursion(std::stack<Token> condExprStack) {
	CondExpr condExpr();
	if ()
}

// end for parsing While


