#include "Clause.h"
#include "Entity.h"
#include "RelationshipRef.h"
#include "Type.h"
#include "catch.hpp"
#include <iostream>

using namespace std;

TEST_CASE("testing objects equality"){
  Entity e1 = Entity(EntityType::ASSIGNMENT, "a");
  Entity e1_clone = Entity(EntityType::ASSIGNMENT, "a");
  Entity e2 = Entity(EntityType::VARIABLE, "v");
  Entity e3 = Entity(EntityType::WILDCARD, "_");
  Entity e4 = Entity(EntityType::STATEMENT, "s");

  RelationshipRef r1 = RelationshipRef(RelationshipType::PARENT_T, e1, e2);
  RelationshipRef r1_clone = RelationshipRef(RelationshipType::PARENT_T, e1, e2);
  RelationshipRef r2 = RelationshipRef(RelationshipType::PATTERN, e3, e2, e1);
  RelationshipRef r2_clone = RelationshipRef(RelationshipType::PATTERN, e3, e2, e1);
  RelationshipRef r3 = RelationshipRef(RelationshipType::MODIFIES, e3, e4);
  RelationshipRef r4 = RelationshipRef(RelationshipType::PATTERN, e3, e4, e1);

  Clause c1 = Clause();
  c1.appendEntityToFind(e1);
  c1.appendRef(r1);

  Clause c1_clone = Clause();
  c1_clone.appendEntityToFind(e1);
  c1_clone.appendRef(r1);

  Clause c2 = Clause();
  c2.appendEntityToFind(e1);
  c2.appendRef(r2);

  Clause c3 = Clause();
  c3.appendEntityToFind(e3);
  c3.appendRef(r1);

  Clause c4 = Clause();
  c4.appendEntityToFind(e1);
  c4.appendEntityToFind(e2);
  c2.appendRef(r2);
  c4.appendRef(r4);

  Clause c4_1 = Clause();
  c4_1.appendEntityToFind(e2);
  c4_1.appendEntityToFind(e1);
  c4_1.appendRef(r2);
  c4_1.appendRef(r4);

  Clause c4_2 = Clause();
  c4_2.appendEntityToFind(e1);
  c4_2.appendEntityToFind(e2);
  c4_2.appendRef(r4);
  c4_2.appendRef(r2);

  SECTION("entity"){
    REQUIRE(e1 == e1_clone);
    REQUIRE(e1 == e1);
    REQUIRE(!(e1 == e2));
  }

  SECTION("relref"){
    REQUIRE((r1 == r1_clone));
    REQUIRE((r2 == r2_clone));
    REQUIRE(!(r1 == r2));
    REQUIRE(!(r1 == r3));
    REQUIRE(!(r1 == r4));
    REQUIRE(!(r2 == r3));
    REQUIRE(!(r2 == r3));
    REQUIRE(!(r3 == r4));
  }

  SECTION("clause"){
    REQUIRE((c1 == c1_clone));
    REQUIRE(!(c1 == c2));
    REQUIRE(!(c1 == c3));
    REQUIRE(!(c2 == c3));
    REQUIRE((c4 == c4_1));
    REQUIRE((c4 == c4_2));
    REQUIRE((c4_1 == c4_2));
  }
}

