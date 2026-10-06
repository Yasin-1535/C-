# ACADEMIC PROJECT REPORT

## EXPRESSION EVALUATION SYSTEM USING STACK

---

### 1. COVER PAGE

```text
================================================================================
                    DEPARTMENT OF COMPUTER SCIENCE & ENGINEERING
                         DATA STRUCTURES & ALGORITHMS LAB
                               PROJECT REPORT

                     EXPRESSION EVALUATION SYSTEM USING STACK
                             (DSA Project Topic #15)
================================================================================

Submitted in partial fulfillment of the requirements for the degree of
Bachelor of Technology / Bachelor of Science in Computer Science & Engineering

SUBMITTED BY:
  Student Name:       [Student Name]
  Enrollment Number:  [Enrollment Number]
  Section:            [Section]
  Semester / Year:    [Semester / Year]

UNDER THE GUIDANCE OF:
  Faculty Name:       [Faculty Name]
  Designation:        [Department of Computer Science & Engineering]

INSTITUTION:
  Institution Name:   [College / University Name]
  Academic Session:   [Academic Session]
  Submission Date:    [Submission Date]
================================================================================
```

---

### 2. CERTIFICATE / DECLARATION

#### Certificate
This is to certify that the project report titled **"Expression Evaluation System Using Stack"** submitted by **[Student Name]** (Enrollment No: **[Enrollment Number]**) in partial fulfillment of the requirements for the award of the degree in Computer Science and Engineering is an authentic record of academic work carried out under my supervision and guidance.

The matter embodied in this project report has not been submitted to any other University or Institute for the award of any degree or diploma.

\
__________________________  
**[Faculty Name]**  
Department of Computer Science & Engineering  
[College / University Name]  

---

#### Declaration
I hereby declare that the project work entitled **"Expression Evaluation System Using Stack"** is an authentic record of my own work carried out as a course project for Data Structures & Algorithms. All concepts, algorithms, code implementations, and documentation have been developed by me, and due references have been cited wherever external literature was utilized.

\
__________________________  
**[Student Name]**  
Enrollment No: [Enrollment Number]  
Date: [Submission Date]  

---

### 3. ACKNOWLEDGEMENT

I express my deepest gratitude to my course instructor and project guide, **[Faculty Name]**, for invaluable advice, continuous encouragement, and constructive critique throughout the conceptualization, design, and implementation of this Data Structures and Algorithms project.

I am also thankful to the **Department of Computer Science & Engineering** and the laboratory administration of **[College / University Name]** for providing the computing resources and facilities essential to complete and rigorously test this project.

Finally, I would like to extend my thanks to my peers and family for their continuous moral support throughout this academic endeavor.

---

### 4. TABLE OF CONTENTS

1. Cover Page
2. Certificate / Declaration
3. Acknowledgement
4. Table of Contents
5. Introduction
6. Problem Statement
7. Objectives
8. Scope of the Project
9. Data Structures Used
10. Algorithms / Logic Used
11. Flowchart / System Flow
12. C++ Implementation
13. Sample Input and Output
14. Test Cases
15. Time Complexity Analysis
16. Space Complexity Analysis
17. Limitations
18. Future Enhancements
19. Conclusion
20. References

---

### 5. INTRODUCTION

The evaluation of arithmetic expressions is one of the foundational problems in computer science and compiler design. When humans write mathematical expressions such as $(25 + 5) \times 3 - 10$, they utilize **Infix Notation**, wherein binary operators are placed between their operands. While intuitively understood by humans through implicit knowledge of operator precedence and parenthesis nesting, computers and microprocessors cannot naturally evaluate infix expressions in a single sequential pass.

Compilers and execution engines resolve this challenge by converting infix expressions into **Postfix Notation (Reverse Polish Notation)** or **Prefix Notation (Polish Notation)**. In postfix notation, operators appear strictly after the operands they govern, eliminating the need for parentheses and precedence rules.

This academic project presents a comprehensive, production-quality, console-based **Expression Evaluation System** in C++17. A central feature of this system is that it does not hide algorithmic logic behind standard library containers like `std::stack`; instead, it provides a transparent, custom **Array-Based Generic Stack** implementation, directly exhibiting the mechanics of contiguous memory allocation, pointer indexing, dynamic buffer resizing, and Last-In First-Out (LIFO) access patterns.

