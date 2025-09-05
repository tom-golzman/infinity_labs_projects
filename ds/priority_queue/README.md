# Priority Queue

## Author - Tom Golzman
This project was developed as part of an educational deep dive into data structures and algorithms.

---

## Project Overview
The **Priority Queue (PQ)** is a generic data structure implemented in **C89**, designed to manage elements based on their priority rather than insertion order.  
It is built on top of an **Ordered Linked List (OLL)**, ensuring that dequeue operations always return the element with the highest priority.  

The implementation is generic, using user-defined comparison and match functions to determine priority and support custom data types.

---

## Features
- Enqueue elements with a given priority
- Always dequeue the highest-priority element
- Peek at the top-priority element without removing it
- Clear all elements efficiently
- Erase elements based on user-defined match criteria
- Generic API with function pointers for flexibility
- Clean separation between interface and implementation

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Core Concepts**:
  - Ordered Linked List (as underlying container)
  - Priority-based ordering
  - Encapsulation of generic data structures
  - Function pointers for comparison and match operations
- **Design Principles**:
  - Reusable and modular C API
  - Separation of interface (`priority_queue.h`) and implementation (`priority_queue.c`)
  - Defensive programming (handling NULL inputs, empty queues)
- **Debugging & Testing**:
  - Unit tests covering enqueue, dequeue, peek, erase, clear, and edge cases
  - Verified with Valgrind for memory leaks and invalid access

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
bin/debug/priority_queue.out
bin/release/priority_queue.out

```
