#include "DB.h"

DB::DB() :
  stmtTable(),
  modifyStmtToVarTable(),
  modifyProcToVarTable(),
  varToModifyStmtTable(),
  varToModifyProcTable(),
  usesStmtToVarTable(),
  usesProcToVarTable(),
  varToUsesStmtTable(),
  varToUsesProcTable(),
  stmtTypeTable(),
  variables(),
  childToParentTable(),
  parentToChildTable(),
  stmtToProcedureCalled(),
  stmtPreceding(),
  stmtFollowing(),
  constantToStmtTable(),
  constants() {}
