#include "ConcreteSyntaxWithValidation.h"

int stmt_count = 1;

ConcreteSyntaxWithValidation::ConcreteSyntaxWithValidation() {

}

// not used for Iteration 1
Program ConcreteSyntaxWithValidation::parseProgram(std::queue<Token> tokensQueue) {
	Program program;
	ProcedureLst procedureLst;

	int stmt_count = 0;

	while (tokensQueue.size() != 0) {
		Procedure temp_procedure = ConcreteSyntaxWithValidation::parseProcedure(tokensQueue);
		stmt_count += temp_procedure.getSize();
		procedureLst.setNextProcedure(temp_procedure);
	}

	program.setProcedureLst(procedureLst);
	return program;
}

Procedure ConcreteSyntaxWithValidation::parseProcedure(std::queue<Token> tokensQueue) {
	Procedure procedure;

	// procedure_keyword
	if (tokensQueue.front().getToken() != TokenType::PROCEDURE_KEYWORD) {
		throw std::invalid_argument("Missing procedure keyword.");
	}
	tokensQueue.pop();

	// procedure name
	if (tokensQueue.front().getToken() != TokenType::NAME) {
		throw std::invalid_argument("Missing procedure name.");
	}
	procedure.setProcName(tokensQueue.front().getId());
	tokensQueue.pop();

	// left_curly
	if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
		throw std::invalid_argument("Missing left curly for procedure.");
	}
	tokensQueue.pop();

	StmtLst stmtLst;
	try {
		stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
	}
	catch (const std::invalid_argument& e){
		throw;
	}
	stmtLst.SetContainerType(ContainerType::kprocedure);
	procedure.setStmtLst(stmtLst);
	procedure.setSize(stmt_count);

	stmt_count = 1;

	return procedure;
}

