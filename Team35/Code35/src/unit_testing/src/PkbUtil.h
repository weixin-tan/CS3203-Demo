#ifndef SPA_TEAM35_CODE35_SRC_UNIT_TESTING_SRC_PKBUTIL_H_
#define SPA_TEAM35_CODE35_SRC_UNIT_TESTING_SRC_PKBUTIL_H_

#include <vector>
#include <map>
#include "ParsedStatement.h"
#include "ProgramElement.h"

struct PKB_BASIC_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;
    ExpressionProcessor ep;


    /* FOR TESTING REFERENCE
    procedure f {
      x = 5;  // 1
      y = x;  // 2
      x = 5;  // 3
      while (x > 4) {  // 4
        read z;  // 5
        if (z < 10) then {  // 6
          x = 100;  // 7
          z = 5;  // 8
        } else {
          y = 100;  // 9
          z = 0;  // 10
        }
      }
      print x; // 11
    }
    */
    PKB_BASIC_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(1,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("5"),
                                                    "f",
                                                    {},
                                                    {"x"},
                                                    {"5"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO),
                                    ParsedStatement(2,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("x"),
                                                    "f",
                                                    {"x"},
                                                    {"y"},
                                                    {},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    1),
                                    ParsedStatement(3,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("5"),
                                                    "f",
                                                    {},
                                                    {"x"},
                                                    {"5"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    2),
                                    ParsedStatement(4,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::WHILE_STMT,
                                                    ep.stringToExpr("x > 4"),
                                                    "f",
                                                    {"x"},
                                                    {},
                                                    {"4"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    3),
                                    ParsedStatement(5,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    4,
                                                    StatementType::READ_STMT,
                                                    ParsedStatement::defaultPattern,
                                                    "f",
                                                    {},
                                                    {"z"},
                                                    {},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO),
                                    ParsedStatement(6,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    4,
                                                    StatementType::IF_STMT,
                                                    ep.stringToExpr("z > 10"),
                                                    "f",
                                                    {"z"},
                                                    {},
                                                    {"10"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    5),
                                    ParsedStatement(7,
                                                    6,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("100"),
                                                    "f",
                                                    {},
                                                    {"x"},
                                                    {"100"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO),
                                    ParsedStatement(8,
                                                    6,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("5"),
                                                    "f",
                                                    {},
                                                    {"z"},
                                                    {"5"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    7),
                                    ParsedStatement(9,
                                                    6,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("100"),
                                                    "f",
                                                    {},
                                                    {"y"},
                                                    {"100"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO),
                                    ParsedStatement(10,
                                                    6,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::ASSIGNMENT_STMT,
                                                    ep.stringToExpr("0"),
                                                    "f",
                                                    {},
                                                    {"z"},
                                                    {"0"},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    9),
                                    ParsedStatement(11,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                    StatementType::PRINT_STMT,
                                                    ParsedStatement::defaultPattern,
                                                    "f",
                                                    {"x"},
                                                    {},
                                                    {},
                                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                    4),
                            }
                    }

            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1)},
                            {2, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2)},
                            {3, ProgramElement::createStatement(ElementType::ASSIGNMENT, 3)},
                            {4, ProgramElement::createStatement(ElementType::WHILE, 4)},
                            {5, ProgramElement::createStatement(ElementType::READ, 5, "z")},
                            {6, ProgramElement::createStatement(ElementType::IF, 6)},
                            {7, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7)},
                            {8, ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)},
                            {9, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)},
                            {10, ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)},
                            {11, ProgramElement::createStatement(ElementType::PRINT, 11, "x")},
                    }

            ),
            vars(
                    {
                            {"x", ProgramElement::createVariable("x")},
                            {"y", ProgramElement::createVariable("y")},
                            {"z", ProgramElement::createVariable("z")},
                    }
            ),
            procs(
                    {
                            {"f", ProgramElement::createProcedure("f")},
                    }
            ),
            constants(
                    {
                            {"5", ProgramElement::createVariable("5")},
                            {"4", ProgramElement::createVariable("4")},
                            {"10", ProgramElement::createVariable("10")},
                            {"100", ProgramElement::createVariable("100")},
                            {"0", ProgramElement::createVariable("0")},
                    }
            )
    {}
};

