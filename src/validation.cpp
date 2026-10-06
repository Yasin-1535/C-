/**
 * ============================================================================
 * File: validation.cpp
 * Description: Implementation of Expression Syntax and Lexical Validation
 * ============================================================================
 */

#include "../include/validation.h"
#include "../include/stack.h"
#include <cctype>
#include <sstream>
#include <vector>

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^');
}

bool isOperandChar(char ch) {
    return (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.');
}

bool isOperand(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch));
}

int precedence(char op) {
    switch (op) {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

bool isRightAssociative(char op) {
    // Only exponentiation '^' has right-to-left associativity in mathematics.
    return (op == '^');
}

bool isValidExpression(const std::string& expression) {
    ValidationResult res = validateExpressionDetailed(expression);
    return res.isValid;
}

ValidationResult validateExpressionDetailed(const std::string& expression) {
    // 1. Check for empty or whitespace-only expression
    size_t firstNonSpace = expression.find_first_not_of(" \t\r\n");
    if (firstNonSpace == std::string::npos) {
        return {false, "Error: Expression cannot be empty.", 0};
    }

    // 2. Check for invalid characters
    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        if (std::isspace(static_cast<unsigned char>(ch))) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(ch)) &&
            ch != '.' &&
            !isOperator(ch) &&
            ch != '(' && ch != ')') {
            return {false, std::string("Error: Invalid character found: '") + ch + "'", static_cast<int>(i)};
        }
    }

    // 3. Parentheses balance verification using custom Array Stack
    Stack<char> parenStack(50);
    Stack<int> parenPosStack(50);

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        if (ch == '(') {
            parenStack.push('(');
            parenPosStack.push(static_cast<int>(i));
        } else if (ch == ')') {
            if (parenStack.isEmpty()) {
                return {false, "Error: Unbalanced parentheses (unexpected closing parenthesis).", static_cast<int>(i)};
            }
            parenStack.pop();
            parenPosStack.pop();
        }
    }

    if (!parenStack.isEmpty()) {
        int openPos = parenPosStack.peek();
        return {false, "Error: Unbalanced parentheses (missing closing parenthesis).", openPos};
    }

    // 4. Tokenization-level syntax verification
    // State machine tracking:
    // expectingOperand: true at start, after operator, after '('
    // expectingOperator: true after operand literal, after ')'
    bool expectingOperand = true;
    bool hasSeenOperand = false;
    char lastNonSpaceChar = '\0';
    int lastNonSpacePos = -1;

    size_t i = 0;
    while (i < expression.length()) {
        char ch = expression[i];

        if (std::isspace(static_cast<unsigned char>(ch))) {
            i++;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.') {
            if (!expectingOperand) {
                return {false, "Error: Missing operator between operands.", static_cast<int>(i)};
            }

            // Parse full number to validate decimal format
            int dotCount = (ch == '.') ? 1 : 0;
            size_t numStart = i;
            i++;
            while (i < expression.length() && (std::isdigit(static_cast<unsigned char>(expression[i])) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    dotCount++;
                    if (dotCount > 1) {
                        return {false, "Error: Invalid number format (multiple decimal points in a single operand).", static_cast<int>(i)};
                    }
                }
                i++;
            }

            // Check for lone decimal point without digits
            if (dotCount == 1 && (i - numStart == 1)) {
                return {false, "Error: Invalid decimal format (lone decimal point '.').", static_cast<int>(numStart)};
            }

            expectingOperand = false;
            hasSeenOperand = true;
            lastNonSpaceChar = '0'; // placeholder for operand
            lastNonSpacePos = static_cast<int>(i - 1);
            continue;
        }

        if (ch == '(') {
            if (!expectingOperand) {
                return {false, "Error: Missing operator before opening parenthesis.", static_cast<int>(i)};
            }
            // Check for empty parentheses ()
            size_t nextPos = expression.find_first_not_of(" \t\r\n", i + 1);
            if (nextPos != std::string::npos && expression[nextPos] == ')') {
                return {false, "Error: Empty parentheses '()' found.", static_cast<int>(i)};
            }
            expectingOperand = true;
            lastNonSpaceChar = '(';
            lastNonSpacePos = static_cast<int>(i);
            i++;
            continue;
        }

        if (ch == ')') {
            if (expectingOperand) {
                return {false, "Error: Invalid operator placement before closing parenthesis.", static_cast<int>(i)};
            }
            expectingOperand = false;
            lastNonSpaceChar = ')';
            lastNonSpacePos = static_cast<int>(i);
            i++;
            continue;
        }

        if (isOperator(ch)) {
            // Handle unary minus or plus at start or after '('
            if (expectingOperand) {
                if ((ch == '-' || ch == '+') && (lastNonSpaceChar == '\0' || lastNonSpaceChar == '(')) {
                    size_t nextPos = expression.find_first_not_of(" \t\r\n", i + 1);
                    if (nextPos != std::string::npos && expression[nextPos] == '(') {
                        return {false, std::string("Error: Unary '") + ch + "' before a parenthesized expression is not supported.", static_cast<int>(i)};
                    }
                    if (nextPos == std::string::npos ||
                        (!std::isdigit(static_cast<unsigned char>(expression[nextPos])) &&
                         expression[nextPos] != '.')) {
                        return {false, "Error: Missing operand following unary operator.", static_cast<int>(i)};
                    }
                    // Valid unary sign
                    lastNonSpaceChar = ch;
                    lastNonSpacePos = static_cast<int>(i);
                    i++;
                    continue;
                } else {
                    return {false, "Error: Invalid operator placement (consecutive operators or leading binary operator).", static_cast<int>(i)};
                }
            } else {
                // We were expecting an operator, so this binary operator is valid here
                expectingOperand = true;
                lastNonSpaceChar = ch;
                lastNonSpacePos = static_cast<int>(i);
                i++;
                continue;
            }
        }

        i++;
    }

    if (expectingOperand) {
        return {false, "Error: Expression ends with an incomplete operator or missing operand.", lastNonSpacePos};
    }

    if (!hasSeenOperand) {
        return {false, "Error: Expression contains no operands.", 0};
    }

    return {true, "Valid expression.", -1};
}

