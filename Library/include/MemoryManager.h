#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <cstdlib>

/* TODO: 
    - Map to different allocators based on the operating system
    - Write premake to upload to github
    - Write caching functions
    - Documentation
    - Write Test
    - Research ways of getting rid of includes
    - Fit algorithms
    - Explicit Free list
*/

// Reserves a block of memory of the specified size
void* Reserve(size_t size);

void Release(void* address);

size_t SizeOf(void* address);

void FlushMemory();

void PrintMemoryStatus();

void LoadFromConfig();

void LoadMemoryCache();

void SaveMemoryCache();

#endif // MEMORYMANAGER_H