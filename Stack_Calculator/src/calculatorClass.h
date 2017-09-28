#ifndef CALCULATORCLASS_H
#define CALCULATORCLASS_H
#include "stack.h"
using namespace std;

class calculator
{
private:
  float operand1, operand2;
  Stack<char> operators;
  Stack<float> postfixexpression;
  string initial;
public:
  calculator();
  bool checkString(string&);
  float calc(string&);
  string getInitial();
};

#endif
