/*
Korman Chen
CS 110C
Honors Project & HW6
*/

#include <iostream>
#include <cstring>
#include <string>
#include "stack.h"

using namespace std;
using namespace ci;

template <class ItemType>
Stack<ItemType>::Stack()
{
  top = 0;
  items = 0;
}

template <class ItemType>
bool Stack<ItemType>::isEmpty()
{
  bool isEmpty = false;
  if(items == 0)
  {
    isEmpty = true;
  }
  return isEmpty;
}

template <class ItemType>
bool Stack<ItemType>::push(ItemType entry)
{
  bool abletoPush = false;
  if(isEmpty())
  {
    aStack[top] = entry;
    items++;
    abletoPush = true;
  }
  else if(top < max_stack - 1)
  {
    top++;
    items++;
    aStack[top] = entry;
    abletoPush = true;
  }
  return abletoPush;
}

template <class ItemType>
bool Stack<ItemType>::pop()
{
  bool abletoPop = false;
  if(isEmpty())
  {
    return abletoPop;
  }
  else if(items == 1)
  {
    items --;
    abletoPop = true;
  }
  else if((items > 1) && (items < max_stack))
  {
    items--;
    top--;
    abletoPop = true;
  }
  return abletoPop;
}

template <class ItemType>
ItemType Stack<ItemType>::peek()
{
  return aStack[top];
}

template class Stack<int>;
template class Stack<float>;
template class Stack<char>;
template class Stack<string>;
