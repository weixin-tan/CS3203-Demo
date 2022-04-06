#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"
#include "PkbRelationshipType.h"
#include "PkbUtil.h"
#include "DesignExtractor/DesignExtractor.h"

#include "catch.hpp"


TEST_CASE("PKB Basic") {
    PKB pkb = PKB();
    PkbGetter* pkb_getter = pkb.getGetter();
    PkbSetter* pkb_setter = pkb.getSetter();

    ExpressionProcessor ep;

    REQUIRE(pkb_getter != NULL);
    REQUIRE(pkb_setter != NULL);

    PKB_BASIC_TEST_CASE tcData;
    pkb_setter->insertStmts(tcData.stmtLists);

    SECTION("General") {
        SECTION("getEntity") {
            SECTION("Statements") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::STATEMENT);
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
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("y"),
                        tcData.vars.at("z"),
                };

                REQUIRE(result == expected);
            }
            SECTION("Assignments") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::ASSIGNMENT);
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
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::IF);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);
            }
            SECTION("Whiles") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::WHILE);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                };

                REQUIRE(result == expected);
            }
            SECTION("READ") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::READ);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(5)
                };

                REQUIRE(result == expected);
            }
            SECTION("PRINT") {
                std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::PRINT);
                std::set<ProgramElement> expected = {
                        tcData.stmt.at(11)
                };

                REQUIRE(result == expected);
            }
        }
    }
    SECTION("Modifies") {
        SECTION("isRelationship") {
            SECTION("STATEMENT and VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::MODIFIES,
                                                   ProgramElement::createStatement(ElementType::ASSIGNMENT, 1),
                                                   ProgramElement::createVariable("x"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::MODIFIES,
                                                   ProgramElement::createStatement(ElementType::ASSIGNMENT, 2),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::MODIFIES,
                                                   ProgramElement::createStatement(ElementType::WHILE, 4),
                                                   ProgramElement::createVariable("y"))
                                == true);
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::MODIFIES,
                                                   ProgramElement::createStatement(ElementType::IF, 6),
                                                   ProgramElement::createVariable("y"))
                                == true);
            }
            SECTION("PROCEDURE and VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::MODIFIES,
                                                   ProgramElement::createProcedure("f"),
                                                   ProgramElement::createVariable("z")));
            }
        }
        SECTION("getLeftSide") {
            SECTION("ASSIGNMENT given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("x"), ElementType::ASSIGNMENT);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(1),
                        tcData.stmt.at(3),
                        tcData.stmt.at(7),
                };

                REQUIRE(result == expected);
            }
            SECTION("IF given VARIABLE") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("x"), ElementType::IF);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };
                REQUIRE(result == expected);
            }
            SECTION("WHILE given VARIABLE") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("x"), ElementType::WHILE);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4)
                };
                REQUIRE(result == expected);
            }
            SECTION("STATEMENT given VARIABLE") {
                std::set<ProgramElement> result =
                        pkb_getter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("z"), ElementType::STATEMENT);
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
            SECTION("Variables given ASSIGNMENT") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ElementType::VARIABLE);
                expected = {
                        tcData.vars.at("y")
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given IF") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::IF, 6), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given WHILE") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::WHILE, 4), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given STATEMENT") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

                REQUIRE(result == expected);

                // check that it traces to children without being told
                result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::VARIABLE);
                expected = {
                        tcData.vars.at("x"),
                        tcData.vars.at("z"),
                        tcData.vars.at("y"),
                };
                REQUIRE(result == expected);
            }
            SECTION("Variables given PROCEDURE") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("f"), ElementType::VARIABLE);
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
        PkbRelationshipType relationshipType = PkbRelationshipType::USES;
        SECTION("isRelationship") {
            SECTION("ASSIGNMENT VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ProgramElement::createVariable("x")));

                REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ProgramElement::createVariable("y")));
            }
            SECTION("IF VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::IF, 6), ProgramElement::createVariable("z")));
            }
            SECTION("WHILE VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createVariable("z")));
            }
            SECTION("STATEMENT VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createVariable("x")));

                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createVariable("z")));

                REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createVariable("y")));
            }
            SECTION("PROCEDURE VARIABLE") {
                REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
            }
        }
        SECTION("getLeftSide") {
            SECTION("ASSIGNMENT given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::ASSIGNMENT);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(2),
                };

                REQUIRE(result == expected);
            }
            SECTION("PRINT given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::PRINT);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(11),
                };

                REQUIRE(result == expected);
            }
            SECTION("IF given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::IF);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);
            }
            SECTION("WHILE given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::WHILE);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                };

                REQUIRE(result == expected);
            }
            SECTION("Statements given VARIABLE") {
                std::set<ProgramElement> result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("z"), ElementType::STATEMENT);

                std::set<ProgramElement> expected = {
                        tcData.stmt.at(4),
                        tcData.stmt.at(6),
                };

                REQUIRE(result == expected);

                result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createVariable("x"), ElementType::STATEMENT);

                expected = {
                        tcData.stmt.at(2),
                        tcData.stmt.at(4),
                        tcData.stmt.at(11),
                };

                REQUIRE(result == expected);
            }
        }
        SECTION("getRightSide") {
            SECTION("Variables given ASSIGNMENT") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x"),
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given IF") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::IF, 6), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("z")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given WHILE") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::WHILE, 4), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("z"),
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given STATEMENT") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 2), ElementType::VARIABLE);
                std::set<ProgramElement> expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::VARIABLE);
                expected = {
                        tcData.vars.at("z"),
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 6), ElementType::VARIABLE);
                expected = {
                        tcData.vars.at("z")
                };

                REQUIRE(result == expected);

                result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::VARIABLE);
                expected = {
                        tcData.vars.at("x")
                };

                REQUIRE(result == expected);
            }
            SECTION("Variables given PROCEDURE") {
                std::set<ProgramElement> result = pkb_getter->getRightSide(relationshipType, ProgramElement::createProcedure("f"), ElementType::VARIABLE);
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
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::PRINT, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
            REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 5)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::STATEMENT);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::WHILE);
            expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::STATEMENT);
            REQUIRE(result.empty());
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::STATEMENT);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(11)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::PRINT);
            expected = {
                    tcData.stmt.at(11)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::FOLLOWS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(2)
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("FOLLOWS_T") {
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::PRINT, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
            REQUIRE(!pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 5)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 5), ProgramElement::createStatement(ElementType::IF, 6)));
        }
        SECTION("getLeftSide") {
            std::set<ProgramElement> result = pkb_getter->getLeftSide(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::ASSIGNMENT);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(1),
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(1),
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            std::set<ProgramElement> result = pkb_getter->getRightSide(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::ASSIGNMENT);
            std::set<ProgramElement> expected = {
                    tcData.stmt.at(2),
                    tcData.stmt.at(3),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::FOLLOWS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::STATEMENT);
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
        PkbRelationshipType relationshipType = PkbRelationshipType::PARENT;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::IF, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::READ, 5)));
            REQUIRE(pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::IF, 6), ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)));
            REQUIRE(!pkb_getter->isRelationship(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 7)));
        }
        SECTION("getLeftSide") {
            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 9), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(6)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7), ElementType::IF);
            expected = {
                    tcData.stmt.at(6)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(relationshipType, ProgramElement::createStatement(ElementType::IF, 6), ElementType::WHILE);
            expected = {
                    tcData.stmt.at(4)
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(6),
                    tcData.stmt.at(5),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::STATEMENT, 6), ElementType::ASSIGNMENT);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(relationshipType, ProgramElement::createStatement(ElementType::WHILE, 4), ElementType::IF);
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("PARENT_T") {
        PkbRelationshipType relationshipType = PkbRelationshipType::PARENT_T;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;

        SECTION("isRelationship") {
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::ASSIGNMENT, 7)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::STATEMENT, 8)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)));
            REQUIRE(pkb_getter->isRelationship(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        }
        SECTION("getLeftSide") {
            result = pkb_getter->getLeftSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(4),
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 8), ElementType::WHILE);
            expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getLeftSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7), ElementType::IF);
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);
        }
        SECTION("getRightSide") {
            result = pkb_getter->getRightSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::WHILE, 4), ElementType::STATEMENT);
            expected = {
                    tcData.stmt.at(5),
                    tcData.stmt.at(6),
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::ASSIGNMENT);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::IF);
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getRightSide(PkbRelationshipType::PARENT_T, ProgramElement::createStatement(ElementType::STATEMENT, 6), ElementType::ASSIGNMENT);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9),
                    tcData.stmt.at(10),
            };
            REQUIRE(result == expected);
        }
    }
    SECTION("PATTERN") {
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("LHS wildcard") {
            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("x"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("5"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(1),
                    tcData.stmt.at(3),
                    tcData.stmt.at(8),
            };
            REQUIRE(result == expected);
        }
        SECTION("LHS variable") {
            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("y"), ep.stringToExpr("x"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(2),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("x"), ep.stringToExpr("100"), ExpressionIndicator::FULL_MATCH);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc2")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc3")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc4")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc3"), ProgramElement::createProcedure("proc4")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc4"), ProgramElement::createProcedure("proc5")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc4"), ProgramElement::createProcedure("proc6")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc2")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc1"), ProgramElement::createProcedure("proc3")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc2"), ProgramElement::createProcedure("proc5")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc2"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc3"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc4"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc2"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc3"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc2"),
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc5"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc4"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc1"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc2")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc3"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc4")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS, ProgramElement::createProcedure("proc4"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6")
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc4"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc3"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc4"),
                tcData.procs.at("proc5"),
                tcData.procs.at("proc6"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::CALLS_T, ProgramElement::createProcedure("proc1"), ElementType::PROCEDURE);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc4"), ProgramElement::createVariable("proc4Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc5"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc2"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc4Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc1Var"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc2Var"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc5Var"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc5"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc3"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc2"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createProcedure("proc1"), ElementType::VARIABLE);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::READ, 6), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::CALL, 5), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 3), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::WHILE, 1), ProgramElement::createVariable("proc3Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc1Var2"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc1Var2"), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc2Var"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc2Var"), ElementType::CALL);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc3Var"), ElementType::READ);
        expectedElementSet = {
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc3Var"), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::MODIFIES, ProgramElement::createVariable("proc3Var"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::READ, 6), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 5), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::CALL, 3), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc1Var2"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::MODIFIES, ProgramElement::createStatement(ElementType::READ, 2), ElementType::VARIABLE);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("proc4"), ProgramElement::createVariable("proc4Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("proc5"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("proc2"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc5Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createProcedure("proc1"), ProgramElement::createVariable("proc4Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("x"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc2Var"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc5Var"), ElementType::PROCEDURE);
        expectedElementSet = {
                tcData.procs.at("proc1"),
                tcData.procs.at("proc2"),
                tcData.procs.at("proc3"),
                tcData.procs.at("proc5"),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createProcedure("proc5"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createProcedure("proc3"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createProcedure("proc2"), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc4Var"),
                tcData.vars.at("proc5Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createProcedure("proc1"), ElementType::VARIABLE);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::PRINT, 6), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::CALL, 5), ProgramElement::createVariable("proc3Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 3), ProgramElement::createVariable("proc2Var")));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::WHILE, 1), ProgramElement::createVariable("proc3Var")));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc1Var2"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc1Var2"), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc2Var"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc2Var"), ElementType::CALL);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc3Var"), ElementType::PRINT);
        expectedElementSet = {
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc3Var"), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::USES, ProgramElement::createVariable("proc3Var"), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::PRINT, 6), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 5), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::CALL, 3), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::VARIABLE);
        expectedElementSet = {
                tcData.vars.at("proc1Var1"),
                tcData.vars.at("proc1Var2"),
                tcData.vars.at("proc2Var"),
                tcData.vars.at("proc3Var"),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::USES, ProgramElement::createStatement(ElementType::PRINT, 2), ElementType::VARIABLE);
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
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::WHILE, 3), ProgramElement::createStatement(ElementType::ASSIGNMENT, 4)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::WHILE, 3), ProgramElement::createStatement(ElementType::STATEMENT, 7)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::CALL, 5), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 7), ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 8), ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 7), ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::ASSIGNMENT, 16), ProgramElement::createStatement(ElementType::WHILE, 13)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::ASSIGNMENT, 16), ProgramElement::createStatement(ElementType::IF, 17)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 19), ProgramElement::createStatement(ElementType::STATEMENT, 22)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 21), ProgramElement::createStatement(ElementType::ASSIGNMENT, 22)));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(2),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 10), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(9),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::IF, 7), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::ASSIGNMENT, 22), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(19),
                tcData.stmt.at(20),
                tcData.stmt.at(21),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::WHILE, 13), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(15),
                tcData.stmt.at(16),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ElementType::STATEMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 22), ElementType::STATEMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::IF, 17), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(18),
                tcData.stmt.at(21),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::IF, 17), ElementType::IF);
        expectedElementSet = {
                tcData.stmt.at(18),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 13), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(14),
                tcData.stmt.at(17),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 20), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(22),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::WHILE, 3), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(7),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 9), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(10),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT, ProgramElement::createStatement(ElementType::STATEMENT, 7), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(9),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB NEXT_T") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_NEXT_TEST_CASE tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 2)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::WHILE, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ProgramElement::createStatement(ElementType::CALL, 5)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4), ProgramElement::createStatement(ElementType::WHILE, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::CALL, 5), ProgramElement::createStatement(ElementType::STATEMENT, 5)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::CALL, 5), ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 8), ProgramElement::createStatement(ElementType::STATEMENT, 9)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 5), ProgramElement::createStatement(ElementType::STATEMENT, 2)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 16), ProgramElement::createStatement(ElementType::STATEMENT, 15)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 15), ProgramElement::createStatement(ElementType::STATEMENT, 19)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 19), ProgramElement::createStatement(ElementType::STATEMENT, 22)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 19), ProgramElement::createStatement(ElementType::STATEMENT, 13)));
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::STATEMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
                tcData.stmt.at(4),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::IF, 7), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
                tcData.stmt.at(3),
                tcData.stmt.at(4),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 12), ElementType::CALL);
        expectedElementSet = {
                tcData.stmt.at(5),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 12), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 12), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(2),
                tcData.stmt.at(4),
                tcData.stmt.at(6),
                tcData.stmt.at(8),
                tcData.stmt.at(9),
                tcData.stmt.at(10),
                tcData.stmt.at(11),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 12), ElementType::STATEMENT);
        expectedElementSet = {
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
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 22), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(13),
                tcData.stmt.at(14),
                tcData.stmt.at(15),
                tcData.stmt.at(16),
                tcData.stmt.at(17),
                tcData.stmt.at(18),
                tcData.stmt.at(19),
                tcData.stmt.at(20),
                tcData.stmt.at(21),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 10), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(11),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::IF, 7), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(9),
                tcData.stmt.at(10),
                tcData.stmt.at(11),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::CALL, 5), ElementType::WHILE);
        expectedElementSet = {
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::CALL, 5), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(3),
                tcData.stmt.at(4),
                tcData.stmt.at(5),
                tcData.stmt.at(6),
                tcData.stmt.at(7),
                tcData.stmt.at(8),
                tcData.stmt.at(9),
                tcData.stmt.at(10),
                tcData.stmt.at(11),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 19), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(22),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::IF, 14), ElementType::IF);
        expectedElementSet = {
                tcData.stmt.at(14),
                tcData.stmt.at(17),
                tcData.stmt.at(18),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 15), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(15),
                tcData.stmt.at(16),
                tcData.stmt.at(19),
                tcData.stmt.at(20),
                tcData.stmt.at(21),
                tcData.stmt.at(22),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::NEXT_T, ProgramElement::createStatement(ElementType::STATEMENT, 16), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(13),
                tcData.stmt.at(14),
                tcData.stmt.at(15),
                tcData.stmt.at(16),
                tcData.stmt.at(17),
                tcData.stmt.at(18),
                tcData.stmt.at(19),
                tcData.stmt.at(20),
                tcData.stmt.at(21),
                tcData.stmt.at(22),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB AFFECTS 1") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_AFFECTS_TEST_CASE_1 tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 6), ProgramElement::createStatement(ElementType::ASSIGNMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 4)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9), ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createStatement(ElementType::ASSIGNMENT, 6)));
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(8),
                tcData.stmt.at(10),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::ASSIGNMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 6), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(6),
                tcData.stmt.at(10),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 12), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(11),
                tcData.stmt.at(10),
                tcData.stmt.at(8),
                tcData.stmt.at(4),
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 10), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(6),
                tcData.stmt.at(2),
                tcData.stmt.at(8),
                tcData.stmt.at(4),
                tcData.stmt.at(1),
                tcData.stmt.at(9),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB AFFECTS 2") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_AFFECTS_TEST_CASE_2 tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
    SECTION("isRelationship") {
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 1)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 3), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 8), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 11), ProgramElement::createStatement(ElementType::ASSIGNMENT, 13)));
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(3),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(3),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(8),
                tcData.stmt.at(11),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(13),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::ASSIGNMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 4), ElementType::ASSIGNMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 6), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(1),
                tcData.stmt.at(3),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 8), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 13), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(11),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 10), ElementType::ASSIGNMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS, ProgramElement::createStatement(ElementType::STATEMENT, 11), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(6),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
}

