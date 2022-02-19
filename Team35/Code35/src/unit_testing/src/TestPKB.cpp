#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"

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

    std::vector<ParsedStatement> statements = {
            ParsedStatement(1, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"5"},  ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(2, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {"x"}, {"y"}, {}, ParsedStatement::default_procedure_name, 1),
            ParsedStatement(3, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"5"}, ParsedStatement::default_procedure_name, 2),
            ParsedStatement(4, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kwhile_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, {"4"}, ParsedStatement::default_procedure_name, 3),
            ParsedStatement(5, ParsedStatement::default_null_stmt_no, 4, StatementType::kread_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {},ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(6, ParsedStatement::default_null_stmt_no, 4, StatementType::kif_stmt, ParsedStatement::default_pattern, "f", {"z"}, {}, {"10"}, ParsedStatement::default_procedure_name, 5),
            ParsedStatement(7, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"100"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(8, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {"5"}, ParsedStatement::default_procedure_name, 7),
            ParsedStatement(9, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"y"}, {"100"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(10, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {"0"}, ParsedStatement::default_procedure_name, 9),
            ParsedStatement(11, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kprint_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, {}, ParsedStatement::default_procedure_name, 4),
    };

    for (const ParsedStatement& parsed_statement : statements)
        pkb_setter->insertStmt(parsed_statement);


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
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies,
                                                   ProgramElement::createStatement(ElementType::kAssignment, 1),
                                                   ProgramElement::createVariable("x"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies,
                                                   ProgramElement::createStatement(ElementType::kAssignment, 2),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies,
                                                   ProgramElement::createStatement(ElementType::kWhile, 4),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies,
                                                   ProgramElement::createStatement(ElementType::kIf, 6),
                                                   ProgramElement::createVariable("y"))
                                == true);
            }
            SECTION("Procedure and Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies,
                                                   ProgramElement::createProcedure("f"),
                                                   ProgramElement::createVariable("z")));
            }
        }
        SECTION("getLeftSide") {
            SECTION("Assignment given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::kAssignment);

                std::set<ProgramElement> expected = {
                        ProgramElement::createStatement(ElementType::kAssignment, 1),
                        ProgramElement::createStatement(ElementType::kAssignment, 3),
                        ProgramElement::createStatement(ElementType::kAssignment, 7),
                };

                REQUIRE(result == expected);
            }
            SECTION("If given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::kIf);

                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kIf, 6)};
                REQUIRE(result == expected);
            }
            SECTION("While given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::kWhile);

                std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};
                REQUIRE(result == expected);
            }
            SECTION("Statement given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("z"), ElementType::kStatement);
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
                std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kAssignment, 1), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kAssignment, 2), ElementType::kVariable);
                expected = {ProgramElement::createVariable("y")};
                REQUIRE(result == expected);
            }
            SECTION("Variables given If") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given While") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given Statement") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                // check that it traces to children without being told
                result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
                expected = {
                        ProgramElement::createVariable("x"),
                        ProgramElement::createVariable("z"),
                        ProgramElement::createVariable("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given Procedure") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createProcedure("f"), ElementType::kVariable);
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
        RelationshipType relationshipType = RelationshipType::Uses;
        SECTION("isRelationship") {
            SECTION("Assignment Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kAssignment, 2), ProgramElement::createVariable("x")));

                REQUIRE(!pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kAssignment, 2), ProgramElement::createVariable("y")));
            }
            SECTION("If Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kIf, 6), ProgramElement::createVariable("z")));
            }
            SECTION("While Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createVariable("z")));
            }
            SECTION("Statement Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kStatement, 2), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("z")));

                REQUIRE(!pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createVariable("y")));
            }
            SECTION("Procedure Variable") {
                REQUIRE(pkb_getter->isRelationship(RelationshipType::Uses, ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
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
            REQUIRE(pkb_getter->isRelationship(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kAssignment, 7), ProgramElement::createStatement(ElementType::kAssignment, 8)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kPrint, 11)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(!pkb_getter->isRelationship(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 5)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kStatement);
            std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kStatement, 4)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kWhile);
            expected = {ProgramElement::createStatement(ElementType::kWhile, 4)};
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            REQUIRE(result.empty());
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kStatement);
            std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::kStatement, 11)};
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kPrint);
            expected = {ProgramElement::createStatement(ElementType::kPrint, 11)};
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::Follows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            expected = {ProgramElement::createStatement(ElementType::kStatement, 2)};
            REQUIRE(result == expected);
        }
    }
    SECTION("FollowsT") {
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kAssignment, 7), ProgramElement::createStatement(ElementType::kAssignment, 8)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kPrint, 11)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(!pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 5)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kAssignment, 1), ProgramElement::createStatement(ElementType::kStatement, 11)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 5), ProgramElement::createStatement(ElementType::kIf, 6)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kAssignment);
            std::set<ProgramElement> expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 1),
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
                    ProgramElement::createStatement(ElementType::kAssignment, 3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 1),
                    ProgramElement::createStatement(ElementType::kStatement, 2),
                    ProgramElement::createStatement(ElementType::kStatement, 3),
                    ProgramElement::createStatement(ElementType::kStatement, 4),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kAssignment);
            std::set<ProgramElement> expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 2),
                    ProgramElement::createStatement(ElementType::kAssignment, 3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::FollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
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
        RelationshipType relationshipType = RelationshipType::Parent;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(kIf, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(kStatement, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(kRead, 5)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ProgramElement::createStatement(kAssignment, 9)));
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
        RelationshipType relationshipType = RelationshipType::ParentT;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;

        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kAssignment, 7)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ProgramElement::createStatement(ElementType::kStatement, 8)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kAssignment, 9)));
            REQUIRE(pkb_getter->isRelationship(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ProgramElement::createStatement(ElementType::kStatement, 10)));
        }
        SECTION("getLeftSide") {
            result = pkb_getter->getLeftSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kAssignment, 9), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 4),
                    ProgramElement::createStatement(ElementType::kStatement, 6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 8), ElementType::kWhile);
            expected = {
                    ProgramElement::createStatement(ElementType::kWhile, 4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kAssignment, 7), ElementType::kIf);
            expected = {
                    ProgramElement::createStatement(ElementType::kIf, 6),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kStatement);
            expected = {
                    ProgramElement::createStatement(ElementType::kStatement, 5),
                    ProgramElement::createStatement(ElementType::kStatement, 6),
                    ProgramElement::createStatement(ElementType::kStatement, 7),
                    ProgramElement::createStatement(ElementType::kStatement, 8),
                    ProgramElement::createStatement(ElementType::kStatement, 9),
                    ProgramElement::createStatement(ElementType::kStatement, 10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kAssignment);
            expected = {
                    ProgramElement::createStatement(ElementType::kAssignment, 7),
                    ProgramElement::createStatement(ElementType::kAssignment, 8),
                    ProgramElement::createStatement(ElementType::kAssignment, 9),
                    ProgramElement::createStatement(ElementType::kAssignment, 10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kIf);
            expected = {
                    ProgramElement::createStatement(ElementType::kIf, 6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(RelationshipType::ParentT, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kAssignment);
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
