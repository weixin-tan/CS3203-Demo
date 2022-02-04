//
// Created by viv on 4/2/2022.
//

#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H


class QPSMainLogic {
 public:
  static QPSMainLogic* getInstance(); // Static access method

 private:
  QPSMainLogic(); // Make constructor private
  QPSMainLogic(const QPSMainLogic&); // Make copy constructor private
  QPSMainLogic& operator = (const QPSMainLogic&); // Make assignment operator private
  static QPSMainLogic* instance; // The instance of QPSMainLogic will be stored here

};


#endif //SPA_QPSMAINLOGIC_H
