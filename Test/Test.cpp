#include <iostream>
#include "MemoryManager.h"

size_t FixSize(size_t size)
{
  return (size + sizeof(intptr_t) - 1) & ~(sizeof(intptr_t) - 1);
}

int main()
{
 

  return 0;
}