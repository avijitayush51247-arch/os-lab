# LAB 2 BUG REPORT# LAB 2 BUG REPORT

---

## 1. Segmentation Fault Bug (crash.c)

### Bug:
Dereferencing NULL pointer:
```c
int *p = NULL;
*p = 42;

How I found it:
Program crashed immediately
Used gdb
bt showed crash at line with *p
Fix:
Never dereference NULL pointer
Allocate memory before use

Bug : for (int i = 0; i <= n; i++)
Problem:
Accessing arr[n] (out of bounds)
How I found it:
Used gdb breakpoints
Observed invalid array access at i = n
Fix: for (int i = 0; i < n; i++)


Memory Leak + Use After Free (leak.c, uaf.c)
Leak Bug:
Forgot to free malloc memory
Use-after-free Bug:
Accessing pointer after free()
How I found it:
Used valgrind --leak-check=full
Found "definitely lost" memory
Found "invalid read" after free
Fix:
Always free allocated memory
Do not use pointer after free
