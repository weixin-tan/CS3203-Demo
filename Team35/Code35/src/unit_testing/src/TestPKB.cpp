#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"
#include "PkbRelationshipType.h"

#include "catch.hpp"


TEST_CASE("PKB Basic") {
    PKB pkb = PKB();
    PkbGetter* pkb_getter = pkb.getGetter();
    PkbSetter* pkb_setter = pkb.getSetter();

    REQUIRE(pkb_getter != NULL);
    REQUIRE(pkb_setter != NULL);

//procedure f {
//  x = 5;  // 1
//  y = x;  // 2
//  x = 5;  // 3
//  while (x > 4) {  // 4
//    read z;  // 5
//    if (z < 10) then {  // 6
//      x = 100;  // 7
//      z = 5;  // 8
//    } else {
//      y = 100;  // 9
//      z = 0;  // 10
//    }
//  }
//  print x; // 11
//}

    std::vector<std::vector<ParsedStatement>> statements = {
            {
                    ParsedStatement(1,
                                    ParsedStatement::default_null_stmt_no,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"x"},
                                    {"5"},
                                    ParsedStatement::default_procedure_name,
                                    ParsedStatement::default_null_stmt_no),
                    ParsedStatement(2,
                                    ParsedStatement::default_null_stmt_no,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {"x"},
                                    {"y"},
                                    {},
                                    ParsedStatement::default_procedure_name,
                                    1),
                    ParsedStatement(3,
                                    ParsedStatement::default_null_stmt_no,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"x"},
                                    {"5"},
                                    ParsedStatement::default_procedure_name,
                                    2),
                    ParsedStatement(4,
                                    ParsedStatement::default_null_stmt_no,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kwhile_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {"x"},
                                    {},
                                    {"4"},
                                    ParsedStatement::default_procedure_name,
                                    3),
                    ParsedStatement(5,
                                    ParsedStatement::default_null_stmt_no,
                                    4,
                                    StatementType::kread_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"z"},
                                    {},
                                    ParsedStatement::default_procedure_name,
                                    ParsedStatement::default_null_stmt_no),
                    ParsedStatement(6,
                                    ParsedStatement::default_null_stmt_no,
                                    4,
                                    StatementType::kif_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {"z"},
                                    {},
                                    {"10"},
                                    ParsedStatement::default_procedure_name,
                                    5),
                    ParsedStatement(7,
                                    6,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"x"},
                                    {"100"},
                                    ParsedStatement::default_procedure_name,
                                    ParsedStatement::default_null_stmt_no),
                    ParsedStatement(8,
                                    6,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"z"},
                                    {"5"},
                                    ParsedStatement::default_procedure_name,
                                    7),
                    ParsedStatement(9,
                                    6,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"y"},
                                    {"100"},
                                    ParsedStatement::default_procedure_name,
                                    ParsedStatement::default_null_stmt_no),
                    ParsedStatement(10,
                                    6,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kassign_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {},
                                    {"z"},
                                    {"0"},
                                    ParsedStatement::default_procedure_name,
                                    9),
                    ParsedStatement(11,
                                    ParsedStatement::default_null_stmt_no,
                                    ParsedStatement::default_null_stmt_no,
                                    StatementType::kprint_stmt,
                                    ParsedStatement::default_pattern,
                                    "f",
                                    {"x"},
                                    {},
                                    {},
                                    ParsedStatement::default_procedure_name,
                                    4),
            }
    };

    pkb_setter->insertStmts(statements);

    SECTION("General") {
        SECTION("getEntity") {
            SECTION("Statements") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kStatement);
                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kStatement, 1),
                        ProgramElement::createStatement(ElementType::kStatement, 2),
                        ProgramElement::createStatement(ElementType::kStatement, 3),
                        ProgramElement::createStatement(ElementType::kStatement, 4),
                        ProgramElement::createStatement(ElementType::kStatement, 5),
                        ProgramElement::createStatement(ElementType::kStatement, 6),
                        ProgramElement::createStatement(ElementType::kStatement, 7),
                        ProgramElement::createStatement(ElementType::kStatement, 8),
                        ProgramElement::createStatement(ElementType::kStatement, 9),
                        ProgramElement::createStatement(ElementType::kStatement, 10),
                        ProgramElement::createStatement(ElementType::kStatement, 11),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("y"),
                        ProgramElement::createVariable("z"),
                };

                REQUIRE(result == expected);
            }
            SECTION("Assignments") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kAssignment);
                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kAssignment, 1),
                        ProgramElement::createStatement(ElementType::kAssignment, 2),
                        ProgramElement::createStatement(ElementType::kAssignment, 3),
                        ProgramElement::createStatement(ElementType::kAssignment, 7),
                        ProgramElement::createStatement(ElementType::kAssignment, 8),
                        ProgramElement::createStatement(ElementType::kAssignment, 9),
                        ProgramElement::createStatement(ElementType::kAssignment, 10),
                };

                REQUIRE(result == expected);
            }
            SECTION("Ifs") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kIf);
                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kIf, 6)};

                REQUIRE(result == expected);
            }
            SECTION("Whiles") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kWhile);
                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};

                REQUIRE(result == expected);
            }
            SECTION("Read") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kRead);
                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kRead, 5)};

                REQUIRE(result == expected);
            }
            SECTION("Print") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kPrint);
                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kPrint, 11)};

                REQUIRE(result == expected);
            }
        }
    }
    SECTION("Modifies") {
        SECTION("isRelationship") {
            SECTION("Statement and Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kModifies,
                                                   ProgramElement::createStatement(ElementType::kAssignment, 1),
                                                   ProgramElement::createVariable("x"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kModifies,
                                                   ProgramElement::createStatement(ElementType::kAssignment, 2),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kModifies,
                                                   ProgramElement::createStatement(ElementType::kWhile, 4),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kModifies,
                                                   ProgramElement::createStatement(ElementType::kIf, 6),
                                                   ProgramElement::createVariable("y"))
                                == true);
            }
            SECTION("Procedure and Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kModifies,
                                                   ProgramElement::createProcedure("f"),
                                                   ProgramElement::createVariable("z")));
            }
        }
        SECTION("getLeftSide") {
            SECTION("Assignment given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("x"), ElementType::kAssignment);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kAssignment, 1),
                        ProgramElement::createStatement(ElementType::kAssignment, 3),
                        ProgramElement::createStatement(ElementType::kAssignment, 7),
                };

                REQUIRE(result == expected);
            }
            SECTION("If given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("x"), ElementType::kIf);

                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kIf, 6)};
                REQUIRE(result == expected);
            }
            SECTION("While given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("x"), ElementType::kWhile);

                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};
                REQUIRE(result == expected);
            }
            SECTION("Statement given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("z"), ElementType::kStatement);
                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kStatement, 4),
                        ProgramElement::createStatement(ElementType::kStatement, 5),
                        ProgramElement::createStatement(ElementType::kStatement, 6),
                        ProgramElement::createStatement(ElementType::kStatement, 8),
                        ProgramElement::createStatement(ElementType::kStatement, 10),
                };
                REQUIRE(result == expected);
            }
        }
        SECTION("getRightSide") {
            SECTION("Variables given Assignment") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kAssignment, 1), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kAssignment, 2), ElementType::kVariable);
                expected = {ProgramElement::createVariable("y")};
                REQUIRE(result == expected);
            }
            SECTION("Variables given If") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given While") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given Statement") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                // check that it traces to children without being told
                result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
                expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given Procedure") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("f"), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("y"),
                        ProgramElement::createVariable("z"),
                };
                REQUIRE(result == expected);
            }
        }
    }
    SECTION("Uses") {
        PkbRelationshipType relationshipType = PkbRelationshipType::kUses;
        SECTION("isRelationship") {
            SECTION("Assignment Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kAssignment, 2), ProgramElement::createVariable("x")));

                REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kAssignment, 2), ProgramElement::createVariable("y")));
            }
            SECTION("If Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kIf, 6), ProgramElement::createVariable("z")));
            }
            SECTION("While Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createVariable("z")));
            }
            SECTION("Statement Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 2), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("z")));

                REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("y")));
            }
            SECTION("Procedure Variable") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
            }
        }
        SECTION("getLeftSide") {
            SECTION("Assignment given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::kAssignment);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kAssignment, 2),
                };

                REQUIRE(result == expected);
            }
            SECTION("Print given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::kPrint);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kPrint, 11),
                };

                REQUIRE(result == expected);
            }
            SECTION("If given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kIf);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kIf, 6),
                };

                REQUIRE(result == expected);
            }
            SECTION("While given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kWhile);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kWhile, 4),
                };

                REQUIRE(result == expected);
            }
            SECTION("Statements given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kStatement);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kStatement, 4),
                        ProgramElement::createStatement(ElementType::kStatement, 6),
                };

                REQUIRE(result == expected);

                result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::kStatement);

                expected = {
                        ProgramElement::createStatement(ElementType::kStatement, 2),
                        ProgramElement::createStatement(ElementType::kStatement, 4),
                        ProgramElement::createStatement(ElementType::kStatement, 11),
                };

                REQUIRE(result == expected);
            }
        }
        SECTION("getRightSide") {
            SECTION("Variables given Assignment") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kAssignment, 2), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);
            }
            SECTION("Variables given If") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("z")};

                REQUIRE(result == expected);
            }
            SECTION("Variables given While") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("z"), ProgramElement::createVariable("x")};

                REQUIRE(result == expected);
            }
            SECTION("Variables given Statement") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 2), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
                expected = {ProgramElement::createVariable("z"), ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kVariable);
                expected = {ProgramElement::createVariable("z")};

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kVariable);
                expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);
            }
            SECTION("Variables given Procedure") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createProcedure("f"), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("z"), ProgramElement::createVariable("x")};

                REQUIRE(result == expected);
            }
        }
    }
    SECTION("Follows") {
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kAssignment, 7), ProgramElement::createStatement(ElementType::kAssignment, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kPrint, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 5)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kStatement);
            std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kStatement, 4)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kWhile);
            expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            REQUIRE(result.empty());
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kStatement);
            std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kStatement, 11)};
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kPrint);
            expected = {ProgramElement::createStatement(ElementType::kPrint, 11)};
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            expected = {ProgramElement::createStatement(ElementType::kStatement, 2)};
            REQUIRE(result == expected);
        }
    }
    SECTION("FollowsT") {
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kAssignment, 7), ProgramElement::createStatement(ElementType::kAssignment, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kPrint, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 5)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kAssignment, 1), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 5), ProgramElement::createStatement(ElementType::kIf, 6)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kAssignment);
            std::set<ProgramElement> expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 1),
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
                    ProgramElement::createStatement(ElementType::kAssignment, 3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 1),
                    ProgramElement::createStatement(ElementType::kStatement, 2),
                    ProgramElement::createStatement(ElementType::kStatement, 3),
                    ProgramElement::createStatement(ElementType::kStatement, 4),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kAssignment);
            std::set<ProgramElement> expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
                    ProgramElement::createStatement(ElementType::kAssignment, 3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 2),
                    ProgramElement::createStatement(ElementType::kStatement, 3),
                    ProgramElement::createStatement(ElementType::kStatement, 4),
                    ProgramElement::createStatement(ElementType::kStatement, 11),
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("Parent") {
        PkbRelationshipType relationshipType = PkbRelationshipType::kParent;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kIf, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kRead, 5)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ProgramElement::createStatement(ElementType::kAssignment, 9)));
            REQUIRE(!pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 7)));
        }
        SECTION("getLeftSide") {
            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 9), ElementType::kStatement);
            expected = {ProgramElement::createStatement(ElementType::kStatement, 6)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::kAssignment, 7), ElementType::kIf);
            expected = {ProgramElement::createStatement(ElementType::kIf, 6)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kWhile);
            expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 6),
                    ProgramElement::createStatement(ElementType::kStatement, 5),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kAssignment);
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 7),
                    ProgramElement::createStatement(ElementType::kAssignment, 8),
                    ProgramElement::createStatement(ElementType::kAssignment, 9),
                    ProgramElement::createStatement(ElementType::kAssignment, 10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kIf);
            expected = {
                    ProgramElement::createStatement(ElementType::kIf, 6),
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("ParentT") {
        PkbRelationshipType relationshipType = PkbRelationshipType::kParentT;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;

        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kAssignment, 7)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kStatement, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kAssignment, 9)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 10)));
        }
        SECTION("getLeftSide") {
            result = pkb_getter->getLeftSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kAssignment, 9), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 4),
                    ProgramElement::createStatement(ElementType::kStatement, 6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 8), ElementType::kWhile);
            expected = {
                    ProgramElement::createStatement(ElementType::kWhile, 4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kAssignment, 7), ElementType::kIf);
            expected = {
                    ProgramElement::createStatement(ElementType::kIf, 6),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 5),
                    ProgramElement::createStatement(ElementType::kStatement, 6),
                    ProgramElement::createStatement(ElementType::kStatement, 7),
                    ProgramElement::createStatement(ElementType::kStatement, 8),
                    ProgramElement::createStatement(ElementType::kStatement, 9),
                    ProgramElement::createStatement(ElementType::kStatement, 10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kAssignment);
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 7),
                    ProgramElement::createStatement(ElementType::kAssignment, 8),
                    ProgramElement::createStatement(ElementType::kAssignment, 9),
                    ProgramElement::createStatement(ElementType::kAssignment, 10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kIf);
            expected = {
                    ProgramElement::createStatement(ElementType::kIf, 6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kAssignment);
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 7),
                    ProgramElement::createStatement(ElementType::kAssignment, 8),
                    ProgramElement::createStatement(ElementType::kAssignment, 9),
                    ProgramElement::createStatement(ElementType::kAssignment, 10),
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("Pattern") {
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("LHS wildcard") {
            result = pkb_getter->getAssignmentGivenExpression(ProgramElement::createVariable("x"));
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenExpression(ProgramElement::createConstant("5"));
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 1),
                    ProgramElement::createStatement(ElementType::kAssignment, 3),
                    ProgramElement::createStatement(ElementType::kAssignment, 8),
            };
            REQUIRE(result == expected);
        }
        SECTION("LHS variable") {
            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("y"), ProgramElement::createVariable("x"));
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("x"), ProgramElement::createConstant("100"));
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 7),
            };
            REQUIRE(result == expected);
        }
    }
}

