#include "PKB.h"

PKB::PKB() : db(), pkbGetter(&db) {}

PkbGetter* PKB::getGetter() {
  return &pkbGetter;
}
