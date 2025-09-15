# Standard-Library (pSTL)
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

### Matrix  
  A 2D matrix container that supports:  
  - Construction (`default`, `rows+cols`, `rows+cols+init`, `initializer_list`, `copy`, `move`, `assignment`)  
  - Element access (`operator[]` for row access, const + non-const)  
  - Queries (`rows`, `cols`)  
  - Output (`printMatrix`)  
  - Arithmetic operators (`+`, `-`, `*` for matrix multiplication, scalar multiplication/division)  
  - Compound assignments (`+=`, `-=`, `*=`, `/=`)  
  - Element-wise operations (`cwiseMul`, `cwiseDiv`)  
  - Transformations (`transpose` in-place, `transposed` copy)  
  - Advanced operations (`determinant`, `inverse`)  

### Binary Search Tree (BST)
  An ordered container that supports:
  - Construction (`default`, `copy`, `move`, `assignment`)
  - Accessors (`getHead`, `getHeadRef`)
  - Queries (`findMin`, `findMax`, `contains`, `isEmpty`)
  - Modifiers (`insert`, `remove`, `makeEmpty`)
  - Output (`printTree` to `std::ostream`)
  - Internal utilities (`clone`, recursive helpers for `insert/remove`)

### HashMap  
  A hash-based associative container that supports:  
  - Construction (`default`, `initializer_list`, `copy`, `move`, `assignment`)  
  - Element access (`operator[]`, `at`)  
  - Capacity queries (`size`, `empty`)  
  - Modifiers (`insert`, `erase`, `reserve`, `clear`, `grow`, `swap`)  
  - Lookup (`find`)  
  - Internal utilities (linked-list chaining, rehashing on reserve/grow)  

### Graph  
  A node-based graph container that supports:  
  - Construction/Destruction (automatic cleanup of allocated nodes)  
  - Checkers (`doesNodeExist`, `isValidEdge`, `isValidWeight`, `isGraphValid`)  
  - Modifiers (`addNode`, `addEdgeDirected`, `addEdgeUndirected`, `removeNode`,  
    `removeEdgeDirected`, `removeEdgeUndirected`, `setWeightDirected`, `setWeightUndirected`)  
  - Queries (`getWeight`, `findEdge`)  
  - Traversals (`DFS`, `BFS`)  
  - Shortest Paths (`Dijkstra`, `AStar`)  
  - Persistence (`saveToFile`, `loadFromFile`)  
  - Internal utilities (`heuristic`, `reconstructPath`, adjacency via `Node<T>`)  
