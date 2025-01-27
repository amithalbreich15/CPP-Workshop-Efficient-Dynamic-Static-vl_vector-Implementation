### C++ Workshop: Efficient `vl_vector` Implementation

This project implements a highly efficient, flexible vector-like container in C++, named `vl_vector`. The design optimally balances between stack and heap memory usage, offering improved runtime efficiency and minimal memory overhead. This exercise is part of the Hebrew University of Jerusalem's advanced C++ programming curriculum.

---

### Project Overview

#### Objectives:
1. Create a generic container `vl_vector` with:
   - Automatic transitions between static (stack-based) and dynamic (heap-based) memory management.
   - An API similar to `std::vector` but optimized for small, frequently-used data sizes.
2. Implement advanced memory management strategies to minimize overhead and maximize performance.
3. Evaluate design choices through real-world use cases and memory benchmarks.

#### Features:
- **Dual Memory Management**: 
  - **Static Capacity (`C`)**: The initial `C` elements are stored in stack memory for fast access.
  - **Dynamic Transition**: When the size exceeds `C`, the container transitions to heap memory while preserving existing data.
  - Configurable `C` with a default value of 16.
- **Capacity Growth Policy**:
  - Stack capacity is fixed at `C`.
  - Heap capacity grows based on the formula:
    \[
    \text{capacity}(size, k) = 
    \begin{cases} 
    C & \text{if } \text{size} + k \leq C \\ 
    3 \cdot (\text{size} + k) / 2 & \text{otherwise}
    \end{cases}
    \]
  - Reduces memory reallocation frequency while maintaining linear space growth.
  
---

### `vl_vector` Class Design

#### Constructors:
1. **Default Constructor**: Initializes an empty vector with static capacity `C`.
2. **Copy Constructor**: Creates a deep copy of an existing `vl_vector`.
3. **Range-Based Constructor**: Initializes from a range of iterators.
4. **Single-Value Initialization**: Fills the vector with `count` copies of a given value.

#### Core Methods:
- **Memory Management**:
  - `push_back`: Add an element to the end.
  - `insert`: Insert one or multiple elements at a specific position.
  - `pop_back`: Remove the last element.
  - `erase`: Remove one or a range of elements.
  - `clear`: Remove all elements and reset memory allocation.
- **Access and Capacity**:
  - `size`: Returns the current number of elements.
  - `capacity`: Returns the total allocated capacity.
  - `data`: Returns a pointer to the underlying data.
  - `empty`: Checks if the vector is empty.
  - `at` and `operator[]`: Provide random access with bounds checking (`at` only).

#### Iterators:
- Supports **random-access iterators**, including:
  - `iterator` and `const_iterator`.
  - `reverse_iterator` and `const_reverse_iterator`.

#### Operator Overloads:
- **Assignment (`=`)**: Deep copy assignment.
- **Equality (`==`)** and inequality (`!=`): Compare two vectors element-wise.
- **Subscript (`[]`)**: Access elements directly.

---

### Efficiency Highlights

1. **Dynamic Transition**:
   - Automatic reallocation to heap memory if the size exceeds `C`.
   - Heap-to-stack fallback upon size reduction below `C`.

2. **Amortized Complexity**:
   - `O(1)` for `push_back`, `pop_back`, and `insert` (in most cases).
   - `O(n)` for reallocations (rare due to growth policy).

3. **Memory Optimization**:
   - Leverages stack memory for small datasets.
   - Heap growth ensures scalability for larger datasets.

---

### Example Usage

#### Initializing and Using `vl_vector`:
```cpp
#include "vl_vector.h"

vl_vector<int> vec;
vec.push_back(1);
vec.push_back(2);

for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
}
```

#### Handling Memory Transitions:
```cpp
vl_vector<int> vec;
for (int i = 0; i < 20; ++i) {
    vec.push_back(i);  // Transitions to heap when size > 16 (default `C`)
}
vec.clear();  // Resets to stack-based memory.
```

---

### Evaluation and Testing

1. **Pre-Submission Script**:
   - Ensure compliance with coding guidelines and runtime performance using the pre-submission script:
     ```bash
     ~labcc/presubmit/ex7/run <path_to_submission>
     ```

2. **Performance Benchmarks**:
   - Compare `vl_vector` performance with standard containers (e.g., `std::vector`) in small-to-large dataset scenarios.

3. **Memory Debugging**:
   - Validate memory management using `valgrind` to avoid leaks or invalid accesses.

---

### Files and Submission
Submit the following files through the course's Git system:
- `vl_vector.h`

Ensure compliance with coding and formatting standards as outlined in the course guidelines. Use `C++14` or later. 

### Additional Resources
- Example program: `highest_student_grade.cpp`.
- Presubmission tests and examples are available on the course platform.

---
