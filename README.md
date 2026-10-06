# Expression Evaluation System Using Stack

An academic-grade, production-ready **Data Structures & Algorithms (DSA)** project in **C++17** demonstrating stack-based mathematical expression conversion, syntax validation, and numerical evaluation.

**DSA Topic #15: Expression Evaluation System Using Stack**

---

## Table of Contents
- [1. Project Overview](#1-project-overview)
- [2. Problem Statement](#2-problem-statement)
- [3. Key Features](#3-key-features)
- [4. DSA Concepts Demonstrated](#4-dsa-concepts-demonstrated)
- [5. Custom Stack Explanation & Architecture](#5-custom-stack-explanation--architecture)
- [6. Supported Operators & Precedence](#6-supported-operators--precedence)
- [7. Infix to Postfix Conversion](#7-infix-to-postfix-conversion)
- [8. Infix to Prefix Conversion](#8-infix-to-prefix-conversion)
- [9. Postfix & Prefix Evaluation](#9-postfix--prefix-evaluation)
- [10. Validation & Error Handling](#10-validation--error-handling)
- [11. Complete Working Example](#11-complete-working-example)
- [12. Complexity Analysis](#12-complexity-analysis)
- [13. Testing Results](#13-testing-results)
- [14. Compilation Instructions](#14-compilation-instructions)
- [15. Execution Instructions](#15-execution-instructions)
- [16. Project Structure](#16-project-structure)
- [17. System Limitations](#17-system-limitations)
- [18. Future Improvements](#18-future-improvements)

---

## 1. Project Overview

In computer science and compiler engineering, human-readable mathematical expressions are written in **Infix Notation** (e.g., `(25 + 5) * 3 - 10`), where binary operators are positioned between operands. However, infix expressions introduce ambiguity regarding operator precedence, associativity, and parenthetical sub-expressions. 

Compilers and execution engines resolve this by converting infix expressions into parenthesized-free intermediate forms:
- **Postfix Notation (Reverse Polish Notation - RPN)**: Operands precede their operator (e.g., `25 5 + 3 * 10 -`).
- **Prefix Notation (Polish Notation)**: Operators precede their operands (e.g., `- * + 25 5 3 10`).

This project implements an end-to-end, robust **Expression Evaluation System** in standard C++17. In strict adherence to academic DSA standards and viva requirements, the core **Stack data structure is implemented completely from scratch using a contiguous dynamic array**, without relying on the `std::stack` standard library container.

---

## 2. Problem Statement

Standard computer arithmetic units cannot evaluate multi-operator, nested parenthesized infix expressions directly in a single pass without ambiguity. The core problem requires:
1. Designing a custom, generic, dynamically-sized Stack data structure adhering to the Last-In, First-Out (LIFO) paradigm.
2. Formulating a multi-tier lexical and syntax validation pipeline to intercept and report malformed input prior to algorithmic conversion.
3. Implementing **Dijkstra's Shunting-Yard Algorithm** to translate infix strings into postfix and prefix expressions respecting operator precedence and associativity.
4. Performing numerical evaluation of the resulting postfix and prefix expressions while safely trapping mathematical exceptions (such as division and modulo by zero).
5. Exposing an interactive, educational console interface with step-by-step stack traces and evaluation history.

---

## 3. Key Features

- **Interactive 10-Option Menu**:
  1. Evaluate Infix Expression
  2. Convert Infix to Postfix
  3. Evaluate Postfix Expression
  4. Validate Expression
  5. Convert Infix to Prefix
  6. Evaluate Prefix Expression
  7. View Expression History
  8. Clear Expression History
  9. Display DSA Information
  10. Exit
- **Custom Generic Array-Based Stack (`Stack<T>`)**:
  - Encapsulated dynamic array with doubling capacity strategy.
  - Exception-safe underflow and overflow protection.
- **Educational Step-by-Step Traces**:
  - Live ASCII tabular traces detailing Token processed, Action taken, Output Buffer, and Stack State.
- **Lexical & Numeric Flexibility**:
  - Supports multi-digit integers (`125`, `5000`), floating-point decimals (`25.5`), and signed numeric operands (`-5`, `+10`).
- **Robust Error Handling**:
  - Traps division/modulo by zero, mismatched parentheses, invalid characters, and operator misplacement without program termination.

---

## 4. DSA Concepts Demonstrated

| Concept | Demonstration in Project |
| :--- | :--- |
| **Stack (LIFO)** | Custom `Stack<T>` template class managing operator precedence and operand evaluation. |
| **Dynamic Array** | Heap-allocated contiguous memory `T* data` backing the stack with amortized $O(1)$ expansion. |
| **Strings & Parsing** | Lexical token isolation, string-stream representations, and whitespace stripping. |
| **Algorithms** | Dijkstra's Shunting-Yard algorithm; right-to-left prefix evaluation; parenthesis matching. |
| **Complexity Analysis**| Rigorous $O(N)$ linear time and auxiliary space proofs. |

---

## 5. Custom Stack Explanation & Architecture

To avoid relying on `std::stack` as a black box, this system implements a generic `Stack<T>` backed by a dynamic heap array:

### Core Operations & Signatures:
- `void push(const T& value)`: Inserts an element at the top. If `size == capacity`, dynamically doubles array capacity ($O(1)$ amortized).
- `T pop()`: Removes and returns the top element. Throws `std::underflow_error` if stack is empty.
- `const T& peek() const`: Returns a constant reference to the top element without removing it. Throws `std::underflow_error` if empty.
- `bool isEmpty() const`: Returns `true` if `topIndex == -1`.
- `size_t size() const`: Returns the active element count (`topIndex + 1`).
- `void clear()`: Resets `topIndex = -1` without deallocating buffer memory.
- `void display() const`: Prints the internal stack contents from bottom to top for educational inspection.

### Memory Management & Rule of Three:
- **Destructor**: Releases heap-allocated memory (`delete[] data;`).
- **Copy Constructor & Copy Assignment Operator**: Implements deep copying of underlying array to prevent dangling pointers and double-free errors.

---

## 6. Supported Operators & Precedence

| Operator | Symbol | Precedence | Associativity | Example |
| :--- | :---: | :---: | :---: | :--- |
| **Parentheses** | `(`, `)` | Highest (Grouping) | Non-associative | `(10 + 5) * 2` $\rightarrow 30$ |
| **Exponentiation** | `^` | 3 (High) | **Right-to-Left** | `2 ^ 3 ^ 2 = 2 ^ 9 = 512` |
| **Multiplication / Division / Modulo** | `*`, `/`, `%` | 2 (Medium) | **Left-to-Right** | `100 / (5 * 2) = 10` |
| **Addition / Subtraction** | `+`, `-` | 1 (Low) | **Left-to-Right** | `10 + 5 * 2 = 20` |

---

## 7. Infix to Postfix Conversion

Implemented via **Dijkstra's Shunting-Yard Algorithm**:
1. Scan tokens from left to right.
2. If operand (multi-digit, decimal, or signed number), append directly to output.
3. If `(`, push to operator stack.
4. If `)`, pop operators to output until `(` is reached, then discard `(`.
5. If operator $op_1$:
   - While stack top $op_2$ is an operator and has higher precedence, or equal precedence with left-associativity, pop $op_2$ to output.
   - Push $op_1$ onto operator stack.
6. At end of expression, pop all remaining operators to output.

---

## 8. Infix to Prefix Conversion

Implemented via the modified reverse Shunting-Yard technique:
1. Reverse the infix token sequence.
2. Invert parentheses: `(` becomes `)`, and `)` becomes `(`.
3. Apply Shunting-Yard with adapted operator precedence:
   - For left-associative operators, pop only if top operator has strictly greater precedence ($>$).
   - For right-associative operators (`^`), pop if top operator has greater or equal precedence ($\ge$).
4. Reverse the resulting postfix expression to obtain the prefix expression.

---

## 9. Postfix & Prefix Evaluation

### Postfix Evaluation:
- Traverse tokens from **left to right**.
- If token is an operand, push to `Stack<double>`.
- If token is a binary operator:
  - Pop right operand $B = \text{pop()}$.
  - Pop left operand $A = \text{pop()}$.
  - Compute $R = A \text{ op } B$.
  - Push result $R$ back onto stack.
- Final result is the single value remaining on the stack.

### Prefix Evaluation:
- Traverse tokens from **right to left**.
- If token is an operand, push to `Stack<double>`.
- If token is a binary operator:
  - Pop left operand $A = \text{pop()}$.
  - Pop right operand $B = \text{pop()}$.
  - Compute $R = A \text{ op } B$.
  - Push result $R$ back onto stack.
- Final result is the single value remaining on the stack.

---

## 10. Validation & Error Handling

The system executes strict multi-tier syntax validation prior to evaluation:
1. **Character Whitelist Check**: Only digits `0-9`, decimal point `.`, parentheses `()`, and valid operators `+ - * / % ^` are allowed.
2. **Parentheses Matching Check**: Custom `Stack<char>` ensures every `(` has a corresponding `)` with proper nesting.
3. **Grammar & Placement Alternation**:
   - Forbids leading/trailing binary operators (`10 +`, `* 5`).
   - Forbids adjacent invalid operators (`10 + * 5`).
   - Forbids empty parentheses `()`.
   - Distinguishes unary signed literals (`-5`, `+3.14`) from binary subtraction/addition.
4. **Intentionally Unsupported Cases**:
   - `-(5 + 2)` and `+(5 + 2)` are explicitly rejected with the informative message:
     `Unary '-' before a parenthesized expression is not supported.`
5. **Runtime Safety**:
   - Zero division (`10 / 0`) and modulo zero (`25 % 0`) throw descriptive exceptions and are caught gracefully without crashing.

---

## 11. Complete Working Example

```text
Input:
(25 + 5) * 3 - 10

Postfix:
25 5 + 3 * 10 -

Prefix:
- * + 25 5 3 10

Result:
80
```

### Educational Trace Demonstration:
```text
+-------+--------------------+--------------------------+--------------------+
| Token | Action Taken       | Output Buffer            | Operator Stack     |
+-------+--------------------+--------------------------+--------------------+
| (     | Push Parenthesis   |                          | (                  |
| 25    | Output Operand     | 25                       | (                  |
| +     | Push Operator      | 25                       | ( +                |
| 5     | Output Operand     | 25 5                     | ( +                |
| )     | Pop Until '('      | 25 5 +                   |                    |
| *     | Push Operator      | 25 5 +                   | *                  |
| 3     | Output Operand     | 25 5 + 3                 | *                  |
| -     | Pop Higher Preced. | 25 5 + 3 *               | -                  |
| 10    | Output Operand     | 25 5 + 3 * 10            | -                  |
| <END> | Pop Remaining Ops  | 25 5 + 3 * 10 -          |                    |
+-------+--------------------+--------------------------+--------------------+
```

---

## 12. Complexity Analysis

| Operation | Time Complexity | Auxiliary Space Complexity | Theoretical Justification |
| :--- | :---: | :---: | :--- |
| **Stack `push()`** | $O(1)$ amortized | $O(1)$ per element | Dynamic array doubling amortizes reallocation overhead. |
| **Stack `pop()` / `peek()`**| $O(1)$ worst case | $O(1)$ | Direct array indexing using `topIndex`. |
| **Expression Validation** | $O(N)$ | $O(N)$ | Single-pass linear scan with parentheses stack. |
| **Tokenization** | $O(N)$ | $O(N)$ | Linear lexical scan isolating numbers and symbols. |
| **Infix to Postfix** | $O(N)$ | $O(N)$ | Each token pushed/popped at most once ($2N$ operations). |
| **Infix to Prefix** | $O(N)$ | $O(N)$ | String reverse + Shunting-Yard + String reverse. |
| **Postfix Evaluation** | $O(N)$ | $O(N)$ | Linear token scan with operand evaluation stack. |
| **Prefix Evaluation** | $O(N)$ | $O(N)$ | Reverse linear token scan with operand evaluation stack. |
| **History Log** | $O(1)$ / $O(H)$ | $O(H)$ | Bounded circular/linear history buffer of size $H=100$. |

---

## 13. Testing Results

The automated regression test suite contains **23 comprehensive test cases** and custom Stack unit tests.

```text
======================================================
        RUNNING DSA CORE UNIT TESTS (CUSTOM STACK)    
======================================================
[PASS] Stack initialization and empty check.
[PASS] Stack push and peek (LIFO top element verification).
[PASS] Stack dynamic array doubling / resizing.
[PASS] Stack pop in exact reverse insertion order (LIFO).
[PASS] Stack underflow exception handling.
[PASS] Stack deep copy constructor & memory independence.

======================================================
       RUNNING EXPRESSION EVALUATION TEST SUITE       
======================================================
ID  Input Expression            Expected Result       Actual Output         Status    
--------------------------------------------------------------------------------------
1   5 + 3                       8 [5 3 +]             8.000000              [PASS]    
2   10 + 5 * 2                  20 [10 5 2 * +]       20.000000             [PASS]    
3   (10 + 5) * 2                30 [10 5 + 2 *]       30.000000             [PASS]    
4   100 / (5 * 2)               10 [100 5 2 * /]      10.000000             [PASS]    
5   25 % 4                      1 [25 4 %]            1.000000              [PASS]    
6   2 ^ 3                       8 [2 3 ^]             8.000000              [PASS]    
7   2 ^ 3 ^ 2                   512 [2 3 2 ^ ^]       512.000000            [PASS]    
8   25.5 + 4.5                  30 [25.5 4.5 +]       30.000000             [PASS]    
9   125 + 50                    175 [125 50 +]        175.000000            [PASS]    
10  ((10 + 5) * (2 + 3)) / 5    15 [10 5 + 2 3 + * 5  15.000000             [PASS]    
11  (-5 * 2)                    -10 [-5 2 *]          -10.000000            [PASS]    
12  10 / 0                      Runtime Error         Caught Expected Error [PASS]    
13  25 % 0                      Runtime Error         Caught Expected Error [PASS]    
14  10 + * 5                    Syntax Error          Syntax Error Detected [PASS]    
15  (10 + 5                     Syntax Error          Syntax Error Detected [PASS]    
16  10 + 5)                     Syntax Error          Syntax Error Detected [PASS]    
17  10 + A                      Syntax Error          Syntax Error Detected [PASS]    
18  ""                          Syntax Error          Syntax Error Detected [PASS]    
19  *                           Syntax Error          Syntax Error Detected [PASS]    
20  10 +                        Syntax Error          Syntax Error Detected [PASS]    
21  -(5 + 2)                    Syntax Error          Syntax Error Detected [PASS]    
22  +(5 + 2)                    Syntax Error          Syntax Error Detected [PASS]    
23  3 * (4 + 2)                 18 [3 4 2 + *]        18.000000             [PASS]    
--------------------------------------------------------------------------------------
SUMMARY: Total: 23 | Passed: 23 | Failed: 0
======================================================
```

**Final Verification Result:** `23/23 tests passed, 0 failed`

---

## 14. Compilation Instructions

### Requirements:
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+ (optional)

### Compile Single-File Version (Fastest for College Evaluation):
```bash
g++ -std=c++17 -Wall -Wextra -O2 ExpressionEvaluationSystem.cpp -o ExpressionEvaluationSystem.exe
```

### Compile Modular Version:
```bash
g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/validation.cpp src/conversion.cpp src/evaluation.cpp src/history.cpp -o ExpressionEvaluationSystem.exe
```

### Compile Automated Test Runner:
```bash
g++ -std=c++17 -Wall -Wextra -O2 tests/test_runner.cpp src/validation.cpp src/conversion.cpp src/evaluation.cpp src/history.cpp -o test_runner.exe
```

### Compile via CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## 15. Execution Instructions

### Running the Interactive Application:
- On Windows:
  ```powershell
  .\ExpressionEvaluationSystem.exe
  ```
- On Linux / macOS:
  ```bash
  ./ExpressionEvaluationSystem
  ```

### Running the Test Suite:
- On Windows:
  ```powershell
  .\test_runner.exe
  ```
- On Linux / macOS:
  ```bash
  ./test_runner
  ```

---

## 16. Project Structure

```text
Expression-Evaluation-System/
│
├── ExpressionEvaluationSystem.cpp      # Consolidated single-file source for direct evaluation
├── CMakeLists.txt                      # Multi-target CMake build configuration
├── README.md                           # Complete project guide and documentation
├── PROJECT_REPORT.md                   # 20-section university academic report
├── .gitignore                          # Git ignore definitions for C++ artifacts
│
├── include/                            # Header declarations (Modular Architecture)
│   ├── stack.h                         # Custom array-based generic Stack class
│   ├── validation.h                    # Syntax & lexical validation declarations
│   ├── conversion.h                    # Infix to Postfix & Prefix conversion declarations
│   ├── evaluation.h                    # Postfix & Prefix numerical evaluation declarations
│   └── history.h                       # Array-based history buffer declarations
│
├── src/                                # Source implementations (Modular Architecture)
│   ├── main.cpp                        # Main interactive console menu driver
│   ├── validation.cpp                  # Syntax checking & parenthesis matching logic
│   ├── conversion.cpp                  # Shunting-Yard conversion & step trace generator
│   ├── evaluation.cpp                  # Stack-based numerical expression evaluation
│   └── history.cpp                     # History buffer implementation
│
├── tests/                              # Automated testing suite
│   ├── test_runner.cpp                 # 23-test regression and unit test driver
│   └── test_cases.txt                  # Full documentation of all 23 test scenarios
│
└── docs/                               # Detailed academic & viva documentation
    ├── pseudocode.md                   # Formal algorithmic pseudocode for all routines
    ├── flowchart.md                    # Mermaid control-flow diagrams
    ├── complexity.md                   # Mathematical Big-O time and space proofs
    └── viva_questions.md               # 25 university viva voce questions with model answers
```

---

## 17. System Limitations

1. **Unary Operators on Parenthesized Sub-Expressions**: Expressions such as `-(5 + 2)` or `+(5 + 2)` are intentionally rejected by the validation grammar. Unary signs are supported on numeric operands directly (e.g. `(-5 * 2)` or `-5 + 3`).
2. **Trigonometric & Transcendental Functions**: Mathematical functions like `sin(x)`, `cos(x)`, `log(x)`, and `sqrt(x)` are not included in the standard arithmetic operator set.
3. **Variable Symbols**: Operands must be numerical values; symbolic identifiers (e.g. `x + y`) are rejected by lexical validation.

---

## 18. Future Improvements

1. **Symbol Table & Variables**: Integrate a hash table or binary search tree to bind and evaluate variables (`x = 10`, `y = 20`, `x * y`).
2. **Unary Functions Support**: Extend tokenization to recognize standard functions (`sin`, `cos`, `tan`, `abs`, `sqrt`).
3. **Binary Expression Trees**: Generate and visualize binary expression trees from postfix notation with prefix/infix/postfix tree traversal.
4. **History Persistence**: Export session history to JSON or CSV files for academic record-keeping.
