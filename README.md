# Standard-Library
Custom C++ Standard Template Library (STL) implementation created for learning purposes. This project re-implements core STL components and other similar data structures, algorithms, containers, iterators, etc. from scratch to better understand their design, complexity, and underlying mechanisms.

>

## Note on Coding Style and Testing

Coding style and conventions may vary across files, since I am experimenting with different approaches as I learn.  

All **production code** in this repository (e.g., `HashMap` and other containers) is written by me.  
The only exception is the **testing code in `main.c`** and this `README`, which may include snippets generated with the help of ChatGPT and is used only for simple testing and demonstration purposes.

## Currently Implemented

### Vector  
  A vector-like container that supports:
  - Construction (default, size, size + init value, copy, move)
  - Element access (`at`, `operator[]`, `front`, `back`)
  - Iterator support (`begin`, `end`) using raw pointers 
  - Capacity queries (`size`, `capacity`, `empty`)
  - Modifiers (`push_back`, `insert_at`, `pop_back`, `reserve`, `clear`)
  - Search (`find`)
