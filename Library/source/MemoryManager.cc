#include "MemoryManager.h"

#include <cstdio>
#include <stdint.h>

struct MemoryBlock
{
  bool bIsFree;
  size_t Size;
  void* Address;
  MemoryBlock* Next;
};

// Memory address of the first reserved block
MemoryBlock* FirstBlock = nullptr;

/**
 * Machine word size. Depending on the architecture,
 * can be 4 or 8 bytes.
 */
using WordSize = intptr_t;

MemoryBlock* CreateBlock(size_t size)
{
  MemoryBlock* block = nullptr;
  block = (MemoryBlock*)malloc(sizeof(MemoryBlock));

  if (block == nullptr)
  {
    printf("Error: Not enough space for block creation.");
    return nullptr;
  }

  block->bIsFree = 1;
  block->Size = size;
  block->Next = nullptr;
  block->Address = malloc(size);

  if (block->Address == nullptr)
  {
    printf("Error: Not enough space for memory allocation.");
    free(block);
    return nullptr;
  }

  return block;
}

MemoryBlock* GetFreeBlock(size_t size)
{
  if (FirstBlock == nullptr)
  {
    FirstBlock = CreateBlock(size);
    return FirstBlock;
  }

  MemoryBlock* freeBlock = nullptr;
  MemoryBlock* currentBlock = FirstBlock;
  MemoryBlock* lastBlock = nullptr;

  do
  {
    if (currentBlock->bIsFree && currentBlock->Size >= size)
    {
      // Only get the block if we don't have one, or if we find one that's better suited for the size
      if (freeBlock == nullptr || (freeBlock && currentBlock->Size < freeBlock->Size))
      {
        freeBlock = currentBlock;
      }
    }

    lastBlock = currentBlock;
    currentBlock = currentBlock->Next;
  } while (lastBlock->Next != nullptr);

  if (freeBlock == nullptr)
  {
    freeBlock = CreateBlock(size);
    lastBlock->Next = freeBlock;
  }

  return freeBlock;
}

MemoryBlock* GetBlock(void* address)
{
  MemoryBlock* currentBlock = FirstBlock;
  while (currentBlock != nullptr)
  {
    if (address == currentBlock->Address)
    {
      return currentBlock;
    }

    currentBlock = currentBlock->Next;
  }

  // We haven't found a block with such address
  return nullptr;
}

size_t Align(size_t size)
{
  return (size + sizeof(WordSize) - 1) & ~(sizeof(WordSize) - 1);
}

void ResetBlock(MemoryBlock* block)
{
  if (block == nullptr)
  {
    return;
  }

  uint8_t* address = (uint8_t*)block->Address;
  for (size_t i = 0; i < block->Size; ++i)
  {
    address[i] = 0;
  }

  block->bIsFree = 1;
}

void* Reserve(size_t size)
{
  if (size == 0)
  {
    return nullptr;
  }

  size = Align(size);
  MemoryBlock* block = GetFreeBlock(size);

  if (block == nullptr)
  {
    return nullptr;
  }

  // Now the block is in use
  block->bIsFree = 0;
  return block->Address;
}

void Release(void* address)
{
  if (address == nullptr)
  {
    return;
  }

  MemoryBlock* currentBlock = GetBlock(address);

  if (currentBlock == nullptr)
  {
    return;
  }

  // Reset memory and set as free
  ResetBlock(currentBlock);
}

size_t SizeOf(void* address)
{
  MemoryBlock* currentBlock = GetBlock(address);

  if (currentBlock == nullptr)
  {
    return 0;
  }

  return currentBlock->Size;
}

void FlushMemory()
{
#if _DEBUG
  printf("\nClearing out memory...");
  PrintMemoryStatus();
#endif // _DEBUG

  MemoryBlock* currentBlock = FirstBlock;
  while (currentBlock != nullptr)
  {
    MemoryBlock* lastBlock = currentBlock;
    currentBlock = currentBlock->Next;

    free(lastBlock->Address);
    free(lastBlock);
  }
}

void PrintMemoryStatus() 
{
  printf("\n");
  uint32_t blocksInUse = 0;
  uint32_t totalBlocks = 0;

  MemoryBlock* currentBlock = FirstBlock;
  while (currentBlock != nullptr)
  {
    printf("[Block] Address: 0x%p - Size: %zd - State: %s\n", currentBlock->Address, currentBlock->Size, currentBlock->bIsFree ? "In use" : "Free");

    if (!currentBlock->bIsFree)
    {
      ++blocksInUse;
    }

    currentBlock = currentBlock->Next;
    ++totalBlocks;
  }

  printf("\nMemory blocks currently in use: %d\nTotal allocated blocks: %d\n", blocksInUse, totalBlocks);
}

void LoadFromConfig()
{
}

void LoadMemoryCache()
{
}

void SaveMemoryCache()
{
}