struct PKB_CALLS_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /* FOR TESTING REFERENCE
    proc1 calls proc2
    proc2 calls proc3
    proc2 calls proc4
    proc3 calls proc4
    proc4 calls proc5
    proc4 calls proc6
    */
    PKB_CALLS_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc1",
                                            {},
                                            {},
                                            {},
                                            "proc2",
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    )
                            },
                            {
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc2",
                                            {},
                                            {},
                                            {},
                                            "proc3",
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc3",
                                            {},
                                            {},
                                            {},
                                            "proc4",
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            },
                            {
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc4",
                                            {},
                                            {},
                                            {},
                                            "proc5",
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            6,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc4",
                                            {},
                                            {},
                                            {},
                                            "proc6",
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            },
                            {
                                    ParsedStatement(
                                            7,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc5",
                                            {"b"},
                                            {"a"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            },
                            {
                                    ParsedStatement(
                                            8,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc6",
                                            {},
                                            {"c"},
                                            {"100"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            },
                    }
            ),
            procs(
                    {
                            {"proc1", ProgramElement::createProcedure("proc1")},
                            {"proc2", ProgramElement::createProcedure("proc2")},
                            {"proc3", ProgramElement::createProcedure("proc3")},
                            {"proc4", ProgramElement::createProcedure("proc4")},
                            {"proc5", ProgramElement::createProcedure("proc5")},
                            {"proc6", ProgramElement::createProcedure("proc6")},
                    }
            ) {}
};

struct PKB_MODIFIES_P_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /* FOR TESTING REFERENCE
    procedure proc1 {
        proc1Var = 5 + x;
        call proc2;
        call proc3;
    }

    procedure proc2 {
        read proc2Var;
        call proc4;
        call proc5;
    }

    procedure proc3 {
        read proc3Var;
        call proc5;
    }

    procedure proc4 {
        proc4Var = 123;
    }

    procedure proc5 {
        proc5Var = 456;
    }
    */
    PKB_MODIFIES_P_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc1",
                                            {"x"},
                                            {"proc1Var"},
                                            {"5"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc1",
                                            {},
                                            {},
                                            {},
                                            "proc2",
                                            1
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc2",
                                            {},
                                            {"proc2Var"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc2",
                                            {},
                                            {},
                                            {},
                                            "proc4",
                                            4
                                    ),
                                    ParsedStatement(
                                            6,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc3",
                                            {},
                                            {"proc3Var"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            8,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc4",
                                            {},
                                            {"proc4Var"},
                                            {"123"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    )
                            },
                            {
                                    ParsedStatement(
                                            10,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc5",
                                            {},
                                            {"proc5Var"},
                                            {"456"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    )
                            }
                    }
            ),
            procs(
                    {
                            {"proc1", ProgramElement::createProcedure("proc1")},
                            {"proc2", ProgramElement::createProcedure("proc2")},
                            {"proc3", ProgramElement::createProcedure("proc3")},
                            {"proc4", ProgramElement::createProcedure("proc4")},
                            {"proc5", ProgramElement::createProcedure("proc5")},
                    }
            ),
            vars(
                    {
                            {"x", ProgramElement::createVariable("x")},
                            {"proc1Var", ProgramElement::createVariable("proc1Var")},
                            {"proc2Var", ProgramElement::createVariable("proc2Var")},
                            {"proc3Var", ProgramElement::createVariable("proc3Var")},
                            {"proc4Var", ProgramElement::createVariable("proc4Var")},
                            {"proc5Var", ProgramElement::createVariable("proc5Var")},
                    }
            ) {}
};

struct PKB_MODIFIES_S_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /*  FOR TESTING REFERENCE
    procedure proc1 {
        while (proc1Var1 == 5) {
            read proc1Var2;
            call proc2;
        }
    }

    procedure proc2 {
        proc2Var = 100;
        call proc3;
    }

    procedure proc3 {
        read proc3Var;
    }
    */
    PKB_MODIFIES_S_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::WHILE_STMT,
                                            Expr(),
                                            "proc1",
                                            {"proc1Var1"},
                                            {},
                                            {"5"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            1,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc1",
                                            {},
                                            {"proc1Var2"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            1,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc2",
                                            {},
                                            {"proc2Var"},
                                            {"100"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc3",
                                            {},
                                            {"proc3Var"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            }
                    }
            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::WHILE, 1)},
                            {2, ProgramElement::createStatement(ElementType::READ, 2, "proc1Var2")},
                            {3, ProgramElement::createStatement(ElementType::CALL, 3, "proc2")},
                            {4, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4)},
                            {5, ProgramElement::createStatement(ElementType::CALL, 5, "proc3")},
                            {6, ProgramElement::createStatement(ElementType::READ, 6, "proc3Var")},
                    }
            ),
            procs(
                    {
                            {"proc1", ProgramElement::createProcedure("proc1")},
                            {"proc2", ProgramElement::createProcedure("proc2")},
                            {"proc3", ProgramElement::createProcedure("proc3")},
                    }
            ),
            vars(
                    {
                            {"proc1Var1", ProgramElement::createVariable("proc1Var1")},
                            {"proc1Var2", ProgramElement::createVariable("proc1Var2")},
                            {"proc2Var", ProgramElement::createVariable("proc2Var")},
                            {"proc3Var", ProgramElement::createVariable("proc3Var")},
                    }
            ) {}
};

