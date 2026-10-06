/**
 * ============================================================================
 * File: conversion.cpp
 * Description: Implementation of Infix to Postfix and Infix to Prefix conversion
 * ============================================================================
 */

#include "../include/conversion.h"
#include "../include/stack.h"
#include "../include/validation.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

std::vector<std::string> tokenizeExpression(const std::string& expression) {
    std::vector<std::string> tokens;
    size_t i = 0;
    bool expectingOperand = true;

    while (i < expression.length()) {
        char ch = expression[i];

        if (std::isspace(static_cast<unsigned char>(ch))) {
            i++;
            continue;
        }

        // Check for unary plus or minus
        if ((ch == '+' || ch == '-') && expectingOperand) {
            // Check if next non-whitespace character is a digit or decimal point
            size_t nextPos = expression.find_first_not_of(" \t\r\n", i + 1);
            if (nextPos != std::string::npos && (std::isdigit(static_cast<unsigned char>(expression[nextPos])) || expression[nextPos] == '.')) {
                // Consume unary sign and number together as a single operand token
                std::string numToken;
                if (ch == '-') {
                    numToken += '-';
                } // Unary '+' can be omitted or kept; omitting '+' gives standard positive representation
                i = nextPos;
                while (i < expression.length() && (std::isdigit(static_cast<unsigned char>(expression[i])) || expression[i] == '.')) {
                    numToken += expression[i];
                    i++;
                }
                tokens.push_back(numToken);
                expectingOperand = false;
                continue;
            }
        }

        // Multi-digit and floating-point operand
        if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.') {
            std::string numToken;
            while (i < expression.length() && (std::isdigit(static_cast<unsigned char>(expression[i])) || expression[i] == '.')) {
                numToken += expression[i];
                i++;
            }
            tokens.push_back(numToken);
            expectingOperand = false;
            continue;
        }

        // Parentheses
        if (ch == '(') {
            tokens.push_back("(");
            expectingOperand = true;
            i++;
            continue;
        }

        if (ch == ')') {
            tokens.push_back(")");
            expectingOperand = false;
            i++;
            continue;
        }

        // Binary operators
        if (isOperator(ch)) {
            tokens.push_back(std::string(1, ch));
            expectingOperand = true;
            i++;
            continue;
        }

        // Fallback for any other single character
        tokens.push_back(std::string(1, ch));
        i++;
    }

    return tokens;
}

std::vector<TraceStep> infixToPostfixWithTrace(const std::string& expression, std::string& outPostfix) {
    std::vector<TraceStep> trace;
    std::vector<std::string> tokens = tokenizeExpression(expression);
    Stack<char> opStack(50);
    std::vector<std::string> output;
    int stepNum = 1;

    auto makeOutputStr = [](const std::vector<std::string>& outVec) {
        std::ostringstream oss;
        for (size_t k = 0; k < outVec.size(); ++k) {
            oss << outVec[k];
            if (k + 1 < outVec.size()) oss << " ";
        }
        return oss.str().empty() ? "(empty)" : oss.str();
    };

    for (const std::string& token : tokens) {
        if (token == "(") {
            opStack.push('(');
            trace.push_back({
                stepNum++,
                token,
                "Push '(' to stack",
                makeOutputStr(output),
                opStack.toString()
            });
        } else if (token == ")") {
            while (!opStack.isEmpty() && opStack.peek() != '(') {
                char popped = opStack.pop();
                output.push_back(std::string(1, popped));
            }
            if (!opStack.isEmpty() && opStack.peek() == '(') {
                opStack.pop(); // Discard '('
            }
            trace.push_back({
                stepNum++,
                token,
                "Pop until '(' and discard '('",
                makeOutputStr(output),
                opStack.toString()
            });
        } else if (token.length() == 1 && isOperator(token[0])) {
            char op = token[0];
            int prec = precedence(op);
            bool rightAssoc = isRightAssociative(op);

            while (!opStack.isEmpty() && opStack.peek() != '(') {
                char topOp = opStack.peek();
                int topPrec = precedence(topOp);

                if (topPrec > prec || (topPrec == prec && !rightAssoc)) {
                    output.push_back(std::string(1, opStack.pop()));
                } else {
                    break;
                }
            }
            opStack.push(op);
            trace.push_back({
                stepNum++,
                token,
                std::string("Push operator '") + op + "'",
                makeOutputStr(output),
                opStack.toString()
            });
        } else {
            // Operand token (number)
            output.push_back(token);
            trace.push_back({
                stepNum++,
                token,
                std::string("Send operand '") + token + "' to output",
                makeOutputStr(output),
                opStack.toString()
            });
        }
    }

    // Pop all remaining operators in stack
    while (!opStack.isEmpty()) {
        char topOp = opStack.pop();
        if (topOp != '(') {
            output.push_back(std::string(1, topOp));
        }
    }

    trace.push_back({
        stepNum++,
        "END",
        "Pop all remaining operators from stack",
        makeOutputStr(output),
        opStack.toString()
    });

    outPostfix = makeOutputStr(output);
    if (outPostfix == "(empty)") {
        outPostfix = "";
    }
    return trace;
}