TEST_CASE("PKB AFFECTS_T 1") {
    PKB pkb;
    PkbSetter* pkbSetter = pkb.getSetter();
    PkbGetter* pkbGetter = pkb.getGetter();
    std::set<ProgramElement> resultElementSet, expectedElementSet;

    PKB_AFFECTS_TEST_CASE_1 tcData;
    pkbSetter->insertStmts(tcData.stmtLists);
    SECTION("isRelationship") {
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 6), ProgramElement::createStatement(ElementType::ASSIGNMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 4)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        REQUIRE(!pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createStatement(ElementType::ASSIGNMENT, 6)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 4)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
        REQUIRE(pkbGetter->isRelationship(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)));
    }
    SECTION("getRightSide") {
        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 1), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(4),
                tcData.stmt.at(8),
                tcData.stmt.at(10),
                tcData.stmt.at(11),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::STATEMENT, 3), ElementType::STATEMENT);
        expectedElementSet = {
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getRightSide(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 6), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(6),
                tcData.stmt.at(10),
                tcData.stmt.at(11),
                tcData.stmt.at(12),
        };
        REQUIRE(resultElementSet == expectedElementSet);
    }
    SECTION("getLeftSide") {
        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 12), ElementType::STATEMENT);
        expectedElementSet = {
                tcData.stmt.at(11),
                tcData.stmt.at(10),
                tcData.stmt.at(9),
                tcData.stmt.at(8),
                tcData.stmt.at(6),
                tcData.stmt.at(4),
                tcData.stmt.at(2),
                tcData.stmt.at(1),
        };
        REQUIRE(resultElementSet == expectedElementSet);

        resultElementSet = pkbGetter->getLeftSide(PkbRelationshipType::AFFECTS_T, ProgramElement::createStatement(ElementType::ASSIGNMENT, 10), ElementType::ASSIGNMENT);
        expectedElementSet = {
                tcData.stmt.at(9),
                tcData.stmt.at(8),
                tcData.stmt.at(6),
                tcData.stmt.at(4),
                tcData.stmt.at(2),
                tcData.stmt.at(1),
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
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.RECURSIVE_CALL_STMT_LIST, true));
    }
    SECTION("Cyclic call") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.CYCLIC_CALL_STMT_LIST, true));
    }
    SECTION("Duplicate PROCEDURE") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.DUPLICATE_PROCEDURE_STMT_LIST, true));
    }
    SECTION("Undefined call") {
        REQUIRE_THROWS(pkbSetter->insertStmts(tcData.UNDEFINITED_CALL_STMT_LIST, true));
    }
}

