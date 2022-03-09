#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"
#include "PkbRelationshipType.h"
#include "PkbUtil.h"

#include "catch.hpp"


TEST_CASE("PKB Basic") {
    PKB pkb = PKB();
    PkbGetter* pkb_getter = pkb.getGetter();
    PkbSetter* pkb_setter = pkb.getSetter();

    REQUIRE(pkb_getter != NULL);
    REQUIRE(pkb_setter != NULL);

    PKB_BASIC_TEST_CASE tcData;
    pkb_setter->insertStmts(tcData.stmtLists);

    SECTION("General") {
        SECTION("getEntity") {
            SECTION("Statements") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kStatement);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(1),
                        tcData.stmt.at(2),
                        tcData.stmt.at(3),
                        tcData.stmt.at(4),
                        tcData.stmt.at(5),
                        tcData.stmt.at(6),
                        tcData.stmt.at(7),
                        tcData.stmt.at(8),
                        tcData.stmt.at(9),
                        tcData.stmt.at(10),
                        tcData.stmt.at(11),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("y"),
                        tcData.vars.at("z"),
                };

                REQUIRE(result == expected);
            }
            SECTION("Assignments") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kAssignment);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(1),
                        tcData.stmt.at(2),
                        tcData.stmt.at(3),
                        tcData.stmt.at(7),
                        tcData.stmt.at(8),
                        tcData.stmt.at(9),
                        tcData.stmt.at(10),
                };

                REQUIRE(result == expected);
            }
            SECTION("Ifs") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kIf);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);
            }
            SECTION("Whiles") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kWhile);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                };

                REQUIRE(result == expected);
            }
            SECTION("Read") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kRead);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(5)
                };

                REQUIRE(result == expected);
            }
            SECTION("Print") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::kPrint);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(11)
                };

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
                        tcData.stmt.at(1),
                        tcData.stmt.at(3),
                        tcData.stmt.at(7),
                };

                REQUIRE(result == expected);
            }
            SECTION("If given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("x"), ElementType::kIf);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };
                REQUIRE(result == expected);
            }
            SECTION("While given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("x"), ElementType::kWhile);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4)
                };
                REQUIRE(result == expected);
            }
            SECTION("Statement given Variable") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("z"), ElementType::kStatement);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                        tcData.stmt.at(5),
                        tcData.stmt.at(6),
                        tcData.stmt.at(8),
                        tcData.stmt.at(10),
                };
                REQUIRE(result == expected);
            }
        }
        SECTION("getRightSide") {
            SECTION("Variables given Assignment") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kAssignment, 1), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kAssignment, 2), ElementType::kVariable);
                expected = {
                        tcData.vars.at("y")
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given If") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given While") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
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
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given Procedure") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("f"), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("y"),
                        tcData.vars.at("z"),
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
                        tcData.stmt.at(2),
                };

                REQUIRE(result == expected);
            }
            SECTION("Print given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::kPrint);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(11),
                };

                REQUIRE(result == expected);
            }
            SECTION("If given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kIf);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);
            }
            SECTION("While given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kWhile);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                };

                REQUIRE(result == expected);
            }
            SECTION("Statements given Variable") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::kStatement);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);

                result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::kStatement);

                expected = {
                        tcData.stmt.at(2),
                        tcData.stmt.at(4),
                        tcData.stmt.at(11),
                };

                REQUIRE(result == expected);
            }
        }
        SECTION("getRightSide") {
            SECTION("Variables given Assignment") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kAssignment, 2), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given If") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("z")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given While") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("z"),
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given Statement") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 2), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
                expected = {
                        tcData.vars.at("z"),
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kVariable);
                expected = {
                        tcData.vars.at("z")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kVariable);
                expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given Procedure") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createProcedure("f"), ElementType::kVariable);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("z"),
                        tcData.vars.at("x")
                };

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
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kWhile);
            expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            REQUIRE(result.empty());
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kStatement);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(11)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kPrint);
            expected = {
                    tcData.stmt.at(11)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollows, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            expected = {
                    tcData.stmt.at(2)
            };
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
                    tcData.stmt.at(1),
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 11), ElementType::kStatement);
            expected = {
                    tcData.stmt.at(1),
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kAssignment);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kFollowsT, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kStatement);
            expected = {
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
                    tcData.stmt.at(4),
                    tcData.stmt.at(11),
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
            expected = {
                    tcData.stmt.at(6)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::kAssignment, 7), ElementType::kIf);
            expected = {
                    tcData.stmt.at(6)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::kIf, 6), ElementType::kWhile);
            expected = {
                    tcData.stmt.at(4)
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kStatement);
            expected = {
                    tcData.stmt.at(6),
                    tcData.stmt.at(5),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kAssignment);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kIf);
            expected = {
                    tcData.stmt.at(6),
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
                    tcData.stmt.at(4),
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 8), ElementType::kWhile);
            expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kAssignment, 7), ElementType::kIf);
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kWhile, 4), ElementType::kStatement);
            expected = {
                    tcData.stmt.at(5),
                    tcData.stmt.at(6),
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kAssignment);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kIf);
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::kParentT, ProgramElement::createStatement(ElementType::kStatement, 6), ElementType::kAssignment);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
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
                    tcData.stmt.at(2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenExpression(ProgramElement::createConstant("5"));
            expected = {
                    tcData.stmt.at(1),
                    tcData.stmt.at(3),
                    tcData.stmt.at(8),
            };
            REQUIRE(result == expected);
        }
        SECTION("LHS variable") {
            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("y"), ProgramElement::createVariable("x"));
            expected = {
                    tcData.stmt.at(2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("x"), ProgramElement::createConstant("100"));
            expected = {
                    tcData.stmt.at(7),
            };
            REQUIRE(result == expected);
        }
    }
}