std::string infixToPostfix(const std::string& expression) {
    std::string postfix;
    infixToPostfixWithTrace(expression, postfix);
    return postfix;
}

std::string infixToPrefix(const std::string& expression) {
    std::vector<std::string> tokens = tokenizeExpression(expression);
    if (tokens.empty()) return "";

    // Step 1: Reverse tokens
    std::reverse(tokens.begin(), tokens.end());

    // Step 2: Swap '(' and ')'
    for (std::string& t : tokens) {
        if (t == "(") t = ")";
        else if (t == ")") t = "(";
    }

    // Step 3: Run modified Shunting-Yard
    // For reversed infix, associativity condition is inverted:
    // When precedence is equal:
    // If operator was right-associative (e.g. '^'), it acts as left-associative in reverse (pop).
    // If operator was left-associative (e.g. '+', '*'), it acts as right-associative in reverse (do NOT pop).
    Stack<char> opStack(50);
    std::vector<std::string> output;

    for (const std::string& token : tokens) {
        if (token == "(") {
            opStack.push('(');
        } else if (token == ")") {
            while (!opStack.isEmpty() && opStack.peek() != '(') {
                output.push_back(std::string(1, opStack.pop()));
            }
            if (!opStack.isEmpty() && opStack.peek() == '(') {
                opStack.pop();
            }
        } else if (token.length() == 1 && isOperator(token[0])) {
            char op = token[0];
            int prec = precedence(op);
            bool rightAssoc = isRightAssociative(op);

            while (!opStack.isEmpty() && opStack.peek() != '(') {
                char topOp = opStack.peek();
                int topPrec = precedence(topOp);

                // Inverted associativity check for reversed tokens
                if (topPrec > prec || (topPrec == prec && rightAssoc)) {
                    output.push_back(std::string(1, opStack.pop()));
                } else {
                    break;
                }
            }
            opStack.push(op);
        } else {
            // Operand
            output.push_back(token);
        }
    }

    while (!opStack.isEmpty()) {
        char topOp = opStack.pop();
        if (topOp != '(') {
            output.push_back(std::string(1, topOp));
        }
    }

    // Step 4: Reverse the output tokens to obtain prefix
    std::reverse(output.begin(), output.end());

    std::ostringstream oss;
    for (size_t k = 0; k < output.size(); ++k) {
        oss << output[k];
        if (k + 1 < output.size()) oss << " ";
    }
    return oss.str();
}

void displayTraceTable(const std::vector<TraceStep>& trace) {
    std::cout << "\n+-----+----------+------------------------------------------+------------------------------+--------------------+\n";
    std::cout << "| No. | Token    | Action                                   | Output Buffer                | Stack (Bottom->Top)|\n";
    std::cout << "+-----+----------+------------------------------------------+------------------------------+--------------------+\n";

    for (const auto& step : trace) {
        std::cout << "| " << std::left << std::setw(3) << step.stepNumber << " | "
                  << std::setw(8) << step.token << " | "
                  << std::setw(40) << step.action.substr(0, 40) << " | "
                  << std::setw(28) << step.outputBuffer.substr(0, 28) << " | "
                  << std::setw(18) << step.stackContents.substr(0, 18) << " |\n";
    }

    std::cout << "+-----+----------+------------------------------------------+------------------------------+--------------------+\n";
}
