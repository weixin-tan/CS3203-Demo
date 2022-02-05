#include <queue>
#include <stack>
#include <vector>
#include "Procedure.h"
#include "Statement.h"
#include "Token.h"

class ConcreteSyntaxBasic {
public:
	ConcreteSyntaxBasic();
	Procedure parseProcedure(std::queue<Token> &tokensQueue);
	Statement parseStmt(std::queue<Token> &tokensQueue);
	Statement parseAssign(std::queue<Token> &tokensQueue);
	std::vector<std::string> parseExpr(std::queue<Token> &tokensQueue);
};