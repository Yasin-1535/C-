# Algorithmic Complexity Analysis (Big-O)

This document provides a comprehensive asymptotic analysis of the Time and Space Complexity for all core operations in the **Expression Evaluation System Using Stack**.

---

## 1. Summary Complexity Matrix

| Operation / Module | Best Case Time | Average Case Time | Worst Case Time | Auxiliary Space Complexity | Dominant Factor |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Stack `push()`** | $O(1)$ | $O(1)$ amortized | $O(N)$ worst case (resizing) | $O(1)$ per element | Contiguous array storage |
| **Stack `pop()` / `peek()`** | $O(1)$ | $O(1)$ | $O(1)$ | $O(1)$ | Direct pointer/index decrement |
| **Lexical Tokenization** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Single forward scan of string |
| **Parentheses & Syntax Validation** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Array stack for matching '(' |
| **Infix to Postfix Conversion** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Each token pushed/popped at most once |
| **Postfix Expression Evaluation** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Stack evaluation over $N$ tokens |
| **Infix to Prefix Conversion** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Token reversal + Shunting-Yard |
| **Prefix Expression Evaluation** | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Right-to-left scan with operand stack |
| **History Insertion (`addEntry`)** | $O(1)$ | $O(1)$ normally | $O(H)$ when buffer full | $O(H)$ | Circular sliding window buffer ($H=100$) |
| **History Display** | $O(H)$ | $O(H)$ | $O(H)$ | $O(1)$ | Traversing history buffer up to $H$ |

*(Where $N$ represents the number of characters/tokens in the expression, and $H$ is the maximum history buffer size).*

---

## 2. In-Depth Operational Analysis

### 2.1 Custom Array-Based Stack Operations

#### Time Complexity:
- **`push(element)`**:
  - Under normal circumstances (when `topIndex < capacity - 1`), `push` simply writes to `data[++topIndex]`. This is an instantaneous $O(1)$ operation.
  - When the array is completely full, a dynamic reallocation is triggered: a new memory block of size $2 \times \text{capacity}$ is allocated and all existing elements are copied over ($O(N)$).
  - By aggregate analysis, doubling the array capacity amortizes this resizing cost across all preceding insertions. Therefore, the **amortized time complexity is $O(1)$**.
- **`pop()`**:
  - Simply reads `data[topIndex]` and decrements `topIndex--`. No reallocation or data shifting occurs. **Time Complexity: $O(1)$**.
- **`peek()` / `top()`**:
  - Direct array indexing `data[topIndex]`. **Time Complexity: $O(1)$**.
- **`isEmpty()` and `isFull()`**:
  - Comparison of integer `topIndex` with $-1$ or $\text{capacity}-1$. **Time Complexity: $O(1)$**.

#### Space Complexity:
- A dynamically allocated contiguous block of memory `T* data` of size $\text{capacity}$.
- Space complexity is $O(C)$ where $C$ is the allocated capacity ($C \ge N$). Auxiliary overhead per operation is $O(1)$.

---

### 2.2 Expression Syntax & Parentheses Validation

#### Time Complexity: $O(N)$
- The validation process consists of three sequential passes over the string of length $N$:
  1. **Lexical scan**: Validates that all characters belong to the allowable alphabet (`[0-9]`, `.`, `+`, `-`, `*`, `/`, `%`, `^`, `(`, `)`). Each character is examined exactly once: $O(N)$.
  2. **Parentheses balance scan**: Pushes each `(` onto the custom `Stack<char>` and pops on `)`. Each character causes at most one push or pop operation: $O(N)$.
  3. **Grammar state machine**: Scans tokens to verify valid alternation between operands and binary operators. Consecutive operators (e.g. `10 + * 5`) or invalid start/end tokens are caught in a single linear pass: $O(N)$.
- Total time: $O(N) + O(N) + O(N) = O(N)$.

#### Space Complexity: $O(N)$
- The parenthesis validation stack holds at most $N/2$ open parentheses in the worst case (e.g., `((((...))))`). Auxiliary space required is $O(N)$.

