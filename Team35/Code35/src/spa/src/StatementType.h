#include<stdio.h>

#ifndef STATEMENTTYPE_H
#define STATEMENTTYPE_H

enum StatementType {
	kassign_stmt,
	kprint_stmt,
	kcall_stmt,
	kif_stmt,
	kwhile_stmt,
	kread_stmt,
	kprocedure_stmt
};

#endif