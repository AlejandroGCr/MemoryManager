#include <iostream>
#include "MemoryManager.h"

int main()
{
  int* one = (int*)Reserve(sizeof(int) * 68);

  Release(one);

  PrintMemoryStatus();

  return 0;
}