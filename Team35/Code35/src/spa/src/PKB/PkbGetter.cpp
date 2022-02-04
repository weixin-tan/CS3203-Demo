#include "PkbGetter.h"
#include "Type.h"
#include "DB.h"
#include <set>
#include <assert.h>

static std::set<EntityType> statements = {  // this might be better to have in Types
        Assignment,
        Read,
        If,
        While,
        Call
};

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(RelationshipType r, Entity leftSide, Entity rightSide) {
    if (r == Modifies) {
        assert(leftSide.name != "" && statements.find(leftSide.cType) != statements.end()
                && rightSide.name != "" && rightSide.cType == Variable);  // must be (stmt, var), both must be named
        const int& varId = db->varToIntTable[rightSide.name];
        const std::set<int>& modifiedVars = db->modifiesStmtTable[stoi(leftSide.name)];
        return modifiedVars.find(varId) != modifiedVars.end();
    }
    return false;
}
