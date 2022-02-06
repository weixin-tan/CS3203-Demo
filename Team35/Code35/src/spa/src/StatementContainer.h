#ifndef STATEMENTCONTAINER_H  // idempotence - keep header from being included multiple times
#define STATEMENTCONTAINER_H

#include <iostream>
#include <string>
#include <vector>

#include "Statement.h"

enum ContainerType {
	kprocedure,
	kifthen,
	kwhile,
	kifelse
};

class Statement;

class StatementContainer {
public:
	ContainerType container_type;
	std::vector<Statement> stmt_list;
};

#endif