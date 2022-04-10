#include "TestQueryPreprocessingAndPreOptimiserUtils.h"

Entity a = Entity(EntityType::ASSIGNMENT, "a");
Entity a1 = Entity(EntityType::ASSIGNMENT, "a1");
Entity a2 = Entity(EntityType::ASSIGNMENT, "a2");
Entity a3 = Entity(EntityType::ASSIGNMENT, "a3");

Entity astmt = Entity(EntityType::ASSIGNMENT, "a", EntityAttributeType::STMT);
Entity a1stmt = Entity(EntityType::ASSIGNMENT, "a1", EntityAttributeType::STMT);
Entity a2stmt = Entity(EntityType::ASSIGNMENT, "a2", EntityAttributeType::STMT);

Entity ifEntity = Entity(EntityType::IF, "ifs");
Entity s = Entity(EntityType::STATEMENT, "s");
Entity w = Entity(EntityType::WHILE, "w");
Entity boolean = Entity(EntityType::BOOLEAN, "BOOLEAN");
Entity wildcard = Entity(EntityType::WILDCARD, "_");

Entity cProcname = Entity(EntityType::CALL, "c", EntityAttributeType::PROCNAME);

Entity p = Entity(EntityType::PROCEDURE, "p");
Entity p1 = Entity(EntityType::PROCEDURE, "p1");
Entity pProcname = Entity(EntityType::PROCEDURE, "p", EntityAttributeType::PROCNAME);
Entity p1Procname = Entity(EntityType::PROCEDURE, "p1", EntityAttributeType::PROCNAME);

Entity countEntity = Entity(EntityType::FIXED_STRING, "count");
Entity x = Entity(EntityType::FIXED_STRING, "x");
Entity patternString = Entity(EntityType::FIXED_STRING, "pattern");
Entity countplus1Entity = Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, "count + 1");
Entity expr1 = Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, "(count + 1)");

Entity int1 = Entity(EntityType::FIXED_INTEGER, "1");
Entity int2 = Entity(EntityType::FIXED_INTEGER, "2");
Entity int3 = Entity(EntityType::FIXED_INTEGER, "3");
Entity int6 = Entity(EntityType::FIXED_INTEGER, "6");

Entity v = Entity(EntityType::VARIABLE, "v");
Entity v1 = Entity(EntityType::VARIABLE, "v1");
Entity vAttribute = Entity(EntityType::VARIABLE, "v", EntityAttributeType::VARNAME);
Entity v1Attribute = Entity(EntityType::VARIABLE, "v1", EntityAttributeType::VARNAME);








