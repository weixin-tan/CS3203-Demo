#include "QueryProcessor.h"

QueryProcessor::QueryProcessor() = default;

/**
 * Create entities from strings in designEntityArr and add them in entityMap
 * @param designEntityArr List where the first element is a design-entity and the rest are synonym
 * @param entityMap entityMap to add entity objects to
 */
void QueryProcessor::createDeclarationObjects(std::vector<std::string>* designEntityArr,
                                              std::unordered_map<std::string, Entity>* entityMap) {
    std::vector<Entity> returnList;
    std::string designEntity = (*designEntityArr)[0];
    EntityType eType;
    if (designEntity == "stmt") {
        eType = EntityType::STATEMENT;
    } else if (designEntity == "read") {
        eType = EntityType::READ;
    } else if (designEntity == "print") {
        eType = EntityType::PRINT;
    } else if (designEntity == "call") {
        eType = EntityType::CALL;
    } else if (designEntity == "while") {
        eType = EntityType::WHILE;
    } else if (designEntity == "if") {
        eType = EntityType::IF;
    } else if (designEntity == "assign") {
        eType = EntityType::ASSIGNMENT;
    } else if (designEntity == "variable") {
        eType = EntityType::VARIABLE;
    } else if (designEntity == "constant") {
        eType = EntityType::CONSTANT;
    } else if (designEntity == "procedure") {
        eType = EntityType::PROCEDURE;
    } else {
        // this entity is NULL -> invalid
        eType = EntityType::NULL_ENTITY;
    }
    for (int i = 1; i < (*designEntityArr).size(); i++) {
        std::string entityName = (*designEntityArr)[i];
        Entity newEntity = Entity(eType, entityName);
        (*entityMap)[entityName] = newEntity;
    }
}

/**
 * Create relationship objects from strings and returns a RelationshipRef object
 * @param relRefList List where the first element is a relRef and the rest are either stmtRef or entRef
 * @param entityMap entityMap to reference entity objects from
 * @return Created RelationshipRef object
 */
RelationshipRef QueryProcessor::createRelationshipObject(std::vector<std::string>* relRefList,
                                                         std::unordered_map<std::string, Entity>* entityMap) {
    std::string relStr = (*relRefList)[0];
    RelationshipType rType;
    if (relStr == "Follows") {
        rType = RelationshipType::FOLLOWS;
    } else if (relStr == "Follows*") {
        rType = RelationshipType::FOLLOWS_T;
    } else if (relStr == "Parent") {
        rType = RelationshipType::PARENT;
    } else if (relStr == "Parent*") {
        rType = RelationshipType::PARENT_T;
    } else if (relStr == "Uses") {
        rType = RelationshipType::USES;
    } else if (relStr == "Modifies") {
        rType = RelationshipType::MODIFIES;
    } else if (relStr == "Calls") {
        rType = RelationshipType::CALLS;
    } else if (relStr == "Calls*") {
        rType = RelationshipType::CALLS_T;
    } else if (relStr == "Next") {
        rType = RelationshipType::NEXT;
    } else if (relStr == "Next*") {
        rType = RelationshipType::NEXT_T;
    } else if (relStr == "Affects") {
        rType = RelationshipType::AFFECTS;
    } else if (relStr == "Affects*") {
        rType = RelationshipType::AFFECTS_T;
    } else {
        //this relationship type is NULL -> invalid
        rType = RelationshipType::NULL_RELATIONSHIP;
    }
    Entity leftEntity = findRelationshipEntity((*relRefList)[1], entityMap);
    Entity rightEntity = findRelationshipEntity((*relRefList)[2], entityMap);

    if (rType == RelationshipType::NULL_RELATIONSHIP || leftEntity.eType == EntityType::NULL_ENTITY
            || rightEntity.eType == EntityType::NULL_ENTITY) {
        return RelationshipRef(RelationshipType::NULL_RELATIONSHIP, leftEntity, rightEntity);
    } else {
        return RelationshipRef(rType, leftEntity, rightEntity);
    }
}

