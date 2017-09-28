#include <iostream>
#include <cstring>
#include <string>
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip>
#include <math.h>
#include "calculatorClass.h"

using namespace std;

calculator::calculator()
{
  initial = "...";
}

bool calculator::checkString(string &expression)
{

  bool isvalid = true;
  int digitCount = 0, opCount = 0, openCount = 0, closeCount = 0;

  for(int i = 0; i < expression.size(); i++)
  {
    if(isdigit(expression[i]) && (expression[i + 1] == '*' || expression[i + 1] == '/' || expression[i + 1] == '+' || expression[i + 1] == '-'))
    {
      digitCount++;
    }
    else if(isdigit(expression[i]))
    {
      while(isdigit(expression[i]))
      {
        i++;
      }
      digitCount++;
      i--;
    }
    else if(expression[i] == 's' && !isdigit(expression[i + 1]) && expression[i + 1] != '(')
    {
      return false;
    }
    else if(expression[i] == 'p' && !isdigit(expression[i - 1]) && expression[i - 1] != ')')
    {
      return false;
    }
    else if(expression[i] == '*' || expression[i] == '/' || expression[i] == '+' || expression[i] == '-')
    {
      opCount++;
    }
    else if(expression[i] == '(')
    {
      openCount++;
    }
    else if(expression[i] == ')')
    {
      closeCount++;
    }
    else if(closeCount > openCount)
    {
      return false;
    }
    else if(isspace(expression[i]))
    {
      return false;
    }
  }

  if((opCount != digitCount - 1) || (openCount != closeCount))
  {
    return false;
  }

  return isvalid;
}

float calculator::calc(string &expression)
{
  for(int i = 0; i < expression.size(); i++)
  {
    int numCount = 0;
    if(isdigit(expression[i]))
    {
      int curPos = i, bigNumber = expression[i] - '0';

      while((curPos != expression.size()) && isdigit(expression[curPos]))
      {
        numCount++;
        curPos++;
      }
      if(numCount == 3)
      {
        bigNumber *= 100;
        bigNumber += (expression[i + 1] - '0') * 10;
        bigNumber += expression[i + 2] - '0';
      }
      else if(numCount == 2)
      {
        bigNumber *= 10;
        bigNumber += (expression[i + 1] - '0');
      }
      numCount--;
      i += numCount;
      postfixexpression.push((float)bigNumber); //add all constants to postfix stack
    }
    else if(expression[i] == '(') //add ( to postfix stack
    {
      operators.push(expression[i]);
    }
    else if(expression[i] == ')')
    {
      while(operators.peek() != '(') //if closing bracket spotted, add all operators to end of postfix stack until ( is spotted in operators stack
      {
        if(operators.peek() == 's')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(sqrt(operand1));
        }
        else if(operators.peek() == '*')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand1*operand2);
        }
        else if(operators.peek() == '/')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2/operand1);
        }
        else if(operators.peek() == '+')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2+operand1);
        }
        else if(operators.peek() == '-')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2-operand1);
        }
        operators.pop();
      }
      operators.pop(); //pop the (
    }
    else if(expression[i] == 'p')
    {
      operand1 = postfixexpression.peek();
      postfixexpression.pop();
      postfixexpression.push(pow(operand1, 2));
    }
    else if(expression[i] == 's')
    {
      operators.push(expression[i]);
    }
    else if((expression[i] == '*') || (expression[i] == '/'))
    {
      if(operators.isEmpty()) //if empty push the operator through
      {
        operators.push(expression[i]);
      }
      else if((operators.peek() == '*' || operators.peek() == '/' || operators.peek() == '(' || operators.peek() == 's'))//if operator is *, /, or (
      {
        while((operators.peek() != '-' && operators.peek() != '+') && operators.peek() != '(' && !operators.isEmpty())
        {
          if(operators.peek() == 's')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(sqrt(operand1));
          }
          else if(operators.peek() == '*') //While operators are of greater precendence in the op stack
          {                           //check operator and compute math of top two operands according to operator
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand1*operand2);//pop both operands and push result onto postfix stack
          }
          else if(operators.peek() == '/')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand2/operand1);
          }
          operators.pop();
        }
        operators.push(expression[i]); //push the operator onto the operators stack once while loop finishes
      }
      else if(operators.peek() == '+' || operators.peek() == '-')
      {
        operators.push(expression[i]);
      }
    }
    else if((expression[i] == '+') || (expression[i] == '-')) //If operator is plus or minus
    {
      if(operators.isEmpty()) //add to stack if empty
      {
        operators.push(expression[i]);
      }
      else
      {
        while((operators.peek() == '*' || operators.peek() == '/' || operators.peek() == '+' || operators.peek() == '-' || operators.peek() == 's') && operators.peek() != '(' && !operators.isEmpty())
        {
          if(operators.peek() == 's')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(sqrt(operand1));
          }
          else if(operators.peek() == '*') //While operators are of greater precendence in the op stack
          {                           //check operator and compute math of top two operands according to operator
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand1*operand2);//pop both operands and push result onto postfix stack
          }
          else if(operators.peek() == '/')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand2/operand1);
          }
          else if(operators.peek() == '+')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand2+operand1);
          }
          else if(operators.peek() == '-')
          {
            operand1 = postfixexpression.peek();
            postfixexpression.pop();
            operand2 = postfixexpression.peek();
            postfixexpression.pop();
            postfixexpression.push(operand2-operand1);
          }
          operators.pop();
        }
        operators.push(expression[i]); //add the plus or minus once there are no more bigger oprators on stack
      }
    }
    if(i == expression.size() - 1) //once the string has ended, compute the rest of operators
    {
      while(!operators.isEmpty()) //while operators stack is not empty
      {
        if(operators.peek() == 's')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(sqrt(operand1));
        }
        else if(operators.peek() == '*')//Check the peek operator and compute result of top two operands
        {                          //in postfix stack and push the result onto postfix stack
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand1*operand2);
        }
        else if(operators.peek() == '/')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2/operand1);
        }
        else if(operators.peek() == '+')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2+operand1);
        }
        else if(operators.peek() == '-')
        {
          operand1 = postfixexpression.peek();
          postfixexpression.pop();
          operand2 = postfixexpression.peek();
          postfixexpression.pop();
          postfixexpression.push(operand2-operand1);
        }
        operators.pop();
      }
    }
  }

  return postfixexpression.peek();
}

string calculator::getInitial()
{
  return initial;
}
