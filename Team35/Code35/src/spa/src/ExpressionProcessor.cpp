#include "ExpressionProcessor.h"
#include "ConcreteSyntaxWithValidation.h"
#include "Tokeniser.h"
#include <iostream>

// code to be refactored
Expr ExpressionProcessor::stringToExpr(std::string query) const {
	query = query + ";";
	Tokeniser tokeniser;
	std::queue<Token> tokenQueue = tokeniser.putInQueue(query);
	ConcreteSyntaxWithValidation validator;
	return validator.parseExpr(tokenQueue);
}

// Checks if exp1 is a subset of exp2. 
bool ExpressionProcessor::fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) const {
	if (expressionIndicator == ExpressionIndicator::FULL_MATCH) {
		return areIdenticalExpr(&exp1, &exp2);
	}
	else {
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
		}
		else {
			Term* term1 = exp1.getTermPtr();
			Term* term2 = exp2.getTermPtr();
			return isSubtree(term1, term2);
		}
	}
}

// We can only check whether expressions are similar. 
// returns True or False if the expressions are similar. 
bool ExpressionProcessor::areIdenticalExpr(Expr *root1, Expr *root2) const {
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
std::shared_ptr<Expr> ExpressionProcessor::getNestedExpr(Term* root2) const
{
	if (root2->getFactorPtr() != nullptr) {
		Factor* factor = root2->getFactorPtr();
		if (factor->hasExpr()) {
			return factor->getExpr();
		}
	}
	
	return std::shared_ptr<Expr>(nullptr);;
}