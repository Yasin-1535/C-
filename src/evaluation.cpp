/**
 * ============================================================================
 * File: evaluation.cpp
 * Description: Implementation of Postfix and Prefix Evaluation using Stack
 * ============================================================================
 */

#include "../include/evaluation.h"
#include "../include/stack.h"
#include "../include/validation.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
    double applyOperation(char op, double op1, double op2) {
        const double EPSILON = 1e-12;
        switch (op) {
            case '+':
                return op1 + op2;
            case '-':
                return op1 - op2;
            case '*':
                return op1 * op2;
            case '/':
                if (std::abs(op2) < EPSILON) {
                    throw std::runtime_error("Error: Division by zero is not allowed.");
                }
                return op1 / op2;
            case '%':
                if (std::abs(op2) < EPSILON) {
                    throw std::runtime_error("Error: Modulo by zero is not allowed.");
                }
                return std::fmod(op1, op2);
            case '^':
                return std::pow(op1, op2);
            default:
                throw std::runtime_error(std::string("Error: Unknown operator '") + op + "' encountered.");
        }
    }
}

double evaluatePostfixWithTrace(const std::string& postfix, std::vector<EvalStep>& outTrace) {
    outTrace.clear();
    std::istringstream iss(postfix);
    std::string token;
    Stack<double> valStack(50);
    int stepNum = 1;

    while (iss >> token) {
        if (token.length() == 1 && isOperator(token[0])) {
            char op = token[0];
            if (valStack.size() < 2) {
                throw std::runtime_error(std::string("Error: Invalid postfix expression: Insufficient operands for operator '") + op + "'.");
            }
            double op2 = valStack.pop();
            double op1 = valStack.pop();
            double res = applyOperation(op, op1, op2);
            valStack.push(res);

            std::ostringstream act;
            act << "Pop " << op2 << ", " << op1 << " -> Compute " << op1 << " " << op << " " << op2 << " = " << res << " -> Push " << res;
            outTrace.push_back({
                stepNum++,
                token,
                act.str(),
                valStack.toString()
            });
        } else {
            // Number literal
            char* endPtr = nullptr;
            double val = std::strtod(token.c_str(), &endPtr);
            if (*endPtr != '\0') {
                throw std::runtime_error(std::string("Error: Invalid numeric token '") + token + "' in postfix expression.");
            }
            valStack.push(val);

            std::ostringstream act;
            act << "Push operand " << val;
            outTrace.push_back({
                stepNum++,
                token,
                act.str(),
                valStack.toString()
            });
        }
    }

    if (valStack.isEmpty()) {
        throw std::runtime_error("Error: Postfix expression is empty.");
    }
    if (valStack.size() > 1) {
        throw std::runtime_error("Error: Invalid postfix expression: Too many operands remaining on stack.");
    }

    return valStack.pop();
}

double evaluatePostfix(const std::string& postfix) {
    std::vector<EvalStep> trace;
    return evaluatePostfixWithTrace(postfix, trace);
}

double evaluatePrefixWithTrace(const std::string& prefix, std::vector<EvalStep>& outTrace) {
    outTrace.clear();
    std::istringstream iss(prefix);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        throw std::runtime_error("Error: Prefix expression is empty.");
    }

    Stack<double> valStack(50);
    int stepNum = 1;

    // Scan from Right to Left
    for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; --i) {
        const std::string& t = tokens[i];
        if (t.length() == 1 && isOperator(t[0])) {
            char op = t[0];
            if (valStack.size() < 2) {
                throw std::runtime_error(std::string("Error: Invalid prefix expression: Insufficient operands for operator '") + op + "'.");
            }
            // In right-to-left scan:
            // First popped element is the left operand (op1)
            // Second popped element is the right operand (op2)
            double op1 = valStack.pop();
            double op2 = valStack.pop();
            double res = applyOperation(op, op1, op2);
            valStack.push(res);

            std::ostringstream act;
            act << "Pop " << op1 << ", " << op2 << " -> Compute " << op1 << " " << op << " " << op2 << " = " << res << " -> Push " << res;
            outTrace.push_back({
                stepNum++,
                t,
                act.str(),
                valStack.toString()
            });
        } else {
            char* endPtr = nullptr;
            double val = std::strtod(t.c_str(), &endPtr);
            if (*endPtr != '\0') {
                throw std::runtime_error(std::string("Error: Invalid numeric token '") + t + "' in prefix expression.");
            }
            valStack.push(val);

            std::ostringstream act;
            act << "Push operand " << val;
            outTrace.push_back({
                stepNum++,
                t,
                act.str(),
                valStack.toString()
            });
        }
    }

    if (valStack.size() != 1) {
        throw std::runtime_error("Error: Invalid prefix expression: Operand count mismatch.");
    }

    return valStack.pop();
}

double evaluatePrefix(const std::string& prefix) {
    std::vector<EvalStep> trace;
    return evaluatePrefixWithTrace(prefix, trace);
}

void displayEvalTraceTable(const std::vector<EvalStep>& trace) {
    std::cout << "\n+-----+----------+--------------------------------------------------------------+--------------------+\n";
    std::cout << "| No. | Token    | Action Performed                                             | Stack (Bottom->Top)|\n";
    std::cout << "+-----+----------+--------------------------------------------------------------+--------------------+\n";

    for (const auto& step : trace) {
        std::cout << "| " << std::left << std::setw(3) << step.stepNumber << " | "
                  << std::setw(8) << step.token << " | "
                  << std::setw(60) << step.action.substr(0, 60) << " | "
                  << std::setw(18) << step.stackContents.substr(0, 18) << " |\n";
    }

    std::cout << "+-----+----------+--------------------------------------------------------------+--------------------+\n";
}