---

### 6. PROBLEM STATEMENT

Direct evaluation of human-readable mathematical expressions involves:
1. Handling arbitrary multi-digit integers and decimal numbers.
2. Managing conflicting operator precedence ($^$ over $\times, /, \%$ over $+,-$).
3. Preserving operator associativity (specifically right-to-left for exponentiation, left-to-right for others).
4. Enforcing parenthetical groupings that override default precedence.
5. Detecting and mitigating syntactic errors (e.g., mismatched parentheses, consecutive operators) and numerical anomalies (e.g., division by zero) without crashing.

The objective is to design, implement, and validate a robust C++ software system that solves these challenges using classical stack-based data structures and algorithms.

---

### 7. OBJECTIVES

1. **Foundational DSA Demonstration:** Implement a custom, array-based generic stack class (`Stack<T>`) supporting `push`, `pop`, `peek`, `isEmpty`, `isFull`, and `display`.
2. **Lexical Tokenization:** Build a tokenizer capable of distinguishing multi-digit numbers, floating-point literals, unary plus/minus signs, binary operators, and parentheses.
3. **Syntactic & Structural Validation:** Perform three-tier validation (lexical, parenthetical balancing, and grammatical state-machine) before parsing.
4. **Shunting-Yard Conversion:** Implement Edsger Dijkstra's Shunting-Yard algorithm to transform infix expressions into space-separated postfix notation.
5. **Expression Evaluation:** Evaluate postfix and prefix notations using stack-based numerical operand aggregation with robust zero-division exception handling.
6. **Advanced Prefix Feature:** Support dual-conversion to Polish (prefix) notation and right-to-left prefix evaluation.
7. **Educational Visibility:** Provide optional step-by-step tracing displaying token actions and stack states at every execution step.
8. **History Auditing:** Implement an array-based history log recording expression forms, results, and timestamps.

---

### 8. SCOPE OF THE PROJECT

- **Supported Operators:**
  - Addition (`+`), Subtraction (`-`) — Precedence 1, Left-to-Right Associative.
  - Multiplication (`*`), Division (`/`), Modulo (`%`) — Precedence 2, Left-to-Right Associative.
  - Exponentiation (`^`) — Precedence 3, Right-to-Left Associative.
- **Grouping:** Parentheses `(` and `)` with arbitrary nesting levels.
- **Operands:** Multi-digit integers (e.g., `125`, `5000`), floating-point values (e.g., `25.5`), and unary signed operands (e.g., `(-5 * 2)`).
- **Environment:** Pure ISO C++17 console application with zero external dependencies, compatible across Windows (MinGW/MSVC) and Linux/macOS (GCC/Clang).

---

### 9. DATA STRUCTURES USED

#### 9.1 Custom Array-Based Stack (`Stack<T>`)
- **Underlying Container:** Heap-allocated contiguous memory buffer `T* data`.
- **Index Management:** Integer `topIndex` initialized to `-1` (indicating empty).
- **Growth Strategy:** Capacity doubling whenever `topIndex == capacity - 1`.
- **Key Operations:**
  - `push(const T& element)`: $O(1)$ amortized insertion at `++topIndex`.
  - `pop()`: $O(1)$ removal and return of `data[topIndex--]`.
  - `peek()` / `top()`: $O(1)$ inspection of `data[topIndex]`.
  - `isEmpty()` / `isFull()`: $O(1)$ state queries.

#### 9.2 Fixed Array History Buffer (`ExpressionHistory`)
- **Structure:** Array of 100 `HistoryEntry` structs.
- **Behavior:** Sliding window circular replacement maintaining the most recent 100 evaluations.

---

### 10. ALGORITHMS / LOGIC USED

#### 10.1 Parentheses Balancing (Stack Algorithm)
A stack of characters is maintained. Every `(` pushes onto the stack. Every `)` pops the top. If `)` is encountered on an empty stack or if the stack is non-empty at string termination, unbalanced parentheses are reported.

