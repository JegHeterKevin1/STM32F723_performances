# STM32 Project: Resolving Cache Issues with DMA

This project demonstrates and resolves a data cache (D-Cache) issue encountered when using DMA (Direct Memory Access) on an STM32 microcontroller. The issue involves data inconsistency between the cache and the RAM, which results in incorrect values being read from memory due to cached data not being properly updated.

## Problem Description

### Context

When the data cache (D-Cache) is enabled, it temporarily stores values from memory to speed up access times. However, when DMA or other processes modify the memory directly, the D-Cache may not reflect the updated memory content, leading to inconsistencies and incorrect reads.

### Issue Encountered

In this project, reading data from RAM returned incorrect values because the D-Cache either did not have enough time to update the memory or was unaware that memory had been modified externally. This led to an inconsistency between cached data and the actual RAM content.

### Implemented Solution

To resolve this issue, we used the following functions to clean and invalidate the D-Cache:

- **Clean**: Writes data from the cache to the memory, ensuring that memory contains the latest data.
- **Invalidate**: Clears the cache so that subsequent reads fetch data directly from memory.

```c
// Example code to resolve cache inconsistency
SCB_CleanDCache_by_Addr((uint32_t*)srcBuffer, sizeof(srcBuffer)); // Clean the cache
SCB_InvalidateDCache_by_Addr((uint32_t*)destBuffer, sizeof(destBuffer)); // Invalidate the cache
```

---

This `README.md` outlines the purpose of the branch-specific project, focusing on cache management issues when using DMA with STM32 microcontrollers. Let me know if you need any additional information or modifications!