/**
 * Creates a entity object based on the string
 * @param s string representing a stmtRef or entRef
 * @param entityMap entityMap to reference entity objects from
 * @return Created Entity object
 */
Entity QueryProcessor::findRelationshipEntity(const std::string& s,
                                              std::unordered_map<std::string, Entity>* entityMap) {
    if (entityMapContains(s, entityMap)) {
        return (*entityMap)[s];
    } else if (isWildCard(s)) {
        return Entity(EntityType::WILDCARD, "_");
    } else if (isInteger(s)) {
        return Entity(EntityType::FIXED_INTEGER, s);
    } else if (isQuotationIdent(s)) {
        std::string s2 = extractStringFromQuotation(s);
        return Entity(EntityType::FIXED_STRING, s2);
    } else {
        // this entity is NULL -> invalid
        return Entity(EntityType::NULL_ENTITY, s);
    }
}

/**
 * Create PATTERN relationship objects from strings and returns a RelationshipRef object
 * @param patternList List where the first element is a syn-assign, the second element is a stmtRef or entRef and the third element is an expression
 * @param entityMap entityMap to reference entity objects from
 * @return Created PATTERN RelationshipRef object
 */
RelationshipRef QueryProcessor::createPatternObject(std::vector<std::string>* patternList,
                                                    std::unordered_map<std::string, Entity>* entityMap) {
    RelationshipType rType = RelationshipType::PATTERN;
    Entity assignmentEntity = findRelationshipEntity((*patternList)[0], entityMap);
    Entity leftEntity = findRelationshipEntity((*patternList)[1], entityMap);
    Entity rightEntity = createExpressionEntity((*patternList)[2]);

    if (assignmentEntity.eType == EntityType::NULL_ENTITY || leftEntity.eType == EntityType::NULL_ENTITY
            || rightEntity.eType == EntityType::NULL_ENTITY) {
        return RelationshipRef(RelationshipType::NULL_RELATIONSHIP, leftEntity, rightEntity);
    } else {
        return RelationshipRef(rType, leftEntity, rightEntity, assignmentEntity);
    }
}

/**
 * Create expression entity object from strings
 * @param s string representing an expression
 * @return Created Entity object
 */
Entity QueryProcessor::createExpressionEntity(const std::string& s) {
    std::string s2;
    if (isWildCard(s)) {
        return Entity(EntityType::WILDCARD, "_");
    } else if (isStringWithinQuotations(s)) {
        s2 = extractStringFromQuotation(s);
        return Entity(EntityType::FIXED_STRING, s2);
    } else if (isStringWithinWildCard(s)) {
        s2 = extractStringFromWildCard(s);
        return Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, s2);
    } else {
        // this entity is NULL -> invalid
        return Entity(EntityType::NULL_ENTITY, s);
    }
}

Entity QueryProcessor::findRelationshipEntityWithAttribute(const std::string& s,
                                                           std::unordered_map<std::string, Entity>* entityMap) {
    std::vector<std::string> tempArr = splitString(s, ".");
    if (tempArr.size() == 1) {
        return findRelationshipEntity(s, entityMap);
    } else if (tempArr.size() == 2) {
        std::string entityName = tempArr[0];
        std::string attributeName = tempArr[1];

        Entity basicEntity = findRelationshipEntity(entityName, entityMap);
        EntityType eType = basicEntity.eType;
        std::string name = basicEntity.name;
        EntityAttributeType aType;

        // 'procName'| 'varName' | 'value' | 'stmt#'
        if (attributeName == "procName") {
            aType = EntityAttributeType::PROCNAME;
        } else if (attributeName == "varName") {
            aType = EntityAttributeType::VARNAME;
        } else if (attributeName == "value") {
            aType = EntityAttributeType::VALUE;
        } else if (attributeName == "stmt#") {
            aType = EntityAttributeType::STMT;
        } else {
            aType = EntityAttributeType::NULL_ATTRIBUTE;
            eType = EntityType::NULL_ENTITY;
        }
        return Entity(eType, name, aType);
    } else {
        return Entity(EntityType::NULL_ENTITY, s);
    }
}