StmtLst ConcreteSyntaxWithValidation::parseStmtLst(std::queue<Token>& tokensQueue) {
	StmtLst stmtLst;

	// handle when procedure missing right curly
	if (tokensQueue.empty()) {
		throw std::invalid_argument("Missing right curly for procedure.");
	}

	while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
		Statement temp_statement;
		try {
			temp_statement = ConcreteSyntaxWithValidation::parseStmt(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
		stmtLst.setNextStmt(temp_statement);
	}
	// remove right_curly
	tokensQueue.pop();

	return stmtLst;
}

Statement ConcreteSyntaxWithValidation::parseStmt(std::queue<Token>& tokensQueue) {

	if (tokensQueue.front().getToken() == TokenType::READ_KEYWORD) {
		try {
			return ConcreteSyntaxWithValidation::parseRead(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else if (tokensQueue.front().getToken() == TokenType::PRINT_KEYWORD) {
		try {
			return ConcreteSyntaxWithValidation::parsePrint(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else if (tokensQueue.front().getToken() == TokenType::CALL_KEYWORD) {
		try {
			return ConcreteSyntaxWithValidation::parseCall(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else if (tokensQueue.front().getToken() == TokenType::WHILE_KEYWORD) {
		try {
			return ConcreteSyntaxWithValidation::parseWhile(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else if (tokensQueue.front().getToken() == TokenType::IF_KEYWORD) {
		try {
			return ConcreteSyntaxWithValidation::parseIf(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else if (tokensQueue.front().getToken() == TokenType::NAME) {
		try {
			return ConcreteSyntaxWithValidation::parseAssign(tokensQueue);
		}
		catch (const std::invalid_argument& e) {
			throw;
		}
	}
	else {
		throw std::invalid_argument("Invalid statement declaration keyword.");
	}
}

// parseAssign
Statement ConcreteSyntaxWithValidation::parseAssign(std::queue<Token>& tokensQueue) {
	Statement assignStmt;
	assignStmt.stmt_no = stmt_count;
	stmt_count++;
	assignStmt.statement_type = StatementType::kassign_stmt;

	// var_name
	std::vector<std::string> result;
	if (tokensQueue.front().getToken() != TokenType::NAME) {
		throw std::invalid_argument("Missing variable name.");
	}
	result.push_back(tokensQueue.front().getId());
	assignStmt.var_name = result;
	tokensQueue.pop();

	// assign sign
	if (tokensQueue.front().getToken() != TokenType::ASSIGN) {
		throw std::invalid_argument("Missing equal sign.");
	}
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	try {
		resultString = ConcreteSyntaxWithValidation::parseExprString(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	assignStmt.expr = resultString[0];
	// pass constant
	assignStmt.constant = resultString[1];
	
	// semicolon
	if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		throw std::invalid_argument("Missing semicolon.");
	}
	tokensQueue.pop();

	return assignStmt;
}

// for iteration 1
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseExprString(std::queue<Token>& tokensQueue) {
	std::vector<std::vector<std::string>> result;
	std::vector<std::string> exprVector;
	std::vector<std::string> constVector;
	result.push_back(exprVector);
	result.push_back(constVector);
	while (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		if (tokensQueue.front().getToken() == TokenType::NAME) {
			result[0].push_back(tokensQueue.front().getId());
		}
		else if (tokensQueue.front().getToken() == TokenType::INTEGER) {
			result[1].push_back(tokensQueue.front().getId());
		}
		// other valid symbols in expr
		else if ((tokensQueue.front().getToken() == TokenType::LEFT_BRACE) ||
			(tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) ||
			(tokensQueue.front().getToken() == TokenType::ADD) ||
			(tokensQueue.front().getToken() == TokenType::SUBTRACT) ||
			(tokensQueue.front().getToken() == TokenType::MULTIPLY) ||
			(tokensQueue.front().getToken() == TokenType::DIVIDE) ||
			(tokensQueue.front().getToken() == TokenType::MODULO)) {
			;
		}
		else {
			// throw std::invalid_argument("Invalid symbol in expression.");
			;
		}
		tokensQueue.pop();
	}
	return result;
}

// parseExpr takes inorder, returns reverse
Expr ConcreteSyntaxWithValidation::parseExpr(std::queue<Token>& tokensQueue) {
	std::stack<Token> exprStack;
	while (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		exprStack.push(tokensQueue.front());
		tokensQueue.pop();
	}
	return ConcreteSyntaxWithValidation::parseExprRecursion(exprStack);
}

// parseExprRecursion takes reverse, returns reverse
Expr ConcreteSyntaxWithValidation::parseExprRecursion(std::stack<Token>& exprStack) {
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
	expr.setTerm(ConcreteSyntaxWithValidation::parseTerm(termQueue));

	if (!exprStack.empty()) {
		expr.setOperator(exprStack.top().getToken());
		exprStack.pop();
		expr.setExpr(ConcreteSyntaxWithValidation::parseExprRecursion(exprStack));
	}
	return expr;
}

// parseTerm takes reverse, returns reverse
Term ConcreteSyntaxWithValidation::parseTerm(std::queue<Token>& termQueue) {
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
	term.setFactor(ConcreteSyntaxWithValidation::parseFactor(factorQueue));

	if (!termQueue.empty()) {
		term.setOperator(termQueue.front().getToken());
		termQueue.pop();
		term.setTerm(ConcreteSyntaxWithValidation::parseTerm(termQueue));
	}
	return term;
}

// parseFactor takes reverse, returns reverse
Factor ConcreteSyntaxWithValidation::parseFactor(std::queue<Token>& factorQueue) {
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
Statement ConcreteSyntaxWithValidation::parseWhile(std::queue<Token>& tokensQueue) {
	Statement whileStmt;
	whileStmt.stmt_no = stmt_count;
	stmt_count++;
	whileStmt.statement_type = StatementType::kwhile_stmt;

	// parse cond_expr
	// remove while_keyword
	if (tokensQueue.front().getToken() != TokenType::WHILE_KEYWORD) {
		throw std::invalid_argument("Missing while keyword.");
	}
	tokensQueue.pop();

	// remove left_brace
	if (tokensQueue.front().getToken() != TokenType::LEFT_BRACE) {
		throw std::invalid_argument("Missing left brace.");
	}
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	try {
		resultString = ConcreteSyntaxWithValidation::parseCondExprString(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	whileStmt.cond_expr = resultString[0];
	// pass constant
	whileStmt.constant = resultString[1];

	// parse stmtLst
	// remove left_curly
	if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
		throw std::invalid_argument("Missing left curly.");
	}
	tokensQueue.pop();

	StmtLst stmtLst;
	try {
		stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	stmtLst.SetContainerType(ContainerType::kwhile);
	std::shared_ptr<StmtLst> p = std::make_shared<StmtLst>(stmtLst);
	whileStmt.while_stmt_list = p;
	return whileStmt;
}

// parse cond_expr String for Iteration 1
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseCondExprString(std::queue<Token>& tokensQueue) {
	std::vector<std::vector<std::string>> result;
	std::vector<std::string> exprVector;
	std::vector<std::string> constVector;
	result.push_back(exprVector);
	result.push_back(constVector);
	int closure = 1;
	while (closure != 0) {
		if (tokensQueue.front().getToken() == TokenType::LEFT_BRACE) {
			closure++;
		}
		else if (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) {
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
		else if (tokensQueue.front().getToken() == TokenType::NAME) {
			result[0].push_back(tokensQueue.front().getId());
		}
		else if (tokensQueue.front().getToken() == TokenType::INTEGER) {
			result[1].push_back(tokensQueue.front().getId());
		}
		// invalid symbols in cond_expr
		else if ((tokensQueue.front().getToken() == TokenType::SPECIALCHAR) ||
			(tokensQueue.front().getToken() == TokenType::SEMICOLON) ||
			(tokensQueue.front().getToken() == TokenType::LEFT_CURLY) ||
			(tokensQueue.front().getToken() == TokenType::RIGHT_CURLY) ||
			(tokensQueue.front().getToken() == TokenType::READ_KEYWORD) ||
			(tokensQueue.front().getToken() == TokenType::PRINT_KEYWORD) ||
			(tokensQueue.front().getToken() == TokenType::CALL_KEYWORD) ||
			(tokensQueue.front().getToken() == TokenType::WHILE_KEYWORD) ||
			(tokensQueue.front().getToken() == TokenType::IF_KEYWORD) ||
			(tokensQueue.front().getToken() == TokenType::PROCEDURE_KEYWORD)) {
			throw std::invalid_argument("Invalid symbol.");
		}

		tokensQueue.pop();
	}
	return result;
}

// parse cond_expr
CondExpr ConcreteSyntaxWithValidation::parseCondExpr(std::queue<Token>& tokensQueue) {
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
	return ConcreteSyntaxWithValidation::parseCondExprRecursion(condExprQueue);
}

// parse cond_expr recursive
CondExpr ConcreteSyntaxWithValidation::parseCondExprRecursion(std::queue<Token>& condExprQueue) {
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
	condExpr.setRelExpr(ConcreteSyntaxWithValidation::parseRelExpr(relExprQueue));

	if (!condExprQueue.empty()) {
		condExpr.setOperator(condExprQueue.front().getToken());
		condExprQueue.pop();
		condExpr.setCondExpr(ConcreteSyntaxWithValidation::parseCondExprRecursion(condExprQueue));
	}
	return condExpr;
}

// parse rel_expr
RelExpr ConcreteSyntaxWithValidation::parseRelExpr(std::queue<Token>& relExprQueue) {
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
	relExpr.setRelFactorOne(ConcreteSyntaxWithValidation::parseRelFactor(relFactorOneQueue));

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
	relExpr.setRelFactorTwo(ConcreteSyntaxWithValidation::parseRelFactor(relFactorTwoQueue));
	return relExpr;
}

// parse rel_factor
RelFactor ConcreteSyntaxWithValidation::parseRelFactor(std::queue<Token>& relFactorQueue) {
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
Statement ConcreteSyntaxWithValidation::parseIf(std::queue<Token>& tokensQueue) {
	Statement ifStmt;
	ifStmt.stmt_no = stmt_count;
	stmt_count++;
	ifStmt.statement_type = StatementType::kif_stmt;

	// parse cond_expr
	// remove if_keyword
	if (tokensQueue.front().getToken() != TokenType::IF_KEYWORD) {
		throw std::invalid_argument("Missing if keyword.");
	}
	tokensQueue.pop();
	// remove left_brace
	if (tokensQueue.front().getToken() != TokenType::LEFT_BRACE) {
		throw std::invalid_argument("Missing left brace.");
	}
	tokensQueue.pop();

	// Iteration 1 only passing vector of string
	std::vector<std::vector<std::string>> resultString;
	try {
		resultString = ConcreteSyntaxWithValidation::parseCondExprString(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	ifStmt.cond_expr = resultString[0];
	// pass constant
	ifStmt.constant = resultString[1];

	// parse then stmtLst
	// remove then_keyword
	if (tokensQueue.front().getToken() != TokenType::THEN_KEYWORD) {
		throw std::invalid_argument("Missing then keyword.");
	}
	tokensQueue.pop();
	// remove left_curly
	if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
		throw std::invalid_argument("Missing left curly.");
	}
	tokensQueue.pop();

	StmtLst stmtLst;
	try {
		stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	// set then stmtLst
	stmtLst.SetContainerType(ContainerType::kifthen);
	ifStmt.ifthen_stmt_list = &stmtLst;

	// parse else stmtLst
	// remove else_keyword
	if (tokensQueue.front().getToken() != TokenType::ELSE_KEYWORD) {
		throw std::invalid_argument("Missing else keyword.");
	}
	tokensQueue.pop();
	// remove left_curly
	if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
		throw std::invalid_argument("Missing left curly.");
	}
	tokensQueue.pop();

	StmtLst stmtLstElse;
	try {
		stmtLstElse = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
	}
	catch (const std::invalid_argument& e) {
		throw;
	}
	// set else stmtLst
	stmtLstElse.SetContainerType(ContainerType::kifelse);
	ifStmt.ifelse_stmt_list = &stmtLstElse;

	return ifStmt;
}
// end for parsing If/Else
// done

// start for parsing Read
Statement ConcreteSyntaxWithValidation::parseRead(std::queue<Token>& tokensQueue) {
	Statement readStmt;
	readStmt.stmt_no = stmt_count;
	stmt_count++;
	readStmt.statement_type = StatementType::kread_stmt;
	// remove read_keyword
	if (tokensQueue.front().getToken() != TokenType::READ_KEYWORD) {
		throw std::invalid_argument("Missing read keyword.");
	}
	tokensQueue.pop();
	// Iteration 1 only passing vector of string
	std::vector<std::string> result;
	if (tokensQueue.front().getToken() != TokenType::NAME) {
		throw std::invalid_argument("Missing variable name.");
	}
	result.push_back(tokensQueue.front().getId());
	readStmt.var_name = result;
	// remove var_name
	tokensQueue.pop();
	// remove semicolon
	if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		throw std::invalid_argument("Missing semicolon.");
	}
	tokensQueue.pop();
	return readStmt;
}
// end for parsing Read

// start for parsing Print
Statement ConcreteSyntaxWithValidation::parsePrint(std::queue<Token>& tokensQueue) {
	Statement printStmt;
	printStmt.stmt_no = stmt_count;
	stmt_count++;
	printStmt.statement_type = StatementType::kprint_stmt;
	// remove print_keyword
	if (tokensQueue.front().getToken() != TokenType::PRINT_KEYWORD) {
		throw std::invalid_argument("Missing print keyword.");
	}
	tokensQueue.pop();
	// Iteration 1 only passing vector of string
	std::vector<std::string> result;
	if (tokensQueue.front().getToken() != TokenType::NAME) {
		throw std::invalid_argument("Missing variable name.");
	}
	result.push_back(tokensQueue.front().getId());
	printStmt.var_name = result;
	// remove var_name
	tokensQueue.pop();
	// remove semicolon
	if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
		throw std::invalid_argument("Missing semicolon.");
	}
	tokensQueue.pop();
	return printStmt;
}
// end for parsing Print

// not used for Iteration 1
// start for parsing Call
Statement ConcreteSyntaxWithValidation::parseCall(std::queue<Token>& tokensQueue) {
	Statement callStmt;
	callStmt.stmt_no = stmt_count;
	stmt_count++;
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