# Variable Size Allocator (VSA)

## Author - Tom Golzman
This project was developed as part of an educational deep dive into memory management and system-level programming.

---

## Project Overview
The **Variable Size Allocator (VSA)** is a custom memory management module written in **C89**.  
It provides efficient allocation and deallocation of variable-sized memory blocks from a fixed memory pool, without relying on the system `malloc` and `free`.  
This allocator was designed to deepen understanding of low-level memory handling, alignment, and block management.

---

## Features
- Allocates memory from a pre-defined pool
- Supports variable-sized allocations
- Frees memory and coalesces adjacent free blocks to reduce fragmentation
- 8-byte alignment for all allocations
- Internal block header storing:
  - Allocation status (free/used bit)
  - Block size
  - Magic number for memory corruption detection
- No dependency on the system heap once the pool is initialized
- Lightweight and efficient

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Core Concepts**:
  - Memory pools and manual allocation
  - Pointer arithmetic
  - Data alignment
  - Block splitting and coalescing
- **Design Principles**:
  - Encapsulation of allocator API
  - Safety mechanisms in debug version (magic number checks)
  - Minimal external dependencies
- **Debugging & Testing**:
  - Custom unit tests with edge cases (small/large allocations, frees, corruption attempts)
  - Verified with Valgrind for leaks and invalid access

---

## Build Instructions
This project uses a modular Makefile with debug and release modes.

```bash
# Build debug version (with -g and assertions enabled)
make debug

# Build release version (optimized, NDEBUG defined)
make release

# Run debug version
make run

# Run release version
make run_release

---

## The compiled executables will be placed in:
bin/debug/vsa.out
bin/release/vsa.out

```
