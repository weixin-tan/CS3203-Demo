//
// Created by Prerthan Munireternam on 14/02/2022.
//

#include <unordered_map>

#include "QPS/Entity.h"
#include "catch.hpp"
#include "Tokeniser.h"

using namespace std;

TEST_CASE("test basic token methods") {
    string ident1 = "a";
    string ident2 = "a1";
    string ident3 = "abcdefg";
    string ident4 = "a123456";

    string integer1 = "1";
    string integer2 = "123456789012309830912";
    string falseinteger1 = "-1";

    string space1 = "a=b";
    string space2 = "a= b";
    string space3 = "a= b";
    string space4 = "a =  b";

    string quotationident1 = "\"a\"";
    string quotationident2 = "\"a123\"";
    string quotationident3 = "\"abc\"";
    string falsequotationident1 = "\"123\"";
    string falsequotationident2 = "\"$@!\"";



}