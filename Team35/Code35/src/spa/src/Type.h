//
// Created by Juliet Teoh on 2/2/22.
//
#include <map>

#ifndef SPA_TYPE_H
#define SPA_TYPE_H

enum EntityType {
    Assignment,
    Variable,
    If,
    While,
    Procedure,
    Read,
    Print,
    Call,
    Boolean,
    ConstantNumber,
    ConstantString,
    Wildcard
};

enum RelationshipType {
    Modifies,
    Uses,
    Parent,
    ParentT,
    Follows,
    FollowsT
};

class Type{
public:
    static constexpr const char* entityTypeStringArr[] = {"Assignment",
                                            "Variable",
                                            "If",
                                            "While",
                                            "Procedure",
                                            "Read",
                                            "Print",
                                            "Call",
                                            "Boolean",
                                            "ConstantNumber",
                                            "ConstantString",
                                            "Wildcard"};

    static constexpr const char* relationshipTypeStringArr[] = {"Modifies",
                                           "Uses",
                                           "Parent",
                                           "ParentT",
                                           "Follows",
                                           "FollowsT"};
};

#endif //SPA_TYPE_H