#### 10.2 Infix to Postfix Conversion (Dijkstra's Shunting-Yard)
- Operands are directly routed to the output buffer.
- Open parentheses `(` are pushed onto the operator stack.
- Closed parentheses `)` pop and output operators until `(` is reached and discarded.
- Operators pop operators from the stack that have greater precedence, or equal precedence with left-associativity. Then the current operator is pushed.
- Remaining stack operators are flushed to the output.

#### 10.3 Postfix Evaluation
Tokens are scanned from left to right. Operands are pushed onto a numerical stack. Operators pop the top two values ($op_2$ and $op_1$), compute $op_1 \star op_2$, and push the result back.

#### 10.4 Infix to Prefix Conversion (Token Reversal)
The expression token sequence is reversed, parentheses are inverted (`(` $\leftrightarrow$ `)`), a modified Shunting-Yard is executed with inverted operator associativity conditions, and the resulting token stream is reversed.

---

### 11. FLOWCHART / SYSTEM FLOW

```text
[START]
   │
[Display Main Menu (Options 1 - 10)]
   │
[Read User Selection & Sanitize Input]
   │
   ├─► [Option 1: Evaluate Infix] ──► [Validate Syntax] ──► [Infix->Postfix] ──► [Eval Postfix] ──► [Display & Log]
   ├─► [Option 2: Infix to Postfix] ─► [Validate Syntax] ──► [Generate Postfix] ──► [Optional Step Trace]
   ├─► [Option 3: Evaluate Postfix] ─► [Validate Tokens] ──► [Stack Eval] ──────► [Display & Log]
   ├─► [Option 4: Validate Syntax] ──► [Lexical & Paren Check] ──► [Display Diagnostic Report]
   ├─► [Option 5: Infix to Prefix] ──► [Validate Syntax] ──► [Reversed Shunting-Yard] ──► [Display Prefix]
   ├─► [Option 6: Evaluate Prefix] ──► [Validate Tokens] ──► [Right-to-Left Stack Eval] ──► [Display Result]
   ├─► [Option 7: View History] ─────► [Render History Buffer Table]
   ├─► [Option 8: Clear History] ────► [Reset History Count to 0]
   ├─► [Option 9: DSA Overview] ─────► [Print Theoretical Information]
   └─► [Option 10: Exit] ────────────► [Graceful Termination]
```

---

### 12. C++ IMPLEMENTATION

The application is structured both as a modular multi-file architecture (`include/` and `src/`) and as a consolidated single-file distribution (`ExpressionEvaluationSystem.cpp`).

Core snippets demonstrating the array-based stack:

```cpp
template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    explicit Stack(int initialCapacity = 100)
        : topIndex(-1), capacity(initialCapacity) {
        data = new T[capacity];
    }
    ~Stack() { delete[] data; }

    void push(const T& val) {
        if (topIndex == capacity - 1) resize();
        data[++topIndex] = val;
    }

    T pop() {
        if (isEmpty()) throw std::underflow_error("Stack Underflow");
        return data[topIndex--];
    }

    const T& peek() const {
        if (isEmpty()) throw std::underflow_error("Stack Underflow");
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};
```

---

### 13. SAMPLE INPUT AND OUTPUT

#### Demonstration 1: Standard Academic Expression
- **Input Expression:** `(25 + 5) * 3 - 10`
- **Output:**
  ```text
  ================ RESULT ================
  Infix Expression:   (25 + 5) * 3 - 10
  Postfix Expression: 25 5 + 3 * 10 -
  Prefix Expression:  - * + 25 5 3 10
  Calculated Result:  80
  ========================================
  ```

