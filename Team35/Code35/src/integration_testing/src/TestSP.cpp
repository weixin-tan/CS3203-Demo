#include "catch.hpp"
#include "PKB/PKB.h"
#include "SP.h"

TEST_CASE("SP round 0 iteration test") {
    PKB pkb = PKB();
    SP sp(pkb.getSetter());

    sp.Parse("iter0.txt");



}