---

### 2.3 Tokenization

#### Time Complexity: $O(N)$
- The tokenizer maintains an index `i` that monotonically increases from $0$ to $N$.
- Inner while-loops consume contiguous digit sequences for multi-digit numbers (e.g., `125`, `5000`) and fractional digits. Because each character of the input string is inspected and advanced at most twice (lookahead and consume), the loop runs in strict linear time: $O(N)$.

#### Space Complexity: $O(N)$
- The generated `std::vector<std::string>` tokens array stores at most $N$ tokens. Each character appears in exactly one token. Auxiliary space is $O(N)$.

---

### 2.4 Infix to Postfix Conversion (Shunting-Yard)

#### Time Complexity: $O(N)$
- Let $M$ be the number of tokens ($M \le N$).
- Each token is processed once in the main loop:
  - Operands are immediately emitted to the output buffer: $O(1)$ per operand.
  - Left parenthesis `(` is pushed onto the operator stack: $O(1)$.
  - Right parenthesis `)` pops operators from the stack until matching `(` is found.
  - Operators are compared with the top of the operator stack. An operator is pushed once and popped at most once.
- **Key Amortized Observation**: Every operator token is pushed onto the stack at most once and popped at most once throughout the entire algorithm. Thus, the inner `while` loop that pops operators runs at most $M$ times across the *entire* execution of the algorithm, not $M$ times per token.
- Total time complexity is strictly bounded by $O(N)$.

#### Space Complexity: $O(N)$
- **Operator Stack**: At most $M$ operators can reside on the stack simultaneously: $O(N)$.
- **Output Token Buffer**: Contains all operands and operators formatted into Reverse Polish Notation: $O(N)$.
- Total auxiliary memory: $O(N)$.

---

### 2.5 Postfix Expression Evaluation

#### Time Complexity: $O(N)$
- Iterates through the list of $M$ space-separated tokens ($M \le N$):
  - If the token is a numeric operand: parsed with `std::strtod` in $O(L)$ time (where $L$ is the number of digits in the token) and pushed onto the `Stack<double>` in $O(1)$.
  - If the token is an operator: two operands are popped ($O(1)$) and the arithmetic operation is executed in $O(1)$ time, then the result is pushed back ($O(1)$).
- Since every token requires $O(1)$ stack actions and the sum of lengths of all numeric tokens equals the operand portion of the input, the overall time is $O(N)$.

#### Space Complexity: $O(N)$
- The operand value stack `Stack<double>` holds numbers awaiting evaluation. In the worst case (e.g., $K$ operands followed by $K-1$ operators: `10 20 30 40 + + +`), the stack grows to size $K \le N$.
- Auxiliary space complexity is $O(N)$.

---

### 2.6 Infix to Prefix Conversion & Evaluation

#### Time Complexity: $O(N)$
- Infix to Prefix consists of:
  1. Reversing the input token array: $O(N)$.
  2. Swapping `(` and `)`: $O(N)$.
  3. Modified Shunting-Yard conversion: $O(N)$.
  4. Reversing the output token array: $O(N)$.
- Sum of phases: $O(N) + O(N) + O(N) + O(N) = O(N)$.
- Prefix Evaluation:
  - Scans tokens from right to left. Each token is visited once. Operands are pushed, operators pop two operands and push the result.
  - Total time: $O(N)$.

#### Space Complexity: $O(N)$
- Reversed token vector: $O(N)$.
- Operator stack and value stack: $O(N)$.

---

### 2.7 History Management Operations

- **History insertion (`addEntry`)**:
  - $O(1)$ normally when appending to the array (`count < MAX_HISTORY`).
  - $O(H)$ when the buffer is full (`count == MAX_HISTORY`), shifting existing elements left to implement a sliding window log.
- **History display (`display`)**:
  - $O(H)$ time to format and traverse the stored history entries up to capacity $H$.
- **History space**:
  - $O(H)$ auxiliary space for the fixed array of $H=100$ entries.
