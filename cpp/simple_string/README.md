# Simple String

## Author - Tom Golzman
This project was developed as part of an educational deep dive into object-oriented programming concepts in **C++98**.

---

## Project Overview
The **Simple String** project is a custom implementation of a dynamic string class in **C++98**, designed to mimic core features of `std::string` while providing insights into **memory management, encapsulation, operator overloading, and RAII**.  

The implementation is built on top of a `Buffer<char>` class and includes additional functionality through the **CharProxy** pattern for safe indexing.

---

## Features
- Dynamic memory management for character buffers
- Copy constructor, assignment operator, and destructor (Rule of Three)
- Operator overloading:
- Encapsulation of buffer details
- RAII (Resource Acquisition Is Initialization) for memory safety
- Edge case handling (empty strings, self-assignment)

---

## Technologies and Skills
- **Language**: C++98
- **Core Concepts**:
  - Encapsulation and abstraction
  - Operator overloading
  - Rule of Three (constructor, destructor, assignment operator)
  - Proxy pattern (`CharProxy`) for safe character access
  - Memory management and buffer handling
- **Design Principles**:
  - Clear separation of interface and implementation
  - Defensive programming for boundary conditions
  - Reusable, minimalistic class design
- **Debugging & Testing**:
  - Unit tests for all operators and member functions
  - Edge case validation (empty strings, self assignment, out-of-range indexing)

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
bin/debug/simple_string.out
bin/release/simple_string.out

```