TEST_CASE("Compute Reverse") {
    std::map<int, std::set<std::string>> normalMap {
            {1, {"a", "b", "c"}}
    };
    std::map<std::string, std::set<int>> reverseMap;
    DesignExtractor::computeReverse<int, std::string>(normalMap, reverseMap, {"a", "b", "c"});
    REQUIRE(!reverseMap.empty());
}

TEST_CASE("PATTERN RECOGNITION") {
    PKB pkb = PKB();
    PkbGetter* pkb_getter = pkb.getGetter();
    PkbSetter* pkb_setter = pkb.getSetter();

    ExpressionProcessor ep;

    PKB_PATTERN_TEST_CASE tcData;
    pkb_setter->insertStmts(tcData.stmtLists);

    SECTION("PATTERN") {
        std::set<std::pair<ProgramElement, ProgramElement>> pairResult;
        std::set<std::pair<ProgramElement, ProgramElement>> pairExpected;
        std::set<ProgramElement> result;
        std::set<ProgramElement> expected;
        SECTION("get assignment given expression - SINGLE VAR") {
            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("y"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(2)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("y"), ExpressionIndicator::PARTIAL_MATCH);
            expected = {
                    tcData.stmt.at(2),
                    tcData.stmt.at(7),
                    tcData.stmt.at(8),
                    tcData.stmt.at(9)
            };
            REQUIRE(result == expected);
        }

        SECTION("get assignment given expression - MULTI VAR") {
            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("x * y"), ExpressionIndicator::PARTIAL_MATCH);
            expected = {
                    tcData.stmt.at(7),
                    tcData.stmt.at(9)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenExpression(ep.stringToExpr("x + 5"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(1)
            };
            REQUIRE(result == expected);
        }

        SECTION("get assignment given var and expression - SINGLE var") {
            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("x"), ep.stringToExpr("pattern"), ExpressionIndicator::FULL_MATCH);
            expected = {
                    tcData.stmt.at(3)
            };
            REQUIRE(result == expected);

            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("y"), ep.stringToExpr("y"), ExpressionIndicator::PARTIAL_MATCH);
            expected = {
                    tcData.stmt.at(2),
                    tcData.stmt.at(9)
            };
            REQUIRE(result == expected);
        }

        SECTION("get assignment given var and expression - MULTI var") {
            result = pkb_getter->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable("y"), ep.stringToExpr("x * y"), ExpressionIndicator::PARTIAL_MATCH);
            expected = {
                    tcData.stmt.at(9)
            };
            REQUIRE(result == expected);
        }

        SECTION("get assignment given var and expression") {
            pairResult = pkb_getter->getAssignmentWithVariableGivenExpression(ep.stringToExpr("x"), ExpressionIndicator::PARTIAL_MATCH);
            pairExpected = {
                    std::make_pair(tcData.stmt.at(1), ProgramElement::createVariable("x")),
                    std::make_pair(tcData.stmt.at(7), ProgramElement::createVariable("x")),
                    std::make_pair(tcData.stmt.at(8), ProgramElement::createVariable("z")),
                    std::make_pair(tcData.stmt.at(9), ProgramElement::createVariable("y"))

            };
            REQUIRE(pairResult == pairExpected);

            pairResult = pkb_getter->getAssignmentWithVariableGivenExpression(ep.stringToExpr("x * y + 100"), ExpressionIndicator::FULL_MATCH);
            pairExpected = {
                    std::make_pair(tcData.stmt.at(7), ProgramElement::createVariable("x"))

            };
            REQUIRE(pairResult == pairExpected);
        }

        SECTION("get if/while expression given variable") {
            result = pkb_getter->getIfGivenVariable(ProgramElement::createVariable("z"));
            expected = {
                    tcData.stmt.at(6)

            };
            REQUIRE(result == expected);

            result = pkb_getter->getIfGivenVariable(ProgramElement::createVariable("y"));
            expected = {
                    tcData.stmt.at(6),
            };
            REQUIRE(result == expected);

            result = pkb_getter->getWhileGivenVariable(ProgramElement::createVariable("y"));
            expected = {
                    tcData.stmt.at(4)

            };
            REQUIRE(result == expected);

            result = pkb_getter->getWhileGivenVariable(ProgramElement::createVariable("x"));
            expected = {
                    tcData.stmt.at(4),
            };
            REQUIRE(result == expected);
        }

        SECTION("get if/while with variable") {
            pairResult = pkb_getter->getIfWithVariable();
            pairExpected = {
                    std::make_pair(tcData.stmt.at(6), ProgramElement::createVariable("y")),
                    std::make_pair(tcData.stmt.at(6), ProgramElement::createVariable("z"))

            };
            REQUIRE(pairResult == pairExpected);

            pairResult = pkb_getter->getWhileWithVariable();
            pairExpected = {
                    std::make_pair(tcData.stmt.at(4), ProgramElement::createVariable("y")),
                    std::make_pair(tcData.stmt.at(4), ProgramElement::createVariable("x"))

            };
            REQUIRE(pairResult == pairExpected);
        }
    }
}
