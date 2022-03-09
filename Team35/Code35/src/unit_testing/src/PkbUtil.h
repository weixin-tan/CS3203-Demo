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
                    }

            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::kAssignment, 1)},
                            {2, ProgramElement::createStatement(ElementType::kAssignment, 2)},
                            {3, ProgramElement::createStatement(ElementType::kAssignment, 3)},
                            {4, ProgramElement::createStatement(ElementType::kWhile, 4)},
                            {5, ProgramElement::createStatement(ElementType::kRead, 5, "z")},
                            {6, ProgramElement::createStatement(ElementType::kIf, 6)},
                            {7, ProgramElement::createStatement(ElementType::kAssignment, 7)},
                            {8, ProgramElement::createStatement(ElementType::kAssignment, 8)},
                            {9, ProgramElement::createStatement(ElementType::kAssignment, 9)},
                            {10, ProgramElement::createStatement(ElementType::kAssignment, 10)},
                            {11, ProgramElement::createStatement(ElementType::kPrint, 11, "x")},
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
                    }
            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::kWhile, 1)},
                            {2, ProgramElement::createStatement(ElementType::kRead, 2, "proc1Var2")},
                            {3, ProgramElement::createStatement(ElementType::kCall, 3, "proc2")},
                            {4, ProgramElement::createStatement(ElementType::kAssignment, 4)},
                            {5, ProgramElement::createStatement(ElementType::kCall, 5, "proc3")},
                            {6, ProgramElement::createStatement(ElementType::kRead, 6, "proc3Var")},
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
                    }
            ),
            stmt(
                    {
                            {1, ProgramElement::createStatement(ElementType::kWhile, 1)},
                            {2, ProgramElement::createStatement(ElementType::kPrint, 2, "proc1Var2")},
                            {3, ProgramElement::createStatement(ElementType::kCall, 3, "proc2")},
                            {4, ProgramElement::createStatement(ElementType::kAssignment, 4)},
                            {5, ProgramElement::createStatement(ElementType::kCall, 5, "proc3")},
                            {6, ProgramElement::createStatement(ElementType::kPrint, 6, "proc3Var")},
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

struct PKB_VALIDATION_TEST_CASES {
    std::vector<std::vector<ParsedStatement>> RECURSIVE_CALL_STMT_LIST =
            {
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
    std::vector<std::vector<ParsedStatement>> CYCLIC_CALL_STMT_LIST =
            {
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
    std::vector<std::vector<ParsedStatement>> DUPLICATE_PROCEDURE_STMT_LIST =
            {
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
    std::vector<std::vector<ParsedStatement>> UNDEFINITED_CALL_STMT_LIST =
            {
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

};

#endif //SPA_TEAM35_CODE35_SRC_UNIT_TESTING_SRC_PKBUTIL_H_