ValidationResult validatePostfixExpression(const std::string& postfix) {
    std::istringstream iss(postfix);
    std::string token;
    int operandCount = 0;

    while (iss >> token) {
        if (token.length() == 1 && isOperator(token[0])) {
            if (operandCount < 2) {
                return {false, "Error: Invalid postfix expression: Insufficient operands for operator '" + token + "'.", -1};
            }
            operandCount -= 1; // 2 operands consumed, 1 result pushed
        } else {
            // Check if valid numeric token
            char* endPtr = nullptr;
            std::strtod(token.c_str(), &endPtr);
            if (*endPtr != '\0') {
                return {false, "Error: Invalid token in postfix expression: '" + token + "'.", -1};
            }
            operandCount += 1;
        }
    }

    if (operandCount == 0) {
        return {false, "Error: Empty postfix expression.", -1};
    }
    if (operandCount > 1) {
        return {false, "Error: Invalid postfix expression: Too many operands remaining.", -1};
    }

    return {true, "Valid postfix expression.", -1};
}

ValidationResult validatePrefixExpression(const std::string& prefix) {
    std::istringstream iss(prefix);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return {false, "Error: Empty prefix expression.", -1};
    }

    // Evaluate prefix operand count by scanning from right to left
    int operandCount = 0;
    for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; --i) {
        const std::string& t = tokens[i];
        if (t.length() == 1 && isOperator(t[0])) {
            if (operandCount < 2) {
                return {false, "Error: Invalid prefix expression: Insufficient operands for operator '" + t + "'.", -1};
            }
            operandCount -= 1;
        } else {
            char* endPtr = nullptr;
            std::strtod(t.c_str(), &endPtr);
            if (*endPtr != '\0') {
                return {false, "Error: Invalid token in prefix expression: '" + t + "'.", -1};
            }
            operandCount += 1;
        }
    }

    if (operandCount != 1) {
        return {false, "Error: Invalid prefix expression structure.", -1};
    }

    return {true, "Valid prefix expression.", -1};
}
