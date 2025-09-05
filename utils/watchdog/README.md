# Watchdog

## Author - Tom Golzman
This project was developed as part of an educational deep dive into system-level programming and process management.

---

## Project Overview
The **Watchdog** is a fault-tolerance system written in **C89**.  
It ensures that a critical client process remains alive by supervising it with a dedicated watchdog process.  
If the client stops responding, the watchdog automatically restarts it, guaranteeing continuity and reliability.

The client integrates with the watchdog by calling a single function (`MakeMeImmortal()`), which spawns and configures the watchdog process in the background.

---

## Features
- Continuous monitoring of a client process by a watchdog
- Automatic restart of the client if it becomes unresponsive
- Cooperative heartbeat mechanism using signals
- Configurable runtime parameters (interval, max misses, PID) passed via `argv`
- No shared memory or environment variables required
- Delivered as a client library (`.so`) for easy integration
- Built on top of the **Scheduler** project for periodic tasks

---

## Technologies and Skills
- **Language**: ANSI C (C89)
- **Core Components**:
  - Scheduler (for periodic monitoring tasks)
  - Task abstraction
  - UID Generator
  - Doubly Linked List / Ordered Linked List (via dependencies)
- **System Programming Concepts**:
  - Process creation and management (`fork`, `exec`)
  - Inter-process communication via signals
  - Robustness without shared memory
- **Design Principles**:
  - Encapsulation of client API (single exposed function)
  - Clear separation between client and watchdog responsibilities
  - Modular Makefile with debug and release builds
- **Debugging & Testing**:
  - Custom test framework
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
bin/debug/watchdog.out
bin/release/watchdog.out

```
