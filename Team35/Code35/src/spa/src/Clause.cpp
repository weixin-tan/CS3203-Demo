#include "Clause.h"

Clause::Clause() {

};

void Clause::appendEntityToFind(Entity e) {
    Clause::entityToFindList.push_back(e);
};

void Clause::appendRef(RelationshipRef r) {
    Clause::refList.push_back(r);
}

std::string Clause::toString() {
    std::ostringstream buffer;

    buffer << "entities to find: \n";
    for (auto e: entityToFindList) {
        buffer << e.toString();
    };

    buffer << "relationships to satisfy: \n";
    for (auto r: refList){
        buffer << r.toString();
    };

    buffer << std::endl;
    return buffer.str();
};
