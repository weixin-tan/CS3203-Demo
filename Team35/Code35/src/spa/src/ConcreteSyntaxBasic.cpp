#include "ConcreteSyntaxBasic.h"

ConcreteSyntaxBasic::ConcreteSyntaxBasic() {

}

Procedure ConcreteSyntaxBasic::parseProcedure(std::queue<Token> tokensQueue) {
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

	stmtLst.SetContainerType(kprocedure);
	procedure.setStmtLst(stmtLst);
	return procedure;
}

Statement ConcreteSyntaxBasic::parseStmt(std::queue<Token> &tokensQueue) {
	if (tokensQueue.front().type == NAME) {
		return ConcreteSyntaxBasic::parseAssign(tokensQueue);
	}
}

Statement ConcreteSyntaxBasic::parseAssign(std::queue<Token> &tokensQueue) {
	Statement assignStmt;
	assignStmt.statement_type = kassign_stmt;

	std::vector<std::string> var_name_result;
	var_name_result.push_back(tokensQueue.front().getId());
	assignStmt.var_name = var_name_result;
	// var_name
	tokensQueue.pop();
	// equals sign
	tokensQueue.pop();
	assignStmt.expr = ConcreteSyntaxBasic::parseExpr(tokensQueue);
	// semicolon
	tokensQueue.pop();
	return assignStmt;
}

std::vector<std::string> ConcreteSyntaxBasic::parseExpr(std::queue<Token> &tokensQueue) {
	std::vector<std::string> result;
	while (tokensQueue.front().type != SEMICOLON) {
		if (tokensQueue.front().type == NAME) {
			result.push_back(tokensQueue.front().getId());
		}
		tokensQueue.pop();
	}
	return result;
}