struct PKB_USES_P_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /* FOR TESTING REFERENCE
    procedure proc1 {
        proc1Var = 5 + x;
        call proc2;
        call proc3;
    }

    procedure proc2 {
        print proc2Var;
        call proc4;
        call proc5;
    }

    procedure proc3 {
        print proc3Var;
        call proc5;
    }

    procedure proc4 {
        abc = proc4Var;
    }

    procedure proc5 {
        def = proc5Var;
    }
    */

    PKB_USES_P_TEST_CASE() :
            stmtLists(
                    {

                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::READ_STMT,
                                            Expr(),
                                            "proc1",
                                            {"x"},
                                            {"proc1Var"},
                                            {"5"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc1",
                                            {},
                                            {},
                                            {},
                                            "proc2",
                                            1
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::PRINT_STMT,
                                            Expr(),
                                            "proc2",
                                            {"proc2Var"},
                                            {},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "proc2",
                                            {},
                                            {},
                                            {},
                                            "proc4",
                                            4
                                    ),
                                    ParsedStatement(
                                            6,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::PRINT_STMT,
                                            Expr(),
                                            "proc3",
                                            {"proc3Var"},
                                            {},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            8,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc4",
                                            {"proc4Var"},
                                            {"abc"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    )
                            },
                            {
                                    ParsedStatement(
                                            10,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc5",
                                            {"proc5Var"},
                                            {"def"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    )
                            }
                    }
            ),
            procs(
                    {
                            {"proc1", ProgramElement::createProcedure("proc1")},
                            {"proc2", ProgramElement::createProcedure("proc2")},
                            {"proc3", ProgramElement::createProcedure("proc3")},
                            {"proc4", ProgramElement::createProcedure("proc4")},
                            {"proc5", ProgramElement::createProcedure("proc5")},
                    }
            ),
            vars(
                    {
                            {"x", ProgramElement::createVariable("x")},
                            {"proc1Var", ProgramElement::createVariable("proc1Var")},
                            {"proc2Var", ProgramElement::createVariable("proc2Var")},
                            {"proc3Var", ProgramElement::createVariable("proc3Var")},
                            {"proc4Var", ProgramElement::createVariable("proc4Var")},
                            {"proc5Var", ProgramElement::createVariable("proc5Var")},
                    }
            ){}
};

struct PKB_USES_S_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /* FOR TESTING REFERENCE
    procedure proc1 {
        while (proc1Var1 == 5) {
            print proc1Var2;
            call proc2;
        }
    }

    procedure proc2 {
        abc = proc2Var;
        call proc3;
    }

    procedure proc3 {
        print proc3Var;
    }
    */

    PKB_USES_S_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::WHILE_STMT,
                                            Expr(),
                                            "proc1",
                                            {"proc1Var1"},
                                            {},
                                            {"5"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            1,
                                            StatementType::PRINT_STMT,
                                            Expr(),
                                            "proc1",
                                            {"proc1Var2"},
                                            {},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            1,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "proc2",
                                            {"proc2Var"},
                                            {"abc"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::CALL_STMT,
                                            Expr(),
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
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::PRINT_STMT,
                                            Expr(),
                                            "proc3",
                                            {"proc3Var"},
                                            {},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                            }
                    }
            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::WHILE, 1)},
                            {2, ProgramElement::createStatement(ElementType::PRINT, 2, "proc1Var2")},
                            {3, ProgramElement::createStatement(ElementType::CALL, 3, "proc2")},
                            {4, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4)},
                            {5, ProgramElement::createStatement(ElementType::CALL, 5, "proc3")},
                            {6, ProgramElement::createStatement(ElementType::PRINT, 6, "proc3Var")},
                    }
            ),
            procs(
                    {
                            {"proc1", ProgramElement::createProcedure("proc1")},
                            {"proc2", ProgramElement::createProcedure("proc2")},
                            {"proc3", ProgramElement::createProcedure("proc3")},
                    }
            ),
            vars(
                    {
                            {"proc1Var1", ProgramElement::createVariable("proc1Var1")},
                            {"proc1Var2", ProgramElement::createVariable("proc1Var2")},
                            {"proc2Var", ProgramElement::createVariable("proc2Var")},
                            {"proc3Var", ProgramElement::createVariable("proc3Var")},
                    }
            ){}
};

