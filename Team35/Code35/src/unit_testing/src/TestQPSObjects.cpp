#include "Clause.h"
#include "Entity.h"
#include "RelationshipRef.h"
#include "Type.h"
#include "catch.hpp"
#include <iostream>

using namespace std;

TEST_CASE("testing objects equality"){
  Entity e1 = Entity(EntityType::Assignment, "a");
  Entity e1_clone = Entity(EntityType::Assignment, "a");
  Entity e2 = Entity(EntityType::Variable, "v");
  Entity e3 = Entity(EntityType::Wildcard, "_");
  Entity e4 = Entity(EntityType::Statement, "s");

  RelationshipRef r1 = RelationshipRef(RelationshipType::ParentT, e1, e2);
  RelationshipRef r1_clone = RelationshipRef(RelationshipType::ParentT, e1, e2);
  RelationshipRef r2 = RelationshipRef(RelationshipType::Pattern, e3, e2, e1);
  RelationshipRef r2_clone = RelationshipRef(RelationshipType::Pattern, e3, e2, e1);
  RelationshipRef r3 = RelationshipRef(RelationshipType::Modifies, e3, e4);
  RelationshipRef r4 = RelationshipRef(RelationshipType::Pattern, e3, e4, e1);

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
  }
}

