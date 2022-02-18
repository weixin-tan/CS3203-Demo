#include "ConcreteSyntax.h"

ConcreteSyntax::ConcreteSyntax() {

}

Program ConcreteSyntax::parseProgram(std::queue<Token> tokensQueue) {
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

	int stmt_count = 1;

	while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
		Statement temp_statement = ConcreteSyntax::parseStmt(tokensQueue);
		temp_statement.stmt_no = stmt_count;
		stmtLst.setNextStmt(temp_statement);
		stmt_count++;
	}

	stmtLst.SetContainerType(ContainerType::kprocedure);
	procedure.setStmtLst(stmtLst);
	procedure.setSize(stmt_count);
	return procedure;
}

Statement ConcreteSyntax::parseStmt(std::queue<Token> &tokensQueue) {

	if (tokensQueue.front().getToken() == TokenType::READ_KEYWORD) {
		return ConcreteSyntax::parseRead(tokensQueue);
	}
	else if (tokensQueue.front().getToken() == TokenType::PRINT_KEYWORD) {
		return ConcreteSyntax::parsePrint(tokensQueue);
	}
	else if (tokensQueue.front().getToken() == TokenType::CALL_KEYWORD) {
		return ConcreteSyntax::parseCall(tokensQueue);
	}
	else if (tokensQueue.front().getToken() == TokenType::WHILE_KEYWORD) {
		return ConcreteSyntax::parseWhile(tokensQueue);
	}
	else if (tokensQueue.front().getToken() == TokenType::IF_KEYWORD) {
		return ConcreteSyntax::parseIf(tokensQueue);
	}
	else if (tokensQueue.front().getToken() == TokenType::NAME) {
		return ConcreteSyntax::parseAssign(tokensQueue);
	}
}

// parseAssign
Statement ConcreteSyntax::parseAssign(std::queue<Token> &tokensQueue) {
	Statement assignStmt;
	assignStmt.statement_type = StatementType::kassign_stmt;
	std::vector<std::string> result;
	result.push_back(tokensQueue.front().getId());
	assignStmt.var_name = result;
	// var_name
	tokensQueue.pop();
	// equals sign
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	resultString = ConcreteSyntax::parseExprString(tokensQueue);
	assignStmt.expr = resultString[0];
	// pass constant
	assignStmt.constant = resultString[1];
	// semicolon
	tokensQueue.pop();
	return assignStmt;
}

// for iteration 1
std::vector<std::vector<std::string>> ConcreteSyntax::parseExprString(std::queue<Token>& tokensQueue) {
	std::vector<std::vector<std::string>> result;
	while (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		if (tokensQueue.front().getToken() == TokenType::NAME) {
			result[0].push_back(tokensQueue.front().getId());
		}
		if (tokensQueue.front().getToken() == TokenType::INTEGER) {
			result[1].push_back(tokensQueue.front().getId());
		}
		tokensQueue.pop();
	}
	return result;
}

// parseExpr takes inorder, returns reverse
Expr ConcreteSyntax::parseExpr(std::queue<Token> &tokensQueue) {
	std::stack<Token> exprStack;
	while (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
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
		if (exprStack.top().getToken() == TokenType::RIGHT_BRACE) {
			closure++;
		}
		if (exprStack.top().getToken() == TokenType::LEFT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((exprStack.top().getToken() == TokenType::ADD) || (exprStack.top().getToken() == TokenType::SUBTRACT))) {
			break;
		}
		termQueue.push(exprStack.top());
		exprStack.pop();
	}
	expr.setTerm(ConcreteSyntax::parseTerm(termQueue));

	if (!exprStack.empty()) {
		expr.setOperator(exprStack.top().getToken());
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
		if (termQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		if (termQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((termQueue.front().getToken() == TokenType::MULTIPLY) || (termQueue.front().getToken() == TokenType::DIVIDE) || (termQueue.front().getToken() == TokenType::MODULO))) {
			break;
		}
		factorQueue.push(termQueue.front());
		termQueue.pop();
	}
	term.setFactor(ConcreteSyntax::parseFactor(factorQueue));
	
	if (!termQueue.empty()) {
		term.setOperator(termQueue.front().getToken());
		termQueue.pop();
		term.setTerm(ConcreteSyntax::parseTerm(termQueue));
	}
	return term;
}

// parseFactor takes reverse, returns reverse
Factor ConcreteSyntax::parseFactor(std::queue<Token> &factorQueue) {
	Factor factor;
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

		// comment out as not needed for Iteration 1
		// factor.setExpr(&parseExprRecursion(exprStack));
		factor.setType(FactorType::EXPR);
	}
	else if (factorQueue.front().getToken() == TokenType::NAME) {
		factor.setVarName(factorQueue.front());
		factor.setType(FactorType::VAR);
		factorQueue.pop();
	}
	else if (factorQueue.front().getToken() == TokenType::INTEGER) {
		factor.setConstValue(factorQueue.front());
		factor.setType(FactorType::CONST);
		factorQueue.pop();
	}
	return factor;
}

// end of chaining parsers for parsing Assign
// done



