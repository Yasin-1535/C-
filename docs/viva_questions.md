# Viva Voce Examination Preparation Guide

This document contains **25 comprehensive Viva Voce Questions & Answers** designed specifically for university examiners, external evaluators, and college viva presentations on **Expression Evaluation System Using Stack**.

---

### Q1. What is a Stack data structure?
**Answer:**  
A stack is a linear data structure that follows the **LIFO (Last In, First Out)** principle. In a stack, insertion (called `push`) and deletion (called `pop`) of elements can take place only from one designated end, termed the **top** of the stack. The element that is inserted last is always the first one to be removed.

---

### Q2. Why did you choose a Stack for expression conversion and evaluation?
**Answer:**  
Mathematical expressions involve operator precedence and nested parentheses where inner or higher-priority operations must be resolved before outer or lower-priority ones. A stack is naturally suited for this because:
1. It temporarily suspends operators until their operands are ready.
2. It reverses the operational order so that the highest precedence operator (pushed last) is popped and evaluated first.
3. In postfix evaluation, the two most recently calculated operands are always at the top of the stack, ready to be consumed immediately when an operator is encountered.

---

### Q3. What is LIFO and how does your project demonstrate it?
**Answer:**  
LIFO stands for **Last In, First Out**. In our project:
- When parsing `(10 + 5) * 2`, the addition operator `+` is encountered inside the parentheses and pushed onto the stack.
- The closing parenthesis `)` forces the stack to pop the most recently pushed operator (`+`) before the multiplication operator `*` is applied.
- During postfix evaluation of `10 5 +`, `10` is pushed, then `5` is pushed. The `+` operator pops `5` (last in) first, then `10`.

---

### Q4. Why did you implement a custom Stack using an Array instead of using `std::stack`?
**Answer:**  
Using `std::stack` hides internal memory layout and algorithmic details as a black box. Our academic requirement is to visibly demonstrate foundational DSA principles:
1. **Contiguous array memory**: How elements are stored sequentially in RAM.
2. **Top index manipulation**: How an integer pointer/index `topIndex` tracks the active boundary.
3. **Array resizing**: Demonstrates dynamic array growth (doubling strategy) to achieve amortized $O(1)$ push time.
4. **Bounds and error checking**: Explicitly writing stack overflow and underflow routines.

---

### Q5. What is Infix notation?
**Answer:**  
Infix notation is the standard mathematical notation where the operator is written **between** its two operands (e.g., `A + B` or `10 * 5`). While intuitive for human reading, infix notation requires explicit rules of precedence and parentheses to eliminate ambiguity during automated computer evaluation.

---

### Q6. What is Postfix notation (Reverse Polish Notation)?
**Answer:**  
Postfix notation places the operator **after** its operands (e.g., `A B +` or `10 5 *`). In postfix notation, the order of evaluation is strictly dictated by the position of operators from left to right; parentheses and operator precedence tables are never required.

---

### Q7. What is Prefix notation (Polish Notation)?
**Answer:**  
Prefix notation places the operator **before** its operands (e.g., `+ A B` or `* 10 5`). Similar to postfix, prefix notation requires no parentheses. It is evaluated by scanning tokens from right to left using a stack.

---

### Q8. Why do computers prefer Postfix notation over Infix notation?
**Answer:**  
Evaluating an infix expression directly requires multiple passes over the string or recursive back-tracking to look ahead for higher-precedence operators and resolve nested parentheses. In contrast, a postfix expression can be evaluated in a **single linear pass ($O(N)$)** using a simple operand stack without any parenthesis tracking or precedence comparisons.

---

### Q9. How does operator precedence work in your system?
**Answer:**  
We define a priority hierarchy using numeric ranks:
- **Priority 3 (Highest):** Exponentiation `^`
- **Priority 2 (Medium):** Multiplication `*`, Division `/`, Modulo `%`
- **Priority 1 (Lowest):** Addition `+`, Subtraction `-`
- Parentheses `(` have highest grouping priority during scanning, but are assigned priority $0$ inside the stack so other operators can be placed on top of them.