RelationshipRef QueryProcessor::createWithObject(std::vector<std::string>* clauseList,
                                                 std::unordered_map<std::string, Entity>* entityMap) {
    RelationshipType rType = RelationshipType::WITH;
    Entity leftEntity = findRelationshipEntityWithAttribute((*clauseList)[0], entityMap);
    Entity rightEntity = findRelationshipEntityWithAttribute((*clauseList)[1], entityMap);
    if (leftEntity.eType == EntityType::NULL_ENTITY || rightEntity.eType == EntityType::NULL_ENTITY) {
        return RelationshipRef(RelationshipType::NULL_RELATIONSHIP, leftEntity, rightEntity);
    } else {
        return RelationshipRef(rType, leftEntity, rightEntity);
    }
}

void QueryProcessor::addIfNotDuplicate(Clause* newClause, const RelationshipRef& newRef) {
    if (!checkAlreadyInClause((*newClause).refList, newRef)) {
        (*newClause).appendRef(newRef);
    }
}

void QueryProcessor::handleVariablesToFind(std::vector<std::string>* variablesToSelect, Clause* newClause,
                                           bool* isValid, std::unordered_map<std::string, Entity>* entityMap) {
    bool containsBoolean = false;
    for (const auto& s : (*variablesToSelect)) {
        Entity toAdd = findRelationshipEntityWithAttribute(s, entityMap);
        (*newClause).appendEntityToFind(toAdd);
        (*isValid) = (*isValid) && checkVariableToSelect(toAdd);
        if (toAdd.eType == EntityType::BOOLEAN) {
            containsBoolean = true;
        }
    }
    if (containsBoolean && (*newClause).entityToFindList.size() > 1) {
        (*isValid) = false;
    }
}

void QueryProcessor::handleSuchThat(std::vector<std::string>* SuchThatClauses, Clause* newClause,
                                    bool* isValid, bool* isSemanticallyValid,
                                    std::unordered_map<std::string, Entity>* entityMap) {
    for (const auto& s : *SuchThatClauses) {
        std::vector<std::string> relRefList = extractItemsInBrackets(s);
        (*isValid) = (*isValid) && checkRelRefList(&relRefList);
        if ((*isValid)) {
            RelationshipRef newRef = createRelationshipObject(&relRefList, entityMap);
            (*isSemanticallyValid) = (*isSemanticallyValid) && checkRelationshipRef(newRef);
            addIfNotDuplicate(newClause, newRef);
        }
    }
}

void QueryProcessor::handlePattern(std::vector<std::string>* PatternClauses, Clause* newClause,
                                   bool* isValid, bool* isSemanticallyValid,
                                   std::unordered_map<std::string, Entity>* entityMap) {
    for (const auto& s : *PatternClauses) {
        std::vector<std::string> patternList = extractPatternBrackets(s);
        (*isValid) = (*isValid) && checkPatternList(&patternList);
        if (*isValid) {
            RelationshipRef newRef = createPatternObject(&patternList, entityMap);
            (*isValid) = (*isValid) && checkPatternSyntax(&patternList, entityMap, newRef);
            (*isSemanticallyValid) = (*isSemanticallyValid) && checkRelationshipRef(newRef);
            addIfNotDuplicate(newClause, newRef);
        }
    }
}