#### Demonstration 2: Step-by-Step Educational Conversion Trace
```text
+-----+----------+------------------------------------------+------------------------------+--------------------+
| No. | Token    | Action                                   | Output Buffer                | Stack (Bottom->Top)|
+-----+----------+------------------------------------------+------------------------------+--------------------+
| 1   | (        | Push '(' to operator stack               | (empty)                      | (                  |
| 2   | 25       | Send operand '25' to output              | 25                           | (                  |
| 3   | +        | Push operator '+'                        | 25                           | ( +                |
| 4   | 5        | Send operand '5' to output               | 25 5                         | ( +                |
| 5   | )        | Pop operators until '(' and discard '('  | 25 5 +                       | EMPTY              |
| 6   | *        | Push operator '*'                        | 25 5 +                       | *                  |
| 7   | 3        | Send operand '3' to output               | 25 5 + 3                     | *                  |
| 8   | -        | Push operator '-'                        | 25 5 + 3 *                   | -                  |
| 9   | 10       | Send operand '10' to output              | 25 5 + 3 * 10                | -                  |
| 10  | END      | Pop all remaining operators from stack   | 25 5 + 3 * 10 -              | EMPTY              |
+-----+----------+------------------------------------------+------------------------------+--------------------+
```

#### Demonstration 3: Step-by-Step Educational Evaluation Trace
```text
+-----+----------+--------------------------------------------------------------+--------------------+
| No. | Token    | Action Performed                                             | Stack (Bottom->Top)|
+-----+----------+--------------------------------------------------------------+--------------------+
| 1   | 25       | Push operand 25                                              | 25                 |
| 2   | 5        | Push operand 5                                               | 25 5               |
| 3   | +        | Pop 5, 25 -> Compute 25 + 5 = 30 -> Push 30                  | 30                 |
| 4   | 3        | Push operand 3                                               | 30 3               |
| 5   | *        | Pop 3, 30 -> Compute 30 * 3 = 90 -> Push 90                  | 90                 |
| 6   | 10       | Push operand 10                                              | 90 10              |
| 7   | -        | Pop 10, 90 -> Compute 90 - 10 = 80 -> Push 80                | 80                 |
+-----+----------+--------------------------------------------------------------+--------------------+
```

---

### 14. TEST CASES

| ID | Category | Input Expression | Expected Result | Actual Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| 1 | Addition | `5 + 3` | `8` | `8` | **PASS** |
| 2 | Precedence | `10 + 5 * 2` | `20` | `20` | **PASS** |
| 3 | Parentheses | `(10 + 5) * 2` | `30` | `30` | **PASS** |
| 4 | Division Group | `100 / (5 * 2)` | `10` | `10` | **PASS** |
| 5 | Modulo | `25 % 4` | `1` | `1` | **PASS** |
| 6 | Power | `2 ^ 3` | `8` | `8` | **PASS** |
| 7 | Zero Division | `10 / 0` | Division by zero error | Division by zero error | **PASS** |
| 8 | Consecutive Op | `10 + * 5` | Syntax Error | Syntax Error | **PASS** |
| 9 | Unclosed Paren | `(10 + 5` | Syntax Error | Syntax Error | **PASS** |
| 10 | Grouping Paren | `10 + (5 * 2)` | `20` | `20` | **PASS** |
| 11 | Multi-Digit | `125 + 50` | `175` | `175` | **PASS** |
| 12 | Large Division | `5000 / 25` | `200` | `200` | **PASS** |
| 13 | Right Assoc | `2 ^ 3 ^ 2` | `512` | `512` | **PASS** |
| 14 | Decimal Numbers| `25.5 + 4.5` | `30` | `30` | **PASS** |
| 15 | Nested Parens | `((10 + 5) * (2 + 3)) / 5` | `15` | `15` | **PASS** |
| 16 | Zero Modulo | `25 % 0` | Modulo by zero error | Modulo by zero error | **PASS** |
| 17 | Invalid Char | `10 + A` | Syntax Error | Syntax Error | **PASS** |
| 18 | Empty String | `""` | Syntax Error | Syntax Error | **PASS** |
| 19 | Single Op | `*` | Syntax Error | Syntax Error | **PASS** |
| 20 | Trailing Op | `10 +` | Syntax Error | Syntax Error | **PASS** |
| 21 | Unary Minus Parens | `-(5 + 2)` | Unary '-' before parens rejected | Unary '-' before parens rejected | **PASS** |
| 22 | Unary Plus Parens | `+(5 + 2)` | Unary '+' before parens rejected | Unary '+' before parens rejected | **PASS** |
| 23 | Prefix Eval | `3 * (4 + 2)` | `18` | `18` | **PASS** |

---

### 15. TIME COMPLEXITY ANALYSIS