TEST_CASE("PKB Calls") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
    //proc1 calls proc2
    //proc2 calls proc3
    //proc2 calls proc4
    //proc3 calls proc4
    //proc4 calls proc5
    //proc4 calls proc6
    procedures = {
            {
                    ParsedStatement(
                            1,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc2",
                            ParsedStatement::default_null_stmt_no
                    )
            },
            {
                    ParsedStatement(
                            2,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc3",
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            3,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc4",
                            2
                    )
            },
            {
                    ParsedStatement(
                            4,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc3",
                            {},
                            {},
                            {},
                            "proc4",
                            ParsedStatement::default_null_stmt_no
                    ),
            },
            {
                    ParsedStatement(
                            5,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc4",
                            {},
                            {},
                            {},
                            "proc5",
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            6,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc4",
                            {},
                            {},
                            {},
                            "proc6",
                            ParsedStatement::default_null_stmt_no
                    ),
            },
            {
                    ParsedStatement(
                            7,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc5",
                            {"b"},
                            {"a"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
            },
            {
                    ParsedStatement(
                            8,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc6",
                            {},
                            {"c"},
                            {"100"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
            },
    };
    pkbSetter->insertStmts(procedures);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc2")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc3")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc4")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc3"), ProgramElement::createProcedure("proc4")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ProgramElement::createProcedure("proc5")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ProgramElement::createProcedure("proc6")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc2")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc3")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc5")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc2"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc1")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc2")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc3")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc2"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc1")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc1")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc5"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc1"),
                              ProgramElement::createProcedure("proc2"),
                              ProgramElement::createProcedure("proc3"),
                              ProgramElement::createProcedure("proc4"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc1"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc2")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc4")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {ProgramElement::createProcedure("proc5"), ProgramElement::createProcedure("proc6")};
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc5"),
                ProgramElement::createProcedure("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc4"),
                ProgramElement::createProcedure("proc5"),
                ProgramElement::createProcedure("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc1"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc2"),
                ProgramElement::createProcedure("proc3"),
                ProgramElement::createProcedure("proc4"),
                ProgramElement::createProcedure("proc5"),
                ProgramElement::createProcedure("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB ModifiesP") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
//    SIMPLE source tested
//    procedure proc1 {
//        proc1Var = 5 + x;
//        call proc2;
//        call proc3;
//    }
//
//    procedure proc2 {
//        read proc2Var;
//        call proc4;
//        call proc5;
//    }
//
//    procedure proc3 {
//        read proc3Var;
//        call proc5;
//    }
//
//    procedure proc4 {
//        proc4Var = 123;
//    }
//
//    procedure proc5 {
//        proc5Var = 456;
//    }

    procedures = {
            {
                    ParsedStatement(
                            1,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kread_stmt,
                            "",
                            "proc1",
                            {"x"},
                            {"proc1Var"},
                            {"5"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            2,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc2",
                            1
                    ),
                    ParsedStatement(
                            3,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc3",
                            2
                    ),
            },
            {
                    ParsedStatement(
                            4,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kread_stmt,
                            "",
                            "proc2",
                            {},
                            {"proc2Var"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            5,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc4",
                            4
                    ),
                    ParsedStatement(
                            6,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc5",
                            5
                    ),
            },
            {
                    ParsedStatement(
                            7,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kread_stmt,
                            "",
                            "proc3",
                            {},
                            {"proc3Var"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            8,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc3",
                            {},
                            {},
                            {},
                            "proc5",
                            7
                    ),
            },
            {
                    ParsedStatement(
                            9,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc4",
                            {},
                            {"proc4Var"},
                            {"123"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    )
            },
            {
                    ParsedStatement(
                            10,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc5",
                            {},
                            {"proc5Var"},
                            {"456"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    )
            }
    };
    pkbSetter->insertStmts(procedures);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc4"), ProgramElement::createVariable("proc4Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc5"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc2"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc4Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc1Var"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
                ProgramElement::createProcedure("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc5Var"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
                ProgramElement::createProcedure("proc2"),
                ProgramElement::createProcedure("proc3"),
                ProgramElement::createProcedure("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc5"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc3"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc2"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc4Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc1"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc1Var"),
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
                ProgramElement::createVariable("proc4Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB ModifiesS") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
//    procedure proc1 {
//        while (proc1Var1 == 5) {
//            read proc1Var2;
//            call proc2;
//        }
//    }
//
//    procedure proc2 {
//        proc2Var = 100;
//        call proc3;
//    }
//
//    procedure proc3 {
//        read proc3Var;
//    }

    procedures = {
            {
                    ParsedStatement(
                            1,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kwhile_stmt,
                            "",
                            "proc1",
                            {"proc1Var1"},
                            {},
                            {"5"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            2,
                            ParsedStatement::default_null_stmt_no,
                            1,
                            StatementType::kread_stmt,
                            "",
                            "proc1",
                            {},
                            {"proc1Var2"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            3,
                            ParsedStatement::default_null_stmt_no,
                            1,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc2",
                            2
                    ),
            },
            {
                    ParsedStatement(
                            4,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc2",
                            {},
                            {"proc2Var"},
                            {"100"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            5,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc3",
                            4
                    ),
            },
            {
                    ParsedStatement(
                            6,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kread_stmt,
                            "",
                            "proc3",
                            {},
                            {"proc3Var"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
            }
    };
    pkbSetter->insertStmts(procedures);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kRead, 6), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kCall, 5), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 3), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kWhile, 1), ProgramElement::createVariable("proc3Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc1Var2"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc1Var2"), ElementType::kWhile);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kWhile, 1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 3),
                ProgramElement::createStatement(ElementType::kStatement, 4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kCall);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kCall, 3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kRead);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kRead, 6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kWhile);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kWhile, 1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 3),
                ProgramElement::createStatement(ElementType::kStatement, 5),
                ProgramElement::createStatement(ElementType::kStatement, 6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kRead, 6), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 5), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kCall, 3), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc1Var2"),
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kRead, 2), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc1Var2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB UsesP") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
//    SIMPLE source tested
//    procedure proc1 {
//        proc1Var = 5 + x;
//        call proc2;
//        call proc3;
//    }
//
//    procedure proc2 {
//        print proc2Var;
//        call proc4;
//        call proc5;
//    }
//
//    procedure proc3 {
//        print proc3Var;
//        call proc5;
//    }
//
//    procedure proc4 {
//        abc = proc4Var;
//    }
//
//    procedure proc5 {
//        def = proc5Var;
//    }

    procedures = {
            {
                    ParsedStatement(
                            1,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kread_stmt,
                            "",
                            "proc1",
                            {"x"},
                            {"proc1Var"},
                            {"5"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            2,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc2",
                            1
                    ),
                    ParsedStatement(
                            3,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc3",
                            2
                    ),
            },
            {
                    ParsedStatement(
                            4,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kprint_stmt,
                            "",
                            "proc2",
                            {"proc2Var"},
                            {},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            5,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc4",
                            4
                    ),
                    ParsedStatement(
                            6,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc5",
                            5
                    ),
            },
            {
                    ParsedStatement(
                            7,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kprint_stmt,
                            "",
                            "proc3",
                            {"proc3Var"},
                            {},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            8,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc3",
                            {},
                            {},
                            {},
                            "proc5",
                            7
                    ),
            },
            {
                    ParsedStatement(
                            9,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc4",
                            {"proc4Var"},
                            {"abc"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    )
            },
            {
                    ParsedStatement(
                            10,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc5",
                            {"proc5Var"},
                            {"def"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    )
            }
    };
    pkbSetter->insertStmts(procedures);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc4"), ProgramElement::createVariable("proc4Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc5"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc2"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc4Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("x"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
                ProgramElement::createProcedure("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc5Var"), ElementType::kProcedure);
        expectedElementSet = {
                ProgramElement::createProcedure("proc1"),
                ProgramElement::createProcedure("proc2"),
                ProgramElement::createProcedure("proc3"),
                ProgramElement::createProcedure("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc5"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc3"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc2"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc4Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc1"), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("x"),
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
                ProgramElement::createVariable("proc4Var"),
                ProgramElement::createVariable("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB UsesS") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
//    procedure proc1 {
//        while (proc1Var1 == 5) {
//            print proc1Var2;
//            call proc2;
//        }
//    }
//
//    procedure proc2 {
//        abc = proc2Var;
//        call proc3;
//    }
//
//    procedure proc3 {
//        print proc3Var;
//    }

    procedures = {
            {
                    ParsedStatement(
                            1,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kwhile_stmt,
                            "",
                            "proc1",
                            {"proc1Var1"},
                            {},
                            {"5"},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            2,
                            ParsedStatement::default_null_stmt_no,
                            1,
                            StatementType::kprint_stmt,
                            "",
                            "proc1",
                            {"proc1Var2"},
                            {},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            3,
                            ParsedStatement::default_null_stmt_no,
                            1,
                            StatementType::kcall_stmt,
                            "",
                            "proc1",
                            {},
                            {},
                            {},
                            "proc2",
                            2
                    ),
            },
            {
                    ParsedStatement(
                            4,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kassign_stmt,
                            "",
                            "proc2",
                            {"proc2Var"},
                            {"abc"},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
                    ParsedStatement(
                            5,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kcall_stmt,
                            "",
                            "proc2",
                            {},
                            {},
                            {},
                            "proc3",
                            4
                    ),
            },
            {
                    ParsedStatement(
                            6,
                            ParsedStatement::default_null_stmt_no,
                            ParsedStatement::default_null_stmt_no,
                            StatementType::kprint_stmt,
                            "",
                            "proc3",
                            {"proc3Var"},
                            {},
                            {},
                            ParsedStatement::default_procedure_name,
                            ParsedStatement::default_null_stmt_no
                    ),
            }
    };
    pkbSetter->insertStmts(procedures);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kPrint, 6), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kCall, 5), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 3), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kWhile, 1), ProgramElement::createVariable("proc3Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc1Var2"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc1Var2"), ElementType::kWhile);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kWhile, 1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 3),
                ProgramElement::createStatement(ElementType::kStatement, 4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kCall);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kCall, 3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kPrint);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kPrint, 6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kWhile);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kWhile, 1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kStatement);
        expectedElementSet = {
                ProgramElement::createStatement(ElementType::kStatement, 1),
                ProgramElement::createStatement(ElementType::kStatement, 3),
                ProgramElement::createStatement(ElementType::kStatement, 5),
                ProgramElement::createStatement(ElementType::kStatement, 6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kPrint, 6), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 5), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kCall, 3), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc1Var1"),
                ProgramElement::createVariable("proc1Var2"),
                ProgramElement::createVariable("proc2Var"),
                ProgramElement::createVariable("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kPrint, 2), ElementType::kVariable);
        expectedElementSet = {
                ProgramElement::createVariable("proc1Var2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB Validation") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::vector<std::vector<ParsedStatement>> procedures;
    std::set<ProgramElement> resultElementSet, expectedElementSet;
    SECTION("Recursive call") {
        procedures = {
                {
                        ParsedStatement(
                                1,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc1",
                                {},
                                {},
                                {},
                                "proc1",
                                ParsedStatement::default_null_stmt_no
                        )
                }
        };
        REQUIRE_THROWS(pkbSetter->insertStmts(procedures));
    }
    SECTION("Cyclic call") {
        procedures = {
                {
                        ParsedStatement(
                                1,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc1",
                                {},
                                {},
                                {},
                                "proc2",
                                ParsedStatement::default_null_stmt_no
                        )
                },
                {
                        ParsedStatement(
                                2,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc2",
                                {},
                                {},
                                {},
                                "proc3",
                                ParsedStatement::default_null_stmt_no
                        ),
                        ParsedStatement(
                                3,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc4",
                                {},
                                {},
                                {},
                                "proc2",
                                2
                        )
                },
                {
                        ParsedStatement(
                                4,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc3",
                                {},
                                {},
                                {},
                                "proc4",
                                ParsedStatement::default_null_stmt_no
                        ),
                },
                {
                        ParsedStatement(
                                5,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc4",
                                {},
                                {},
                                {},
                                "proc5",
                                ParsedStatement::default_null_stmt_no
                        ),
                        ParsedStatement(
                                6,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc4",
                                {},
                                {},
                                {},
                                "proc6",
                                ParsedStatement::default_null_stmt_no
                        ),
                },
        };
        REQUIRE_THROWS(pkbSetter->insertStmts(procedures));
    }
    SECTION("Duplicate Procedure") {
        procedures = {
                {
                        ParsedStatement(
                                1,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc1",
                                {},
                                {},
                                {},
                                "proc2",
                                ParsedStatement::default_null_stmt_no
                        )
                },
                {
                        ParsedStatement(
                                2,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kprint_stmt,
                                "",
                                "proc2",
                                {"abc"},
                                {},
                                {},
                                ParsedStatement::default_procedure_name,
                                ParsedStatement::default_null_stmt_no
                        )
                },
                {
                        ParsedStatement(
                                3,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kprint_stmt,
                                "",
                                "proc1",
                                {"abc"},
                                {},
                                {},
                                ParsedStatement::default_procedure_name,
                                ParsedStatement::default_null_stmt_no
                        )
                },
        };
        REQUIRE_THROWS(pkbSetter->insertStmts(procedures));
    }
    SECTION("Undefined call") {
        procedures = {
                {
                        ParsedStatement(
                                1,
                                ParsedStatement::default_null_stmt_no,
                                ParsedStatement::default_null_stmt_no,
                                StatementType::kcall_stmt,
                                "",
                                "proc1",
                                {},
                                {},
                                {},
                                "proc2",
                                ParsedStatement::default_null_stmt_no
                        )
                },
        };
        REQUIRE_THROWS(pkbSetter->insertStmts(procedures));
    }
}
