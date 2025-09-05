# Bitwise Trie (BitTrie)

## Author - Tom Golzman
This project was developed as part of an educational deep dive into data structures, memory management, and system-level programming.

---

## Project Overview
The **Bitwise Trie (BitTrie)** is a prefix tree implementation optimized for storing and querying integer keys at the **bit level**.  
Instead of branching on entire characters or numbers, the trie decomposes each key into its binary representation and stores it bit by bit.  

This approach provides predictable **O(k)** complexity for operations, where *k* is the number of bits in the key (e.g., 32 for a 32-bit integer).

---

## Features
- Insert integer keys into the trie (bit by bit)
- Search for exact integer keys
- Remove keys safely and reclaim memory
- Efficient memory usage through binary branching (0/1)
- Scalable for large sets of integer keys
- Clean, encapsulated API in C

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Core Concepts**:
  - Tries and prefix trees
  - Bit-level manipulation
  - Efficient lookup using binary paths
  - Dynamic memory allocation
- **Design Principles**:
  - Encapsulation with opaque types
  - Generic, reusable API
  - Emphasis on predictable time complexity
- **Debugging & Testing**:
  - Unit tests for insertion, search, and removal
  - Edge cases: duplicate keys, non-existent keys, empty trie
  - Verified with Valgrind for memory safety

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
bin/debug/bit_trie.out
bin/release/bit_trie.out

```
