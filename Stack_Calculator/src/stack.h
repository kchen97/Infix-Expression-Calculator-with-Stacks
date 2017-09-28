/*
Korman Chen
CS 110C
Honors Project & HW6
*/

#ifndef STACK_H
#define STACK_H

const int max_stack = 51;

template <class ItemType>
class Stack
{
private:
  int top;
  int items;
  ItemType aStack[max_stack];

public:
  Stack();
  bool isEmpty();
  bool push(ItemType);
  bool pop();
  ItemType peek();
};

#endif
