# Expression Evaluation System — Algorithm Pseudocode

This document contains formal pseudocode for all core algorithms implemented in the **Expression Evaluation System Using Stack**.

---

## 1. Custom Array-Based Stack Operations

### 1.1 Push Operation
```text
Algorithm PUSH(stack, element):
    Input: Stack structure containing data[], topIndex, capacity, and the element to insert
    Output: None (element placed at top of stack)

    1. If stack.topIndex == stack.capacity - 1 then:
         // Capacity reached: dynamically allocate array of size (capacity * 2)
         Allocate newData array of size 2 * stack.capacity
         Copy data[0 .. topIndex] into newData[0 .. topIndex]
         Deallocate old data[] array
         Set data = newData
         Set capacity = capacity * 2
    2. Increment stack.topIndex by 1
    3. Set stack.data[stack.topIndex] = element
    4. Return
```

### 1.2 Pop Operation
```text
Algorithm POP(stack):
    Input: Stack structure
    Output: Element removed from top of stack

    1. If stack.topIndex == -1 then:
         Raise Underflow_Error("Attempted to pop from empty stack")
    2. Set poppedElement = stack.data[stack.topIndex]
    3. Decrement stack.topIndex by 1
    4. Return poppedElement
```

### 1.3 Peek Operation
```text
Algorithm PEEK(stack):
    Input: Stack structure
    Output: Element currently at the top of stack without removal

    1. If stack.topIndex == -1 then:
         Raise Underflow_Error("Attempted to peek into empty stack")
    2. Return stack.data[stack.topIndex]
```

---

## 2. Expression Syntax & Lexical Validation

```text
Algorithm VALIDATE_EXPRESSION(expression):
    Input: String expression
    Output: (isValid: Boolean, errorMessage: String, errorPosition: Integer)

    1. If expression is empty or contains only whitespace:
         Return (False, "Expression cannot be empty", 0)

    2. For each character ch at index i in expression:
         If ch is not a digit, not '.', not in {+, -, *, /, %, ^}, and not in {'(', ')'}:
              Return (False, "Invalid character found: " + ch, i)

    3. Initialize parenStack as empty Stack of Characters
    4. For each character ch at index i in expression:
         If ch == '(' then:
              PUSH(parenStack, '(')
         Else if ch == ')' then:
              If parenStack.isEmpty() then:
                   Return (False, "Unbalanced parentheses (unexpected closing parenthesis)", i)
              POP(parenStack)

    5. If NOT parenStack.isEmpty() then:
         Return (False, "Unbalanced parentheses (missing closing parenthesis)", lastOpenPos)

    6. Initialize expectingOperand = True
       Initialize hasSeenOperand = False
       Initialize lastNonSpace = NULL

    7. For each token in expression (lexical scanner):
         If token is a NUMBER:
              If expectingOperand == False:
                   Return (False, "Missing operator between operands", token.position)
              If number has more than one '.' decimal point:
                   Return (False, "Invalid number format (multiple decimal points)", token.position)
              expectingOperand = False
              hasSeenOperand = True

         Else if token == '(':
              If expectingOperand == False:
                   Return (False, "Missing operator before opening parenthesis", token.position)
              If next token == ')':
                   Return (False, "Empty parentheses '()' found", token.position)
              expectingOperand = True

         Else if token == ')':
              If expectingOperand == True:
                   Return (False, "Invalid operator before closing parenthesis", token.position)
              expectingOperand = False

         Else if token is OPERATOR op:
              If expectingOperand == True:
                   If (op == '+' or op == '-') and (lastNonSpace == NULL or lastNonSpace == '('):
                        // Valid unary operator attaching to next number
                        Continue
                   Else:
                        Return (False, "Invalid operator placement / consecutive operators", token.position)
              Else:
                   expectingOperand = True

         Set lastNonSpace = token

    8. If expectingOperand == True:
         Return (False, "Expression ends with incomplete operator or missing operand", lastPos)

    9. If hasSeenOperand == False:
         Return (False, "Expression contains no operands", 0)

    10. Return (True, "Valid expression", -1)
```

---

## 3. Infix to Postfix Conversion (Shunting-Yard Algorithm)

```text
Algorithm INFIX_TO_POSTFIX(expression):
    Input: Infix string expression
    Output: Space-separated Postfix string

    1. Tokenize expression into tokens (handling multi-digit numbers, decimals, unary signs)
    2. Initialize operatorStack as empty Stack of Characters
    3. Initialize outputTokens as empty List of Strings

    4. For each token in tokens:
         a. If token is an OPERAND (e.g. "25", "5.5", "-10"):
              Append token to outputTokens

         b. Else if token == "(":
              PUSH(operatorStack, '(')

         c. Else if token == ")":
              While NOT operatorStack.isEmpty() AND PEEK(operatorStack) != '(':
                   Append POP(operatorStack) to outputTokens
              If NOT operatorStack.isEmpty() AND PEEK(operatorStack) == '(':
                   POP(operatorStack)  // Discard matching '('

         d. Else if token is an OPERATOR op:
              While NOT operatorStack.isEmpty() AND PEEK(operatorStack) != '(' AND
                    (PRECEDENCE(PEEK(operatorStack)) > PRECEDENCE(op) OR
                    (PRECEDENCE(PEEK(operatorStack)) == PRECEDENCE(op) AND NOT IS_RIGHT_ASSOCIATIVE(op))):
                   Append POP(operatorStack) to outputTokens
              PUSH(operatorStack, op)

    5. While NOT operatorStack.isEmpty():
         topOp = POP(operatorStack)
         If topOp != '(':
              Append topOp to outputTokens

    6. Return outputTokens joined by single space
```

