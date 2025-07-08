#ifndef IMP_VALUE
#define IMP_VALUE

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <memory>

using namespace std;


enum ImpVType { NOTYPE=0, TINT, TBOOL, TARRAY, TPOINTER };

class ImpValue {
public:
  ImpValue();
  ImpValue(int v) : type(TINT), int_value(v), bool_value(false) {}
  ImpValue(bool b) : type(TBOOL), int_value(0), bool_value(b) {}
  ImpValue(std::vector<ImpValue> arr) : type(TARRAY), array_value(arr) {}
  ImpValue(std::shared_ptr<ImpValue> ptr) : type(TPOINTER), ptr_value(ptr) {}

  ImpVType type;
  int int_value;
  bool bool_value;
  std::vector<ImpValue> array_value;
  std::shared_ptr<ImpValue> ptr_value;

  void set_default_value(ImpVType tt);
  static ImpVType get_basic_type(string s);
};

std::ostream& operator << ( std::ostream& outs, const ImpValue & v );


#endif
