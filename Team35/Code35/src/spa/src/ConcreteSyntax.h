#include <queue>
#include <stack>
#include "Program.h"

class ConcreteSyntax {
public:
	ConcreteSyntax();
	Program parseProgram(std::queue<Token> tokensQueue);
	Procedure parseProcedure(std::queue<Token> tokensQueue);
	Stmt parseStmt(std::queue<Token> tokensQueue);
	AssignStmt parseAssign(std::queue<Token> tokensQueue);
	Expr parseExpr(std::queue<Token> tokensQueue, AssignStmt assignStmt);
	Expr parseExprRecursion(std::stack<Token> exprStack);
	Term parseTerm(std::queue<Token> tokensQueue);
	Expr parseTermRecursion(std::stack<Token> exprStack);
	Factor parseFactor(std::stack<Token> factorStack);

};