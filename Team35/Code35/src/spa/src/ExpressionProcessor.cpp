#include "ExpressionProcessor.h"

Expr ExpressionProcessor::stringToExpr(std::string query) {
	return Expr(); 
}

bool ExpressionProcessor::fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) {
	if (expressionIndicator == ExpressionIndicator::FULL_MATCH) {
		return areIdenticalExpr(&exp1, &exp2);
	}
	else {
		return isSubtree(&exp1, &exp2);
	}
}

// We can only check whether expressions are similar. 
// returns True or False if the expressions are similar. 
bool ExpressionProcessor::areIdenticalExpr(Expr *root1, Expr *root2) {
	if (root1 == nullptr && root2 == nullptr) {
		return true; 
	}
	if (root1 == nullptr || root2 == nullptr) {
		return false;
	}
	return root1->getOperator() == root2->getOperator() &&
		areIdenticalExpr(&root1->getExpr(), &root2->getExpr()) &&
		areIdenticalTerm(&root1->getTerm(), &root2->getTerm());
}

bool ExpressionProcessor::areIdenticalTerm(Term* root1, Term* root2) {
	if (root1 == nullptr && root2 == nullptr) {
		return true;
	}
	if (root1 == nullptr || root2 == nullptr) {
		return false;
	}
	return root1->getOperator() == root2->getOperator() &&
		areIdenticalFactor(&root1->getFactor(), &root2->getFactor()) &&
		areIdenticalTerm(&root1->getTerm(), &root2->getTerm());

}

bool ExpressionProcessor::areIdenticalFactor(Factor* root1, Factor* root2) {
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
		areIdenticalExpr(root1->getExpr(), root2->getExpr());
}

// Checking if they are partial match or not. 
bool ExpressionProcessor::isSubtree(Expr*root1, Expr* root2) {
	if (root2 == nullptr) {
		return true; 
	}
	if (root1 == nullptr) {
		return false; 
	}
	if (areIdenticalExpr(root1, root2)) {
		return true;
	} else {

		return isSubtree(root1, &root2->getExpr()) || 
		// We need to get the NEXT expression out. 
		isSubtree(root1, getNestedExpr(root2));
	}
}

Expr* ExpressionProcessor::getNestedExpr(Expr* root2)
{
	if (&root2->getTerm() != nullptr) {
		Term* term = &root2->getTerm();
		if (&term->getFactor() != nullptr) {
			Factor* factor = &term->getFactor();
			if (factor->getExpr() != nullptr) {
				return factor->getExpr();
			}
		}
	}
	return nullptr;
}
