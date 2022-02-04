#include "PKB.h"

PKB::PKB() : db(), pkbGetter(&db), pkbSetter(&db) {}

PkbGetter* PKB::getGetter() {
  return &pkbGetter;
}

PkbSetter* PKB::getSetter() {
  return &pkbSetter;
}
