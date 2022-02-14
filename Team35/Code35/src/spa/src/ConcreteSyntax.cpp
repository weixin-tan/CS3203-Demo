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

	while (tokensQueue.front().type != RIGHT_CURLY) {
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
	assignStmt.expr = ConcreteSyntax::parseExpr(tokensQueue);
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
		if ((closure == 0) && ((exprStack.top().type == Operator::ADD) || (exprStack.top().type == Operator::SUBTRACT))) {
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
		if (termQueue.front().type == LEFT_BRACE) {
			closure++;
		}
		if (termQueue.front().type == RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((termQueue.front().type == Operator::MULTIPLY) || (termQueue.front().type == Operator::DIVIDE) || (termQueue.front().type == MODULO))) {
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
Factor ConcreteSyntax::parseFactor(std::queue<Token> &factorQueue) {
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

		factor.setExpr(&parseExprRecursion(exprStack));
		factor.setType(FactorType::EXPR);
	}
	else if (factorQueue.front().type == NAME) {
		factor.setVarName(factorQueue.front());
		factor.setType(FactorType::VAR);
		factorQueue.pop();
	}
	else if (factorQueue.front().type == INTEGER) {
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
	whileStmt.cond_expr = ConcreteSyntax::parseCondExpr(tokensQueue);

	// parse stmtLst
	// remove left_curly
	tokensQueue.pop();
	StmtLst stmtLst;
	int stmt_count = 1;
	while (tokensQueue.front().type != RIGHT_CURLY) {
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

// parse cond_expr
CondExpr ConcreteSyntax::parseCondExpr(std::queue<Token> &tokensQueue) {
	std::queue<Token> condExprQueue;
	int closure = 1;
	while (closure != 0) {
		if (tokensQueue.front().type == LEFT_BRACE) {
			closure++;
		}
		if (tokensQueue.front().type == RIGHT_BRACE) {
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
		if (condExprQueue.front().type == LEFT_BRACE) {
			closure++;
			condExprQueue.pop();
			continue;
		}
		if (condExprQueue.front().type == RIGHT_BRACE) {
			closure--;
			condExprQueue.pop();
			continue;
		}
		if (condExprQueue.front().type == Operator::NOT) {
			continue;
		}
		if ((closure == 0) && ((condExprQueue.front().type == Operator::AND) || (condExprQueue.front().type == Operator::OR))) {
			break;
		}
		relExprQueue.push(condExprQueue.front());
		condExprQueue.pop();
	}
	condExpr.setRelExpr(ConcreteSyntax::parseRelExpr(relExprQueue));

	if (!condExprQueue.empty()) {
		condExpr.setOperator(condExprQueue.front());
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
		if (relExprQueue.front().type == LEFT_BRACE) {
			closure++;
		}
		if (relExprQueue.front().type == RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((relExprQueue.front().type == Operator::GREATER) || (relExprQueue.front().type == Operator::GEQ)
				|| (relExprQueue.front().type == Operator::LESSER) || (relExprQueue.front().type == Operator::LEQ)
				|| (relExprQueue.front().type == Operator::EQUAL) || (relExprQueue.front().type == Operator::NOT_EQUAL))) {
			break;
		}
		relFactorOneQueue.push(relExprQueue.front());
		relExprQueue.pop();
	}
	relExpr.setRelFactorOne(ConcreteSyntax::parseRelFactor(relFactorOneQueue));

	relExpr.setOperator(relExprQueue.front());
	relExprQueue.pop();

	// set rel_factor two
	while (!relExprQueue.empty()) {
		if (relExprQueue.front().type == LEFT_BRACE) {
			closure++;
		}
		if (relExprQueue.front().type == RIGHT_BRACE) {
			closure--;
		}
		if ((closure == 0) && ((relExprQueue.front().type == Operator::GREATER) || (relExprQueue.front().type == Operator::GEQ)
			|| (relExprQueue.front().type == Operator::LESSER) || (relExprQueue.front().type == Operator::LEQ)
			|| (relExprQueue.front().type == Operator::EQUAL) || (relExprQueue.front().type == Operator::NOT_EQUAL))) {
			break;
		}
		relFactorTwoQueue.push(relExprQueue.front());
		relExprQueue.pop();
	}
	relExpr.setRelFactorTwo(ConcreteSyntax::parseRelFactor(relFactorTwoQueue));
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
	else if (relFactorQueue.front().type == NAME) {
		relFactor.setVarName(relFactorQueue.front());
		relFactor.setType(FactorType::VAR);
		relFactorQueue.pop();
	}
	else if (relFactorQueue.front().type == INTEGER) {
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
	ifStmt.cond_expr = ConcreteSyntax::parseCondExpr(tokensQueue);

	// parse then stmtLst
	// remove then_keyword
	tokensQueue.pop();
	// remove left_curly
	tokensQueue.pop();
	StmtLst stmtLst;
	int stmt_count = 1;
	while (tokensQueue.front().type != RIGHT_CURLY) {
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
	while (tokensQueue.front().type != RIGHT_CURLY) {
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
	readStmt.var_name = tokensQueue.front().getId();
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
	printStmt.var_name = tokensQueue.front().getId();
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