When an incoming operator has higher precedence than the operator on top of the stack, it is pushed. If it has lower or equal precedence, the stack top operator is popped first.

---

### Q10. What is operator associativity and how do you handle exponentiation?
**Answer:**  
Associativity determines the order in which operators of the same precedence are evaluated:
- **Left-to-Right Associativity:** Used for `+`, `-`, `*`, `/`, `%`. If two operators of the same priority meet, the earlier (left) operator is popped and executed first.
- **Right-to-Left Associativity:** Exponentiation `^` is right-associative in mathematics ($2^{3^2} = 2^{(3^2)} = 2^9 = 512$, not $(2^3)^2 = 8^2 = 64$).
- In our code, when incoming operator is `^` and stack top is also `^`, we do **not** pop the stack top operator; we push the new `^` on top of it.

---

### Q11. What is the Time Complexity of Infix to Postfix conversion?
**Answer:**  
The Time Complexity is **$O(N)$**, where $N$ is the number of tokens in the expression.  
*Reason:* Although there is a nested `while` loop to pop operators, each operator is pushed onto the stack exactly once and popped at most once throughout the entire conversion process. Therefore, the aggregate number of stack operations across the whole algorithm is at most $2N$.

---

### Q12. What is the Time Complexity of Postfix evaluation?
**Answer:**  
The Time Complexity is **$O(N)$**. Each token in the postfix string is inspected once. Operands are pushed in $O(1)$ time, and each operator executes two pops, one arithmetic computation, and one push in $O(1)$ time.

---

### Q13. What is the Space Complexity of your system?
**Answer:**  
The auxiliary space complexity is **$O(N)$**, where $N$ is the length of the expression. This accounts for:
1. The custom array stack holding operators or numeric operands ($O(N)$).
2. The output token list and intermediate strings ($O(N)$).

---

### Q14. What is Stack Overflow and how does your project handle it?
**Answer:**  
Stack Overflow occurs when an attempt is made to push an element into a stack that has reached its maximum allocated capacity.  
In our implementation, rather than crashing or terminating the program, our custom array stack detects when `isFull()` is true and dynamically executes `resize()`, doubling the internal buffer capacity and preserving all data. This makes `push()` take **$O(1)$ amortized time**, with an **$O(N)$ worst case** only during a resizing step.

---

### Q15. What is Stack Underflow and how does your project handle it?
**Answer:**  
Stack Underflow occurs when an attempt is made to pop or peek from an empty stack (`topIndex == -1`).  
Our stack methods check `isEmpty()` and throw a standard `std::underflow_error` exception with a descriptive message. The calling functions catch this exception and display a clear user-friendly diagnostic.

---

### Q16. How is division by zero handled in your system?
**Answer:**  
During postfix and prefix evaluation, when a division (`/`) or modulo (`%`) operator is popped, the system inspects the divisor operand `op2`. If its absolute value is smaller than $10^{-12}$, the system throws a `std::runtime_error("Division by zero is not allowed")`. This prevents hardware CPU arithmetic exceptions and avoids crashing the console application.

---

### Q17. How does your expression validation system work?
**Answer:**  
Before any conversion or evaluation, the expression undergoes three verification stages:
1. **Lexical Validation:** Verifies all characters belong to valid mathematical characters.
2. **Parenthesis Matching:** Uses a custom `Stack<char>` to ensure every opening parenthesis `(` has an exact corresponding closing parenthesis `)` in correct order.
3. **Grammar State Machine:** Ensures alternating sequence of operands and operators, rejecting consecutive operators (e.g., `10 + * 5`), misplaced operators at the start or end, unsupported unary signs directly before parentheses (e.g., `-(5 + 2)`), and invalid decimal formats (e.g., `10.5.5`).

---