Let $N$ denote the total length of the mathematical expression (character / token count), and $H=100$ denote the maximum history buffer capacity.

1. **Stack Operations:**
   - `push()`: $O(1)$ amortized time, $O(N)$ worst case when dynamic array resizing/doubling occurs.
   - `pop()`: $O(1)$ deterministic constant time.
   - `peek()`: $O(1)$ deterministic constant time.
2. **Expression Validation:**
   - Single linear scan for lexical validation: $O(N)$.
   - Single linear scan for parenthesis matching stack: $O(N)$.
   - Single pass grammar checker: $O(N)$.
   - Overall validation time: $O(N)$.
3. **Infix to Postfix Conversion (Shunting-Yard):**
   - Each token is examined once. Each operator is pushed onto the stack at most once and popped at most once throughout the complete algorithm. Total time: $O(N)$.
4. **Postfix and Prefix Evaluation:**
   - Every token triggers exactly $O(1)$ arithmetic and stack manipulations. Overall time: $O(N)$.
5. **History Operations:**
   - History insertion: $O(1)$ normally when appending; $O(H)$ when the buffer is full ($H=100$) to perform the sliding-window shift.
   - History display: $O(H)$ to traverse and render stored entries.

---

### 16. SPACE COMPLEXITY ANALYSIS

1. **Stack Auxiliary Memory:**
   - In the worst case (e.g., heavily nested parentheses `((((...))))` or sequentially stacked operators), the operator stack stores at most $N$ elements. Space: $O(N)$.
2. **Token Buffering:**
   - Storing extracted token vectors and output strings consumes proportional space: $O(N)$.
3. **History Storage:**
   - Fixed array of $H=100$ `HistoryEntry` records: $O(H)$ auxiliary space.
4. **Total Auxiliary Space:**
   - The total space complexity across all stages is strictly bounded by $O(N) + O(H) = O(N)$.

---

### 17. LIMITATIONS

1. **No Symbolic Variables:** Operands must be provided as explicit numerical constants; algebraic symbols (such as $x, y$) cannot be stored or resolved.
2. **Standard Arithmetic Operators Only:** Functions such as `sin`, `cos`, `log`, and `sqrt` are not natively parsed.
3. **Text-Based Console Interface:** While ideal for educational demonstration and grading, it does not provide graphical expression tree rendering.

---

### 18. FUTURE ENHANCEMENTS

1. **Symbolic Variable Support:** Introduce a Symbol Table (hash table or binary search tree) allowing assignments like `x = 25; y = 5; (x + y) * 3`.
2. **Scientific Function Integration:** Expand the tokenizer to parse multi-character unary functions (`sin`, `cos`, `tan`, `sqrt`, `abs`).
3. **Binary Expression Tree (BET):** Construct and render an ASCII or graphical binary expression tree displaying syntax hierarchy.
4. **File I/O Batch Processing:** Allow reading expressions from an input file and exporting detailed evaluation traces to a CSV/JSON report.

---

### 19. CONCLUSION

The **Expression Evaluation System Using Stack** successfully fulfills all academic requirements of DSA Topic #15. By avoiding opaque standard container wrappers and constructing a transparent, array-based generic stack, the project vividly illustrates:
- How the LIFO principle enables deterministic operator precedence and parenthesis management.
- How contiguous memory arrays are managed dynamically with bounds checking.
- How complex expressions with multi-digit integers, decimals, and right-associative exponentiation can be evaluated reliably with $O(N)$ computational efficiency.

All automated and manual unit tests confirm 100% functional correctness, robust error resistance, and zero program crashes on malformed inputs.

---

### 20. REFERENCES

1. Dijkstra, E. W. (1961). *Algol 60 translation: An Algol 60 translator for the X1 and Making a Translator for ALGOL 60*. Mathematisch Centrum, Amsterdam.
2. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to Algorithms* (4th ed.). MIT Press. (Chapter 10: Elementary Data Structures).
3. Stroustrup, B. (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley Professional.
4. Knuth, D. E. (1997). *The Art of Computer Programming, Volume 1: Fundamental Algorithms* (3rd ed.). Addison-Wesley.