struct PKB_NEXT_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;

    /* FOR TESTING REFERENCE
    procedure Second {
        x = 0; // 1
        i = 5; // 2
        while (i!=0) { // 3
            x = x + 2 * y; // 4
            call Third; // 5
            i = i - 1; } // 6
        if (x==1) then { // 7
            x = x+1; } // 8
        else {
            z = 1; } // 9
        z = z + x + i; // 10
        y = z + 2; // 11
        x = x * y + z; // 12
    }

    procedure Third {
        while (c > 0) { // 13
            if (b > 0) then { // 14
                d = v + a; } // 15
            else {
                a = x * y + v * y + d; } } // 16
        if (b > 0) then {  // 17
            if (b > 0) then { // 18
                d = v + a; } // 19
            else {
                a = x * y + v * y + d; } } // 20
        else {
            a = x * y + v * y + d; } // 21
        d = v + a; // 22
    }
    */

    PKB_NEXT_TEST_CASE() :
            stmtLists(
                    {
                            {
                                    ParsedStatement(
                                            1,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {},
                                            {"x"},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            2,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {},
                                            {"i"},
                                            {"5"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            1
                                    ),
                                    ParsedStatement(
                                            3,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::WHILE_STMT,
                                            Expr(),
                                            "Second",
                                            {"i"},
                                            {},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            2
                                    ),
                                    ParsedStatement(
                                            4,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            3,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"x", "y"},
                                            {"x"},
                                            {"2"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            5,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            3,
                                            StatementType::CALL_STMT,
                                            Expr(),
                                            "Second",
                                            {},
                                            {},
                                            {},
                                            "Third",
                                            4
                                    ),
                                    ParsedStatement(
                                            6,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            3,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"i"},
                                            {"i"},
                                            {"1"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            5
                                    ),
                                    ParsedStatement(
                                            7,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::IF_STMT,
                                            Expr(),
                                            "Second",
                                            {"x"},
                                            {},
                                            {"1"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            3
                                    ),
                                    ParsedStatement(
                                            8,
                                            7,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"x"},
                                            {"x"},
                                            {"1"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            9,
                                            7,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {},
                                            {"z"},
                                            {"1"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            10,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"z", "x"},
                                            {"z"},
                                            {"1"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            7
                                    ),
                                    ParsedStatement(
                                            11,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"z"},
                                            {"y"},
                                            {"2"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            10
                                    ),
                                    ParsedStatement(
                                            12,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Second",
                                            {"x", "y", "z"},
                                            {"x"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            11
                                    ),
                            },
                            {
                                    ParsedStatement(
                                            13,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::WHILE_STMT,
                                            Expr(),
                                            "Third",
                                            {"c"},
                                            {},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            14,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            13,
                                            StatementType::IF_STMT,
                                            Expr(),
                                            "Third",
                                            {"b"},
                                            {},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            15,
                                            14,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Third",
                                            {"v", "a"},
                                            {"d"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            16,
                                            14,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Third",
                                            {"x", "y", "v", "d"},
                                            {"a"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            17,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::IF_STMT,
                                            Expr(),
                                            "Third",
                                            {"b"},
                                            {},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            13
                                    ),
                                    ParsedStatement(
                                            18,
                                            17,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::IF_STMT,
                                            Expr(),
                                            "Third",
                                            {"b"},
                                            {},
                                            {"0"},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            19,
                                            18,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Third",
                                            {"v", "a"},
                                            {"d"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            20,
                                            18,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                            Expr(),
                                            "Third",
                                            {"x", "y", "v", "d"},
                                            {"a"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            21,
                                            17,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                        Expr(),
                                            "Third",
                                            {"x", "y", "v", "d"},
                                            {"a"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO
                                    ),
                                    ParsedStatement(
                                            22,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            ParsedStatement::DEFAULT_NULL_STMT_NO,
                                            StatementType::ASSIGNMENT_STMT,
                                        Expr(),
                                            "Third",
                                            {"v", "a"},
                                            {"d"},
                                            {},
                                            ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                            17
                                    ),
                            }
                    }
            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1)},
                            {2, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2)},
                            {3, ProgramElement::createStatement(ElementType::WHILE, 3)},
                            {4, ProgramElement::createStatement(ElementType::ASSIGNMENT, 4)},
                            {5, ProgramElement::createStatement(ElementType::CALL, 5, "Third")},
                            {6, ProgramElement::createStatement(ElementType::ASSIGNMENT, 6)},
                            {7, ProgramElement::createStatement(ElementType::IF, 7)},
                            {8, ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)},
                            {9, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)},
                            {10, ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)},
                            {11, ProgramElement::createStatement(ElementType::ASSIGNMENT, 11)},
                            {12, ProgramElement::createStatement(ElementType::ASSIGNMENT, 12)},
                            {13, ProgramElement::createStatement(ElementType::WHILE, 13)},
                            {14, ProgramElement::createStatement(ElementType::IF, 14)},
                            {15, ProgramElement::createStatement(ElementType::ASSIGNMENT, 15)},
                            {16, ProgramElement::createStatement(ElementType::ASSIGNMENT, 16)},
                            {17, ProgramElement::createStatement(ElementType::IF, 17)},
                            {18, ProgramElement::createStatement(ElementType::IF, 18)},
                            {19, ProgramElement::createStatement(ElementType::ASSIGNMENT, 19)},
                            {20, ProgramElement::createStatement(ElementType::ASSIGNMENT, 20)},
                            {21, ProgramElement::createStatement(ElementType::ASSIGNMENT, 21)},
                            {22, ProgramElement::createStatement(ElementType::ASSIGNMENT, 22)},
                    }
            ) {}
};

struct PKB_VALIDATION_TEST_CASES {
    std::vector<std::vector<ParsedStatement>> RECURSIVE_CALL_STMT_LIST =
            {
                    {
                            ParsedStatement(
                                    1,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc1",
                                    {},
                                    {},
                                    {},
                                    "proc1",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    }
            };
    std::vector<std::vector<ParsedStatement>> CYCLIC_CALL_STMT_LIST =
            {
                    {
                            ParsedStatement(
                                    1,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc1",
                                    {},
                                    {},
                                    {},
                                    "proc2",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    },
                    {
                            ParsedStatement(
                                    2,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc2",
                                    {},
                                    {},
                                    {},
                                    "proc3",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            ),
                            ParsedStatement(
                                    3,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
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
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc3",
                                    {},
                                    {},
                                    {},
                                    "proc4",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            ),
                    },
                    {
                            ParsedStatement(
                                    5,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc4",
                                    {},
                                    {},
                                    {},
                                    "proc5",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            ),
                            ParsedStatement(
                                    6,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc4",
                                    {},
                                    {},
                                    {},
                                    "proc6",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            ),
                    },
            };
    std::vector<std::vector<ParsedStatement>> DUPLICATE_PROCEDURE_STMT_LIST =
            {
                    {
                            ParsedStatement(
                                    1,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc1",
                                    {},
                                    {},
                                    {},
                                    "proc2",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    },
                    {
                            ParsedStatement(
                                    2,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::PRINT_STMT,
                                    Expr(),
                                    "proc2",
                                    {"abc"},
                                    {},
                                    {},
                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    },
                    {
                            ParsedStatement(
                                    3,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::PRINT_STMT,
                                    Expr(),
                                    "proc1",
                                    {"abc"},
                                    {},
                                    {},
                                    ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    },
            };
    std::vector<std::vector<ParsedStatement>> UNDEFINITED_CALL_STMT_LIST =
            {
                    {
                            ParsedStatement(
                                    1,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    ParsedStatement::DEFAULT_NULL_STMT_NO,
                                    StatementType::CALL_STMT,
                                    Expr(),
                                    "proc1",
                                    {},
                                    {},
                                    {},
                                    "proc2",
                                    ParsedStatement::DEFAULT_NULL_STMT_NO
                            )
                    },
            };

};


/* FOR TESTING REFERENCE
    procedure f {
      x = x+5;  // 1
      y = y;  // 2
      x = pattern;  // 3
      while (x > y) {  // 4
        read z;  // 5
        if (z > y) then {  // 6
          x = x * y + 100;  // 7
          z = (x+y) * z;  // 8
        } else {
          y = x * y * z - y + (y * f);  // 9
          z = 0;  // 10
        }
      }
      print x; // 11
    }
    */

struct PKB_PATTERN_TEST_CASE {
    std::vector<std::vector<ParsedStatement>> stmtLists;
    std::map<int, ProgramElement> stmt;
    std::map<std::string, ProgramElement> procs;
    std::map<std::string, ProgramElement> vars;
    std::map<std::string, ProgramElement> constants;
    ExpressionProcessor ep;
    
    PKB_PATTERN_TEST_CASE():
    stmtLists(
        {
                {
                        ParsedStatement(1,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("x + 5"),
                                        "f",
                                        {"x"},
                                        {"x"},
                                        {"5"},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO),
                        ParsedStatement(2,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("y"),
                                        "f",
                                        {"y"},
                                        {"y"},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        1),
                        ParsedStatement(3,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("pattern"),
                                        "f",
                                        {"pattern"},
                                        {"x"},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        2),
                        ParsedStatement(4,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::WHILE_STMT,
                                        ep.stringToExpr("x > y"),
                                        "f"
                                        ,
                                        {"x", "y"},
                                        {},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        3),
                        ParsedStatement(5,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        4,
                                        StatementType::READ_STMT,
                                        ParsedStatement::defaultPattern,
                                        "f",
                                        {},
                                        {"z"},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO),
                        ParsedStatement(6,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        4,
                                        StatementType::IF_STMT,
                                        ep.stringToExpr("z > y"),
                                        "f",
                                        {"y","z"},
                                        {},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        5),
                        ParsedStatement(7,
                                        6,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("x * y + 100"),
                                        "f",
                                        {"x", "y"},
                                        {"x"},
                                        {"100"},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO),
                        ParsedStatement(8,
                                        6,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("(x+y) * z"),
                                        "f",
                                        {"x","y","z"},
                                        {"z"},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        7),
                        ParsedStatement(9,
                                        6,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("x * y * z - y + (y * f)"),
                                        "f",
                                        {"y","x","z","f"},
                                        {"y"},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO),
                        ParsedStatement(10,
                                        6,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::ASSIGNMENT_STMT,
                                        ep.stringToExpr("0"),
                                        "f",
                                        {},
                                        {"z"},
                                        {"0"},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        9),
                        ParsedStatement(11,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                                        StatementType::PRINT_STMT,
                                        ParsedStatement::defaultPattern,
                                        "f",
                                        {"x"},
                                        {},
                                        {},
                                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                        4),
                }
        }

    ),
    stmt(
        {
                {1, ProgramElement::createStatement(ElementType::ASSIGNMENT, 1)},
                {2, ProgramElement::createStatement(ElementType::ASSIGNMENT, 2)},
                {3, ProgramElement::createStatement(ElementType::ASSIGNMENT, 3)},
                {4, ProgramElement::createStatement(ElementType::WHILE, 4)},
                {5, ProgramElement::createStatement(ElementType::READ, 5, "z")},
                {6, ProgramElement::createStatement(ElementType::IF, 6)},
                {7, ProgramElement::createStatement(ElementType::ASSIGNMENT, 7)},
                {8, ProgramElement::createStatement(ElementType::ASSIGNMENT, 8)},
                {9, ProgramElement::createStatement(ElementType::ASSIGNMENT, 9)},
                {10, ProgramElement::createStatement(ElementType::ASSIGNMENT, 10)},
                {11, ProgramElement::createStatement(ElementType::PRINT, 11, "x")},
        }

        ),
    vars(
        {       
                {"f", ProgramElement::createVariable("f")},
                {"pattern", ProgramElement::createVariable("pattern")},
                {"x", ProgramElement::createVariable("x")},
                {"y", ProgramElement::createVariable("y")},
                {"z", ProgramElement::createVariable("z")},
        }
        ),
    procs(
        {
                {"f", ProgramElement::createProcedure("f")},
        }
        ),
    constants(
        {
                {"5", ProgramElement::createVariable("5")},
                {"10", ProgramElement::createVariable("10")},
                {"100", ProgramElement::createVariable("100")},
                {"0", ProgramElement::createVariable("0")},
        }
        )
{}
};

#endif //SPA_TEAM35_CODE35_SRC_UNIT_TESTING_SRC_PKBUTIL_H_