// start for parsing While
Statement ConcreteSyntax::parseWhile(std::queue<Token> &tokensQueue) {
	Statement whileStmt;
	whileStmt.statement_type = StatementType::kwhile_stmt;

	// parse cond_expr
	// remove while_keyword
	tokensQueue.pop();
	// remove left_brace
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	resultString = ConcreteSyntax::parseCondExprString(tokensQueue);
	whileStmt.cond_expr = resultString[0];
	// pass constant
	whileStmt.constant = resultString[1];

	// parse stmtLst
	// remove left_curly
	tokensQueue.pop();
	StmtLst stmtLst;
	int stmt_count = 1;
	while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
		Statement temp_statement = ConcreteSyntax::parseStmt(tokensQueue);
		temp_statement.stmt_no = stmt_count;
		stmtLst.setNextStmt(temp_statement);
		stmt_count++;
	}
	// remove right_curly
	tokensQueue.pop();

	stmtLst.SetContainerType(ContainerType::kwhile);
	whileStmt.while_stmt_list = &stmtLst;
	whileStmt.stmt_no = stmt_count;
	return whileStmt;
}

// parse cond_expr String for Iteration 1
std::vector<std::vector<std::string>> ConcreteSyntax::parseCondExprString(std::queue<Token>& tokensQueue) {
	std::vector<std::vector<std::string>> result;
	int closure = 1;
	while (closure != 0) {
		if (tokensQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		if (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			if (closure == 1) {
				closure--;
				// remove right_brace
				tokensQueue.pop();
				break;
			}
			else {
				closure--;
			}
		}
		if (tokensQueue.front().getToken() == TokenType::NAME) {
			result[0].push_back(tokensQueue.front().getId());
		}
		if (tokensQueue.front().getToken() == TokenType::INTEGER) {
			result[1].push_back(tokensQueue.front().getId());
		}

		tokensQueue.pop();
	}
	return result;
}

// parse cond_expr
CondExpr ConcreteSyntax::parseCondExpr(std::queue<Token> &tokensQueue) {
	std::queue<Token> condExprQueue;
	int closure = 1;
	while (closure != 0) {
		if (tokensQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		if (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			if (closure == 1) {
				closure--;
				// remove right_brace
				tokensQueue.pop();
				break;
			}
			else {
				closure--;
			}
		}
		condExprQueue.push(tokensQueue.front());
		tokensQueue.pop();
	}
	return ConcreteSyntax::parseCondExprRecursion(condExprQueue);
}

// parse cond_expr recursive
CondExpr ConcreteSyntax::parseCondExprRecursion(std::queue<Token> &condExprQueue) {
	CondExpr condExpr;
	std::queue<Token> relExprQueue;
	int closure = 0;
	while (!condExprQueue.empty()) {
		if (condExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
			condExprQueue.pop();
			continue;
		}
		if (condExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			closure--;
			condExprQueue.pop();
			continue;
		}
		if (condExprQueue.front().getToken() == TokenType::NOT) {
			continue;
		}
		if ((closure == 0) && ((condExprQueue.front().getToken() == TokenType::AND) || (condExprQueue.front().getToken() == TokenType::OR))) {
			break;
		}
		relExprQueue.push(condExprQueue.front());
		condExprQueue.pop();
	}
	condExpr.setRelExpr(ConcreteSyntax::parseRelExpr(relExprQueue));

	if (!condExprQueue.empty()) {
		condExpr.setOperator(condExprQueue.front().getToken());
		condExprQueue.pop();
		condExpr.setCondExpr(ConcreteSyntax::parseCondExprRecursion(condExprQueue));
	}
	return condExpr;
}

// parse rel_expr
RelExpr ConcreteSyntax::parseRelExpr(std::queue<Token> &relExprQueue) {
	RelExpr relExpr;
	std::queue<Token> relFactorOneQueue;
	std::queue<Token> relFactorTwoQueue;
	int closure = 0;

	// set rel_factor one
	while (!relExprQueue.empty()) {
		if (relExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		if (relExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((relExprQueue.front().getToken() == TokenType::GREATER) || (relExprQueue.front().getToken() == TokenType::GEQ)
				|| (relExprQueue.front().getToken() == TokenType::LESSER) || (relExprQueue.front().getToken() == TokenType::LEQ)
				|| (relExprQueue.front().getToken() == TokenType::EQUAL) || (relExprQueue.front().getToken() == TokenType::NOT_EQUAL))) {
			break;
		}
		relFactorOneQueue.push(relExprQueue.front());
		relExprQueue.pop();
	}
	relExpr.setRelFactorOne(ConcreteSyntax::parseRelFactor(relFactorOneQueue));

	relExpr.setOperator(relExprQueue.front().getToken());
	relExprQueue.pop();

	// set rel_factor two
	while (!relExprQueue.empty()) {
		if (relExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		if (relExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((relExprQueue.front().getToken() == TokenType::GREATER) || (relExprQueue.front().getToken() == TokenType::GEQ)
			|| (relExprQueue.front().getToken() == TokenType::LESSER) || (relExprQueue.front().getToken() == TokenType::LEQ)
			|| (relExprQueue.front().getToken() == TokenType::EQUAL) || (relExprQueue.front().getToken() == TokenType::NOT_EQUAL))) {
			break;
		}
		relFactorTwoQueue.push(relExprQueue.front());
		relExprQueue.pop();
	}
	relExpr.setRelFactorTwo(ConcreteSyntax::parseRelFactor(relFactorTwoQueue));
	return relExpr;
}

// parse rel_factor
RelFactor ConcreteSyntax::parseRelFactor(std::queue<Token> &relFactorQueue) {
	RelFactor relFactor;
	if (relFactorQueue.size() != 1) {
		std::stack<Token> exprStack;
		while (!relFactorQueue.empty()) {
			exprStack.push(relFactorQueue.front());
			relFactorQueue.pop();
		}
		relFactor.setExpr(parseExprRecursion(exprStack));
		relFactor.setType(FactorType::EXPR);
	}
	else if (relFactorQueue.front().getToken() == TokenType::NAME) {
		relFactor.setVarName(relFactorQueue.front());
		relFactor.setType(FactorType::VAR);
		relFactorQueue.pop();
	}
	else if (relFactorQueue.front().getToken() == TokenType::INTEGER) {
		relFactor.setConstValue(relFactorQueue.front());
		relFactor.setType(FactorType::CONST);
		relFactorQueue.pop();
	}
	return relFactor;
}

// end for parsing While
// done

// start for parsing If/Else
Statement ConcreteSyntax::parseIf(std::queue<Token> &tokensQueue) {
	Statement ifStmt;
	ifStmt.statement_type = StatementType::kif_stmt;

	// parse cond_expr
	// remove if_keyword
	tokensQueue.pop();
	// remove left_brace
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	resultString = ConcreteSyntax::parseCondExprString(tokensQueue);
	ifStmt.cond_expr = resultString[0];
	// pass constant
	ifStmt.constant = resultString[1];

	// parse then stmtLst
	// remove then_keyword
	tokensQueue.pop();
	// remove left_curly
	tokensQueue.pop();
	StmtLst stmtLst;
	int stmt_count = 1;
	while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
		Statement temp_statement = ConcreteSyntax::parseStmt(tokensQueue);
		temp_statement.stmt_no = stmt_count;
		stmtLst.setNextStmt(temp_statement);
		stmt_count++;
	}
	// remove right_curly
	tokensQueue.pop();
	// set then stmtLst
	stmtLst.SetContainerType(ContainerType::kifthen);
	ifStmt.ifthen_stmt_list = &stmtLst;
	ifStmt.stmt_no = stmt_count;
	
	// parse else stmtLst
	// remove else_keyword
	tokensQueue.pop();
	// remove left_curly
	tokensQueue.pop();
	StmtLst stmtLstElse;
	int stmt_count_else = 1;
	while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
		Statement temp_statement = ConcreteSyntax::parseStmt(tokensQueue);
		temp_statement.stmt_no = stmt_count_else;
		stmtLstElse.setNextStmt(temp_statement);
		stmt_count_else++;
	}
	// remove right_curly
	tokensQueue.pop();
	// set else stmtLst
	stmtLstElse.SetContainerType(ContainerType::kifelse);
	ifStmt.ifelse_stmt_list = &stmtLstElse;


	ifStmt.stmt_no = stmt_count + stmt_count_else;

	return ifStmt;
}
// end for parsing If/Else
// done

// start for parsing Read
Statement ConcreteSyntax::parseRead(std::queue<Token>& tokensQueue) {
	Statement readStmt;
	readStmt.statement_type = StatementType::kread_stmt;
	// remove read_keyword
	tokensQueue.pop();
	// Iteration 1 only passing vector of string
	std::vector<std::string> result;
	result.push_back(tokensQueue.front().getId());
	readStmt.var_name = result;
	// remove var_name
	tokensQueue.pop();
	// remove semicolon
	tokensQueue.pop();
	return readStmt;
}
// end for parsing Read

// start for parsing Print
Statement ConcreteSyntax::parsePrint(std::queue<Token>& tokensQueue) {
	Statement printStmt;
	printStmt.statement_type = StatementType::kprint_stmt;
	// remove print_keyword
	tokensQueue.pop();
	// Iteration 1 only passing vector of string
	std::vector<std::string> result;
	result.push_back(tokensQueue.front().getId());
	printStmt.var_name = result;
	// remove var_name
	tokensQueue.pop();
	// remove semicolon
	tokensQueue.pop();
	return printStmt;
}
// end for parsing Print

// start for parsing Call
Statement ConcreteSyntax::parseCall(std::queue<Token>& tokensQueue) {
	Statement callStmt;
	callStmt.statement_type = StatementType::kcall_stmt;
	// remove call_keyword
	tokensQueue.pop();
	callStmt.proc_name = tokensQueue.front().getId();
	// remove proc_name
	tokensQueue.pop();
	// remove semicolon
	tokensQueue.pop();
	return callStmt;
}
// end for parsing Call