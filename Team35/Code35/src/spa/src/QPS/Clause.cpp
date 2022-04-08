#include "Clause.h"

Clause::Clause() = default;

void Clause::appendEntityToFind(const Entity& e) {
    Clause::entityToFindList.push_back(e);
}

void Clause::appendRef(const RelationshipRef& r) {
    Clause::refList.push_back(r);
}

std::string Clause::toString() {
    std::ostringstream buffer;
    buffer << "entities to find: \n";
    for (const auto& e : entityToFindList) {
        buffer << e.toString();
    }
    buffer << "relationships to satisfy: \n";
    for (const auto& r : refList) {
        buffer << r.toString();
    }
    buffer << std::endl;
    return buffer.str();
}
bool Clause::operator==(const Clause& c1) {
    bool toReturn = true;
    bool temp;
    for (const auto& e : entityToFindList) {
        temp = false;
        for (const auto& e1 : c1.entityToFindList) {
            temp = temp || (e1 == e);
        }
        toReturn = toReturn && temp;
    }
    for (const auto& r : refList) {
        temp = false;
        for (const auto& r1 : c1.refList) {
            temp = temp || (r == r1);
        }
        toReturn = toReturn && temp;
    }
    return toReturn;
}
bool Clause::equals(const Clause& c1) {
    bool toReturn = true;
    bool temp;
    for (const Entity& e : entityToFindList) {
        temp = false;
        for (const auto& e1 : c1.entityToFindList) {
            temp = temp || (e.equals(e1));
        }
        toReturn = toReturn && temp;
    }
    for (const RelationshipRef& r : refList) {
        temp = false;
        for (const auto& r1 : c1.refList) {
            temp = temp || (r.equals(r1));
        }
        toReturn = toReturn && temp;
    }
    return toReturn;
}



