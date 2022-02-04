#pragma once

#include "DB.h"
#include "PkbGetter.h"
#include "PkbSetter.h"

using namespace std;

class PKB {
  DB db;
  PkbGetter pkbGetter;
  PkbSetter pkbSetter;
public:
  PKB();
  PkbGetter* getGetter();
  PkbSetter* getSetter();
};
