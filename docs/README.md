# 📚 Emulator Documentation Index

This directory contains all official documentation for the Intel 8080 Space Invaders Emulator. The documents covver the subsystems, architectural concept, or development strategy used across the project.

---

## Architecture & System Overview

- [`architecture.md`](architecture.md)  
  Describes a high-level overview of the architecture of the emulator.  It includes the component and module relationships across the GUI amd a Model, View, Controller implementation.

- [`integration.md`](integration.md)  
  Highlights the integration strategy we implemented, and how we plammed our cross-module linking plans, and step-by-step validation roadmap.

---

## Model Layer (Core Emulation)

- [`cpu_module.md`](cpu_module.md)  
  Describes the inner workings of the Intel 8080 CPU internals, its instruction decoding, execution, and register model.

- [`memory_module.md`](memory_module.md)  
  Detail behind how the virtual memory was layed out, memory protections, and RAM/ROM handling.

- [`romloader_module.md`](romloader_module.md)  
  Describes the approach we implementsed with loading the binary file, ROM mapping rules, and integrity constraints.

- [`io_ports.md`](io_ports.md)  
  Describes IN/OUT port behaviors, including shift register emulation and port mapping for input/output.

- [`video_module.md`](video_module.md)  
  Framebuffer representation, video memory range, and graphical interpretation logic.

- [`display_renderer.md`](display_renderer.md)  
  Describes how the memory-mapped video buffer is transformed into visual output by the display system.

---

## Controller Layer (MVC Logic)

- [`controller.md`](controller.md)  
  Describes the MVC controller's role in event handling, input processing, glue logic, and coordination between GUI/View and Model layers.

---

## 🧪 Development & Testing

- [`testing.md`](testing.md)  
  Describes the project’s testing structure, test categories (unit, functional, integration), and `test_utils.hpp` usage.

- [`dev_notes.md`](dev_notes.md)  
  Ongoing developer notes, planning thoughts, rough ideas, progress and implementation details in progress.

---

##  ROMs & Compatibility

- [`rom_compatibility.md`](rom_compatibility.md)  
  Information on supported ROMs, quirks, known limitations, and expected memory mappings or test cases.

---

##  Index

- [`README.md`](README.md)  
  This file is the table of contents for the documentation of our Intel 8080 Emulator that runs Space Invaders.
