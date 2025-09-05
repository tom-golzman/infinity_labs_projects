# Scheduler

## Author - Tom Golzman
This project was developed as part of an educational deep dive into system-level programming and data structures.

---

## Project Overview
The Scheduler is a cooperative task scheduler written in **C89**.  
It allows scheduling of tasks to run at specific intervals, supports recurring tasks through rescheduling, and provides controlled start/stop execution.

---

## Features
- Schedule tasks to run after a specified delay
- Support for recurring tasks (rescheduling)
- Stop, start, and clear scheduler tasks
- Safe task removal using unique identifiers (UID)
- Cooperative scheduling model: each task decides whether it should be rescheduled
- Clean, well-encapsulated C API

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Data Structures**:
  - Priority Queue (built on top of an Ordered Linked List)
  - UID Generator
  - Abstract Task Objects
- **Design Principles**:
  - Separation of Concerns
  - Encapsulation
  - Modular Makefile with debug and release modes
- Debugging and Testing: Valgrind, custom test framework

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

bin/debug/scheduler.out
bin/release/scheduler.out

```
