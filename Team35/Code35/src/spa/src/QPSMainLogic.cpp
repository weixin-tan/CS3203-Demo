//
// Created by viv on 4/2/2022.
//

#include "QPSMainLogic.h"

// Global static pointer to ensure only a single instance of the class is initialized.
QPSMainLogic* QPSMainLogic::instance = nullptr; // Null, will be initialized on demand.

// Called to create an instance of the class if one has not been created already.
// Otherwise, return the instance that has already been created.
QPSMainLogic* QPSMainLogic::getInstance() {
  if (!instance) { // Makes sure only one instance of the class is generated.
    instance = new QPSMainLogic();
  }
  return instance;
}

// Default constructor
QPSMainLogic::QPSMainLogic() = default;