TEST_CASE("PKB Calls") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;
    PKB_CALLS_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
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
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc2"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc2"),
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc5"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc4"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc1"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc2")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc4")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCalls, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc4"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc3"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc4"),
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kCallsT, ProgramElement::createProcedure("proc1"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc4"),
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB ModifiesP") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_MODIFIES_P_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
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
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc5Var"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc5"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc3"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc2"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createProcedure("proc1"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc1Var"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB ModifiesS") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_MODIFIES_S_TEST_CASE tcData;

    pkbSetter->insertStmts(tcData.stmtLists);
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
                tcData.stmt.at(1),
                tcData.stmt.at(2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc1Var2"), ElementType::kWhile);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc2Var"), ElementType::kCall);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kRead);
        expectedElementSet = {
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kWhile);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable("proc3Var"), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kRead, 6), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 5), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kCall, 3), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc1Var2"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kModifies, ProgramElement::createStatement(ElementType::kRead, 2), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc1Var2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB UsesP") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_USES_P_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
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
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc5Var"), ElementType::kProcedure);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc5"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc3"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc2"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createProcedure("proc1"), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("x"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB UsesS") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_USES_S_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
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
                tcData.stmt.at(1),
                tcData.stmt.at(2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc1Var2"), ElementType::kWhile);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc2Var"), ElementType::kCall);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kPrint);
        expectedElementSet = {
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kWhile);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kUses, ProgramElement::createVariable("proc3Var"), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kPrint, 6), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 5), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 4), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kCall, 3), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kStatement, 1), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc1Var1"),
                tcData.vars.at("proc1Var2"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kUses, ProgramElement::createStatement(ElementType::kPrint, 2), ElementType::kVariable);
        expectedElementSet = {
                tcData.vars.at("proc1Var2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB Next") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_NEXT_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 2), ProgramElement::createStatement(ElementType::kStatement, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kWhile, 3), ProgramElement::createStatement(ElementType::kAssignment, 4)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kWhile, 3), ProgramElement::createStatement(ElementType::kStatement, 7)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kCall, 5), ProgramElement::createStatement(ElementType::kStatement, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 7), ProgramElement::createStatement(ElementType::kAssignment, 9)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 8), ProgramElement::createStatement(ElementType::kAssignment, 10)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 7), ProgramElement::createStatement(ElementType::kAssignment, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kAssignment, 16), ProgramElement::createStatement(ElementType::kWhile, 13)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kAssignment, 16), ProgramElement::createStatement(ElementType::kIf, 17)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 19), ProgramElement::createStatement(ElementType::kStatement, 22)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 21), ProgramElement::createStatement(ElementType::kAssignment, 22)));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 10), ElementType::kAssignment);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(9),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kIf, 7), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kAssignment, 22), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(19),
                tcData.stmt.at(20),
                tcData.stmt.at(21),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kWhile, 13), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(15),
                tcData.stmt.at(16),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kAssignment, 1), ElementType::kStatement);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 22), ElementType::kStatement);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kIf, 17), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(18),
                tcData.stmt.at(21),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kIf, 17), ElementType::kIf);
        expectedElementSet = {
                tcData.stmt.at(18),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 13), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(14),
                tcData.stmt.at(17),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 20), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(22),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kWhile, 3), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(7),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 9), ElementType::kStatement);
        expectedElementSet = {
                tcData.stmt.at(10),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::kNext, ProgramElement::createStatement(ElementType::kStatement, 7), ElementType::kAssignment);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(9),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB Validation") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;
    PKB_VALIDATION_TEST_CASES tcData;
    SECTION("Recursive call") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.RECURSIVE_CALL_STMT_LIST));
    }
    SECTION("Cyclic call") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.CYCLIC_CALL_STMT_LIST));
    }
    SECTION("Duplicate Procedure") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.DUPLICATE_PROCEDURE_STMT_LIST));
    }
    SECTION("Undefined call") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.UNDEFINITED_CALL_STMT_LIST));
    }
}
