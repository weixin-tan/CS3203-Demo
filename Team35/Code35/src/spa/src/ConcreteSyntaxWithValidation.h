#include <queue>
#include <stack>
#include <stdexcept>
#include "Program.h"
#include "Expr.h"
#include "CondExpr.h"
#include "TokenType.h"
#include "FactorType.h"

class ConcreteSyntaxWithValidation {
public:
	ConcreteSyntaxWithValidation();
	Program parseProgram(std::queue<Token> tokensQueue);
	Procedure parseProcedure(std::queue<Token> tokensQueue);
	StmtLst parseStmtLst(std::queue<Token>& tokensQueue);
	Statement parseStmt(std::queue<Token>& tokensQueue);
	Statement parseAssign(std::queue<Token>& tokensQueue);
	Expr parseExpr(std::queue<Token>& tokensQueue);
	Expr parseExprRecursion(std::stack<Token>& exprStack);
	Term parseTerm(std::queue<Token>& termQueue);
	Factor parseFactor(std::queue<Token>& factorQueue);
	Statement parseWhile(std::queue<Token>& tokensQueue);
	CondExpr parseCondExpr(std::queue<Token>& tokensQueue);
	CondExpr parseCondExprRecursion(std::queue<Token>& condExprQueue);
	RelExpr parseRelExpr(std::queue<Token>& relExprQueue);
	RelFactor parseRelFactor(std::queue<Token>& relFactorQueue);
	Statement parseIf(std::queue<Token>& tokensQueue);
	Statement parseRead(std::queue<Token>& tokensQueue);
	Statement parsePrint(std::queue<Token>& tokensQueue);
	Statement parseCall(std::queue<Token>& tokensQueue);

	std::vector<std::vector<std::string>> parseExprString(std::queue<Token>& tokensQueue);
	std::vector<std::vector<std::string>> parseCondExprString(std::queue<Token>& tokensQueue);
};