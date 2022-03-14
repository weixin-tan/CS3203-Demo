#include "PKB.h"

// TODO: add a debug flag to disable type checking during release
PKB::PKB() : db(), pkbGetter(&db), pkbSetter(&db) {}

PkbGetter* PKB::getGetter() {
  return &pkbGetter;
}

PkbSetter* PKB::getSetter() {
  return &pkbSetter;
}
