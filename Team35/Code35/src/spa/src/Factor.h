#ifndef FACTOR_H
#define FACTOR_H

enum Type{VAR, CONST, EXPR};

class Factor {
private:
	Type type;
	Token varName;
	Token constValue;
	Expr expr;
public:
	Factor();
	Type getType() const;
	Token getVarName() const;
	Token getConstValue() const;
	Expr getExpr() const;
	void setType(Type type);
	void setVarName(Token varName);
	void setConstValue(Token constValue);
	void setExpr(Expr expr);
};

#endif