void QueryProcessor::handleWith(std::vector<std::string>* WithClauses, Clause* newClause,
                                bool* isValid, bool* isSemanticallyValid,
                                std::unordered_map<std::string, Entity>* entityMap) {
    for (const auto& s : *WithClauses) {
        std::vector<std::string> clausesList = splitStringBySpaces(s);
        (*isValid) = (*isValid) && clausesList.size() == 2;
        if ((*isValid)) {
            RelationshipRef newRef = createWithObject(&clausesList, entityMap);
            (*isSemanticallyValid) = (*isSemanticallyValid) && checkRelationshipRef(newRef);
            addIfNotDuplicate(newClause, newRef);
        }
    }
}

/**
 * Translate query string into a list of clauses
 * @param queryString query string
 * @return a list of clauses representing the query
 */
std::vector<Clause> QueryProcessor::parsePQL(const std::string& queryString) {
    bool isValid = !checkLastCharIsSemiColon(queryString);
    bool isSemanticallyValid = true;
    std::vector<Clause> clauseList;
    std::unordered_map<std::string, Entity> entityMap;

    std::vector<std::vector<std::string>> fullList = extractSelectAndDeclarationStatements(queryString);
    std::vector<std::string> selectStmtList = fullList[0];
    std::vector<std::string> declarationStmtList = fullList[1];

    for (const auto& declarationStmt : declarationStmtList) {
        std::vector<std::string> designEntityArr = extractDesignEntityAndSynonyms(declarationStmt);
        isValid = isValid && checkDesignEntitySynonymsList(&designEntityArr, &entityMap);
        isSemanticallyValid = isSemanticallyValid && checkDeclarationSemanticallyValid(&designEntityArr, &entityMap);
        if (isValid) {
            createDeclarationObjects(&designEntityArr, &entityMap);
        }
    }

    if (!entityMapContains("BOOLEAN", &entityMap)) {
        entityMap["BOOLEAN"] = Entity(EntityType::BOOLEAN, "BOOLEAN");
    }

    if (!isValid) {
        std::vector<Clause> emptyClause;
        return emptyClause;
    }

    for (const auto& selectStmt : selectStmtList) {
        Clause newClause = Clause();
        std::vector<std::vector<std::string>> allList = extractClauses(selectStmt);
        std::vector<std::string> variablesToSelect = extractVariablesToSelect(selectStmt);
        std::vector<std::string> SuchThatClauses = allList[0];
        std::vector<std::string> PatternClauses = allList[1];
        std::vector<std::string> WithClauses = allList[2];

        if (existSuchThat(selectStmt)) {
            isValid = (!SuchThatClauses.empty());
        } else {
            isValid = (SuchThatClauses.empty());
        }
        isValid = isValid && checkAndSuchThat(SuchThatClauses);
        removeAndSuchThat(&SuchThatClauses);
        isValid = isValid && checkAndPattern(PatternClauses);
        removeAndPattern(&PatternClauses);
        isValid = isValid && checkAndWith(WithClauses);
        removeAndWith(&WithClauses);
        if (!isValid) {
            break;
        }

        handleVariablesToFind(&variablesToSelect, &newClause, &isValid, &entityMap);

        if (!isValid) {
            break;
        }

        handleSuchThat(&SuchThatClauses, &newClause,
                       &isValid, &isSemanticallyValid, &entityMap);

        if (!isValid) {
            break;
        }

        handlePattern(&PatternClauses, &newClause,
                      &isValid, &isSemanticallyValid, &entityMap);

        if (!isValid) {
            break;
        }

        handleWith(&WithClauses, &newClause,
                   &isValid, &isSemanticallyValid, &entityMap);

        if (!isValid) {
            break;
        }
        clauseList.push_back(newClause);

        if (!isValid) {
            break;
        }
    }

    std::vector<Clause> emptyClause;
    if (!isValid) {
        return emptyClause;
    } else if (isSemanticallyValid) {
        return clauseList;
    } else {
        // not semantically valid
        if (clauseList[0].entityToFindList[0].eType == EntityType::BOOLEAN) {
            clauseList[0].entityToFindList[0].name = "FALSE";
            return clauseList;
        } else {
            return emptyClause;
        }
    }
}