---

## 4. Postfix Expression Evaluation

```text
Algorithm EVALUATE_POSTFIX(postfixExpression):
    Input: Space-separated Postfix string
    Output: Numerical result (Double)

    1. Tokenize postfixExpression by whitespace into tokens
    2. Initialize valueStack as empty Stack of Doubles

    3. For each token in tokens:
         a. If token is an OPERAND:
              Convert token string to numerical double value
              PUSH(valueStack, value)

         b. Else if token is an OPERATOR op:
              If valueStack.size() < 2:
                   Raise Error("Invalid postfix expression: Insufficient operands")

              op2 = POP(valueStack)  // Right operand
              op1 = POP(valueStack)  // Left operand

              If op == '/' AND abs(op2) < 1e-12:
                   Raise Error("Division by zero is not allowed")
              If op == '%' AND abs(op2) < 1e-12:
                   Raise Error("Modulo by zero is not allowed")

              Switch op:
                   Case '+': result = op1 + op2
                   Case '-': result = op1 - op2
                   Case '*': result = op1 * op2
                   Case '/': result = op1 / op2
                   Case '%': result = fmod(op1, op2)
                   Case '^': result = pow(op1, op2)

              PUSH(valueStack, result)

    4. If valueStack.size() != 1:
         Raise Error("Invalid postfix expression: Too many operands remaining")

    5. Return POP(valueStack)
```

---

## 5. Infix to Prefix Conversion

```text
Algorithm INFIX_TO_PREFIX(expression):
    Input: Infix string expression
    Output: Space-separated Prefix string

    1. Tokenize expression into tokens
    2. Reverse the order of tokens
    3. For each token in reversed tokens:
         If token == "(": token = ")"
         Else if token == ")": token = "("

    4. Initialize operatorStack as empty Stack of Characters
    5. Initialize outputTokens as empty List of Strings

    6. For each token in tokens:
         a. If token is an OPERAND:
              Append token to outputTokens
         b. Else if token == "(":
              PUSH(operatorStack, '(')
         c. Else if token == ")":
              While NOT operatorStack.isEmpty() AND PEEK(operatorStack) != '(':
                   Append POP(operatorStack) to outputTokens
              If NOT operatorStack.isEmpty(): POP(operatorStack)
         d. Else if token is an OPERATOR op:
              // For reversed expression, associativity condition is inverted
              While NOT operatorStack.isEmpty() AND PEEK(operatorStack) != '(' AND
                    (PRECEDENCE(PEEK(operatorStack)) > PRECEDENCE(op) OR
                    (PRECEDENCE(PEEK(operatorStack)) == PRECEDENCE(op) AND IS_RIGHT_ASSOCIATIVE(op))):
                   Append POP(operatorStack) to outputTokens
              PUSH(operatorStack, op)

    7. While NOT operatorStack.isEmpty():
         Append POP(operatorStack) to outputTokens

    8. Reverse the outputTokens list
    9. Return outputTokens joined by space
```

---

## 6. Prefix Expression Evaluation

```text
Algorithm EVALUATE_PREFIX(prefixExpression):
    Input: Space-separated Prefix string
    Output: Numerical result (Double)

    1. Tokenize prefixExpression by whitespace into tokens
    2. Initialize valueStack as empty Stack of Doubles

    3. For index i = (tokens.length - 1) down to 0 (SCAN RIGHT TO LEFT):
         token = tokens[i]
         a. If token is an OPERAND:
              Convert token to double value
              PUSH(valueStack, value)

         b. Else if token is an OPERATOR op:
              If valueStack.size() < 2:
                   Raise Error("Invalid prefix expression: Insufficient operands")

              // In right-to-left scan:
              op1 = POP(valueStack)  // Left operand
              op2 = POP(valueStack)  // Right operand

              If op == '/' AND abs(op2) < 1e-12:
                   Raise Error("Division by zero is not allowed")
              If op == '%' AND abs(op2) < 1e-12:
                   Raise Error("Modulo by zero is not allowed")

              Switch op:
                   Case '+': result = op1 + op2
                   Case '-': result = op1 - op2
                   Case '*': result = op1 * op2
                   Case '/': result = op1 / op2
                   Case '%': result = fmod(op1, op2)
                   Case '^': result = pow(op1, op2)

              PUSH(valueStack, result)

    4. If valueStack.size() != 1:
         Raise Error("Invalid prefix expression structure")

    5. Return POP(valueStack)
```