### Q18. Why cannot a Queue be used instead of a Stack for expression evaluation?
**Answer:**  
A Queue follows **FIFO (First In, First Out)**. In expression evaluation, we need to access the most recently evaluated sub-results first (the operands closest to the operator). A queue would yield the oldest operands entered at the beginning of the expression, destroying the mathematical order of operations.

---

### Q19. How did you handle multi-digit numbers and floating-point values?
**Answer:**  
We implemented a dedicated lexical tokenizer (`tokenizeExpression`). Rather than reading individual characters, the tokenizer scans contiguous sequences of digits and decimal points (e.g., `"125"` or `"25.5"`), grouping them into a single string token. In the postfix output, tokens are separated by spaces so that `125 + 50` becomes `"125 50 +"` rather than ambiguous single digits.

---

### Q20. How does Infix to Prefix conversion work?
**Answer:**  
We use the token-reversal algorithm:
1. Reverse the infix tokens.
2. Swap `(` with `)` and vice-versa.
3. Run Shunting-Yard conversion, with the operator associativity condition inverted.
4. Reverse the resulting postfix tokens to yield the final prefix expression.

---

### Q21. How is Prefix notation evaluated?
**Answer:**  
Prefix notation is evaluated by scanning the tokens from **Right to Left**:
- Operands are pushed onto the value stack.
- When an operator is encountered, two operands are popped: the first popped is the left operand `op1`, and the second popped is the right operand `op2`.
- `op1 <op> op2` is calculated and the result is pushed back onto the stack.

---

### Q22. How is expression history stored and what is its complexity?
**Answer:**  
History is stored in an array-based structure `ExpressionHistory` containing a fixed-size buffer of `HistoryEntry` records ($H=100$). Each record stores the infix, postfix, prefix, result, and timestamp.
- **History insertion:** $O(1)$ normally when appending; $O(H)$ when the buffer is full ($H=100$) to perform the sliding-window shift.
- **History display:** $O(H)$ time to traverse and display all active entries.
- **History space:** $O(H)$ auxiliary memory for the fixed array of size $H=100$.

---

### Q23. What are the limitations of this project?
**Answer:**  
1. Variables (such as `x`, `y`) with algebraic assignment are not supported; all operands must be numeric literals.
2. Advanced trigonometric functions (`sin`, `cos`) and logarithmic functions are not part of the basic arithmetic operator set.
3. The console UI is text-based rather than graphical.

---

### Q24. What future enhancements can be added to this project?
**Answer:**  
1. Support for variable symbol tables (e.g. `x = 10, y = 20, x + y`).
2. Support for scientific and trigonometric functions (`sin(x)`, `sqrt(x)`).
3. Expression tree (syntax tree) generation and visual graphical rendering.
4. Persistent file export (saving history to CSV or JSON).

---

### Q25. Can you demonstrate what happens when `(25 + 5) * 3 - 10` is evaluated?
**Answer:**  
1. **Tokenization:** `(`, `25`, `+`, `5`, `)`, `*`, `3`, `-`, `10`.
2. **Infix to Postfix:**
   - `(` pushed to stack.
   - `25` output.
   - `+` pushed to stack.
   - `5` output.
   - `)` pops `+` to output, discards `(`. (Output: `25 5 +`)
   - `*` pushed to stack.
   - `3` output.
   - `-` pops `*` to output (higher precedence), then pushes `-`. (Output: `25 5 + 3 *`)
   - `10` output.
   - Stack pops `-` to output.
   - **Postfix:** `25 5 + 3 * 10 -`
3. **Evaluation:**
   - Push `25`, push `5`.
   - `+` pops `5` and `25` $\rightarrow 25 + 5 = 30 \rightarrow$ push `30`.
   - Push `3`.
   - `*` pops `3` and `30` $\rightarrow 30 \times 3 = 90 \rightarrow$ push `90`.
   - Push `10`.
   - `-` pops `10` and `90` $\rightarrow 90 - 10 = 80 \rightarrow$ push `80`.
   - **Result:** `80`.
