# Systems Programming & Data Structures - Bootcamp Projects

Welcome to my collection of C and C++ projects.  
All work in this repository was built for **educational purposes**, focusing on **low-level system programming**, **memory management**, and **data structures**.

All projects were written in **C89** or **C++98**, following strict coding standards and leveraging manual memory handling and OS-level APIs to build technical depth.

---

## Table of Contents
- [Operating Systems & Process Management](#operating-systems--process-management)
- [Memory Management & Data Structures](#memory-management--data-structures)
- [Object-Oriented Design in C++](#object-oriented-design-in-c)
- [Testing & Debugging](#testing--debugging)
- [Build Instructions](#build-instructions)

---

## Operating Systems & Process Management

### [Scheduler](./ds/scheduler)
A cooperative task scheduler built from scratch in C.  
Supports delayed execution, rescheduling logic, and dynamic task addition/removal.

- Features: time-based execution, run/stop logic, UID-based task management.
- Tech & Skills: `C89`, `POSIX time`, `UID generator`, `Priority Queue`, `Dynamic Memory`, `Unit Testing`, `Valgrind`.

---

### [Watchdog](./utils/watchdog)
A self-healing watchdog system that monitors and revives crashed processes.  
Delivered as a shared object library (`libwd.so`) with simple integration into client apps.

- Features: fault tolerance, heartbeat via signals, automatic respawn, SO library usage.
- Tech & Skills: `C89`, `fork/exec`, `POSIX signals`, `Inter-process Communication`, `Multi-Threading`, `Shared Objects`, `Makefile`.

---

## Memory Management & Data Structures

### [VSA – Variable Size Allocator](./ds/vsa)
Custom memory allocator that divides a static memory pool into variable-size blocks with constant-time allocation/free.

- Features: allocation headers, alignment, magic numbers for corruption detection.
- Tech & Skills: `C89`, `Bitwise operations`, `Memory Layout`, `Alignment`.

---

### [AVL Tree](./ds/avl)
A self-balancing binary search tree with rotation logic and height maintenance.

- Features: left/right rotations, recursive insert/delete, balance factor logic.
- Tech & Skills: `C89`, `Binary Trees`, `Balancing Algorithms`, `Recursion`.

---

### [BitTrie](./ds/bit_trie)
A binary prefix tree that stores integers using their binary representation.  
Designed for fast insert, search, and efficient memory usage.

- Features: bit-level key navigation, compact node structure.
- Tech & Skills: `C89`, `Trie Algorithms`, `Bit Manipulation`, `Recursion`.

---

### [Priority Queue](./ds/priority_queue)
A priority queue built using a doubly linked list and external comparison functions.

- Features: flexible comparison, UID match removal, stable ordering.
- Tech & Skills: `C89`, `Function Pointers`, `Data Structure Abstraction`, `Linked List`.

---

## Object-Oriented Design in C++

### [Simple String](./cpp/simple_string)
Implements a string class on top of a reusable, generic Buffer<char> with internal reference counting and safe data access patterns.

- Features: `CharProxy` pattern, copy-on-write optimization, safe indexing via proxy.
- Tech & Skills: `C++98`, `RAII`, `Operator Overloading`, `Encapsulation`, `Memory Safety`, `Buffer Management`.

---

## Testing & Debugging

All projects are accompanied by:

- Unit tests using a custom test framework  
- Color-coded PASS/FAIL output  
- Stress tests and edge case coverage  
- Leak detection via **Valgrind**  
- Manual debugging with `gdb`, debug macros, and assertions

---

## Build Instructions

Each project is fully modular with its own Makefile:

```bash
make debug         # Build with debugging
make release       # Build optimized (-O3)
make run           # Run debug version
make run_release   # Run release version

```
