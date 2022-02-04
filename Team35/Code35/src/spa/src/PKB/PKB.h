#pragma once

#include "DB.h"
#include "PkbGetter.h"

using namespace std;

class PKB {
  DB db;
  PkbGetter pkbGetter;
public:
  PKB();
  PkbGetter* getGetter();

};
