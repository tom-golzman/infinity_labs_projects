# AVL Tree

## Author - Tom Golzman
This project was developed as part of an educational deep dive into data structures and algorithms.

---

## Project Overview
The **AVL Tree** is a self-balancing binary search tree implemented in **C89**.  
It maintains the **height-balance property** (the height difference between left and right subtrees is at most 1), ensuring that all operations—search, insertion, and removal—run in **O(log n)** time.  

This project was designed to strengthen understanding of tree-based data structures, rotations, and balancing techniques.

---

## Features
- Insert elements while maintaining AVL balance
- Remove elements with automatic tree rebalancing
- Search for elements in logarithmic time
- In-order traversal (sorted output)
- Encapsulated and generic C API using function pointers for comparison
- Efficient height calculation and balancing logic

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Core Concepts**:
  - Binary Search Trees (BST)
  - Self-balancing trees
  - Rotations (single & double: left, right, left-right, right-left)
  - Recursive and iterative algorithms
- **Design Principles**:
  - Encapsulation through opaque data structures
  - Separation of balancing logic into a dedicated static function
  - Reusability via user-defined comparison functions
- **Debugging & Testing**:
  - Custom unit tests covering edge cases (deep trees, skewed trees, repeated inserts/removes)
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
bin/debug/avl.out
bin/release/avl.out

```
