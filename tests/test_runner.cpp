/**
 * ============================================================================
 * Project: Expression Evaluation System Using Stack
 * File: test_runner.cpp
 * Description: Automated Academic Test Suite & Validation Runner
 * ============================================================================
 */

#include "../include/stack.h"
#include "../include/validation.h"
#include "../include/conversion.h"
#include "../include/evaluation.h"
#include "../include/history.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <cassert>

struct TestCase {
    int id;
    std::string category;
    std::string input;
    bool expectedValid;
    std::string expectedPostfix;
    bool expectEvalSuccess;
    double expectedResult;
    std::string expectedErrorSubstring;
};

void runUnitTests() {
    std::cout << "\n======================================================\n";
    std::cout << "        RUNNING DSA CORE UNIT TESTS (CUSTOM STACK)    \n";
    std::cout << "======================================================\n";

    // 1. Stack Initialization
    Stack<int> s(2);
    assert(s.isEmpty());
    assert(s.size() == 0);
    std::cout << "[PASS] Stack initialization and empty check.\n";

    // 2. Stack Push & LIFO order
    s.push(10);
    s.push(20);
    assert(!s.isEmpty());
    assert(s.size() == 2);
    assert(s.peek() == 20);
    std::cout << "[PASS] Stack push and peek (LIFO top element verification).\n";

    // 3. Dynamic Array Resizing
    s.push(30); // exceeds initial capacity 2 -> should resize to 4
    assert(s.size() == 3);
    assert(s.peek() == 30);
    std::cout << "[PASS] Stack dynamic array doubling / resizing.\n";

    // 4. Stack Pop
    int p1 = s.pop();
    assert(p1 == 30);
    int p2 = s.pop();
    assert(p2 == 20);
    int p3 = s.pop();
    assert(p3 == 10);
    assert(s.isEmpty());
    std::cout << "[PASS] Stack pop in exact reverse insertion order (LIFO).\n";

    // 5. Stack Underflow Exception
    bool threwUnderflow = false;
    try {
        s.pop();
    } catch (const std::underflow_error&) {
        threwUnderflow = true;
    }
    assert(threwUnderflow);
    std::cout << "[PASS] Stack underflow exception handling.\n";

    // 6. Stack Copy Constructor and Assignment
    s.push(100);
    s.push(200);
    Stack<int> sCopy = s;
    assert(sCopy.size() == 2);
    assert(sCopy.pop() == 200);
    assert(sCopy.pop() == 100);
    assert(s.size() == 2); // original unmodified
    std::cout << "[PASS] Stack deep copy constructor & memory independence.\n";
}

int main() {
    runUnitTests();

    std::cout << "\n======================================================\n";
    std::cout << "       RUNNING EXPRESSION EVALUATION TEST SUITE       \n";
    std::cout << "======================================================\n";

    std::vector<TestCase> testCases = {
        // --- Core Academic Requirements & Regressions ---
        {1, "Basic Arithmetic", "5 + 3", true, "5 3 +", true, 8.0, ""},
        {2, "Operator Precedence", "10 + 5 * 2", true, "10 5 2 * +", true, 20.0, ""},
        {3, "Parentheses Grouping", "(10 + 5) * 2", true, "10 5 + 2 *", true, 30.0, ""},
        {4, "Division and Grouping", "100 / (5 * 2)", true, "100 5 2 * /", true, 10.0, ""},
        {5, "Modulo Operator", "25 % 4", true, "25 4 %", true, 1.0, ""},
        {6, "Exponentiation", "2 ^ 3", true, "2 3 ^", true, 8.0, ""},
        {7, "Right Associativity (^)", "2 ^ 3 ^ 2", true, "2 3 2 ^ ^", true, 512.0, ""},
        {8, "Decimal Values", "25.5 + 4.5", true, "25.5 4.5 +", true, 30.0, ""},
        {9, "Multi-digit Operands", "125 + 50", true, "125 50 +", true, 175.0, ""},
        {10, "Nested Complex Parens", "((10 + 5) * (2 + 3)) / 5", true, "10 5 + 2 3 + * 5 /", true, 15.0, ""},
        {11, "Unary Minus Operand", "(-5 * 2)", true, "-5 2 *", true, -10.0, ""},
        {12, "Division by Zero", "10 / 0", true, "10 0 /", false, 0.0, "Division by zero"},
        {13, "Modulo by Zero", "25 % 0", true, "25 0 %", false, 0.0, "Modulo by zero"},
        {14, "Consecutive Operators", "10 + * 5", false, "", false, 0.0, "Invalid operator placement"},
        {15, "Unclosed Parenthesis", "(10 + 5", false, "", false, 0.0, "Unbalanced parentheses"},
        {16, "Unexpected Closing Paren", "10 + 5)", false, "", false, 0.0, "Unbalanced parentheses"},
        {17, "Invalid Character", "10 + A", false, "", false, 0.0, "Invalid character"},
        {18, "Empty Expression", "", false, "", false, 0.0, "cannot be empty"},
        {19, "Single Operator Only", "*", false, "", false, 0.0, "Invalid operator placement"},
        {20, "Missing Operand Trailing Op", "10 +", false, "", false, 0.0, "incomplete operator"},
        {21, "Unary Minus Before Parens", "-(5 + 2)", false, "", false, 0.0, "Unary '-' before a parenthesized expression is not supported"},
        {22, "Unary Plus Before Parens", "+(5 + 2)", false, "", false, 0.0, "Unary '+' before a parenthesized expression is not supported"},
        {23, "Prefix Evaluation Equiv", "3 * (4 + 2)", true, "3 4 2 + *", true, 18.0, ""}
    };

    int passed = 0;
    int failed = 0;

    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(28) << "Input Expression"
              << std::setw(22) << "Expected Result"
              << std::setw(22) << "Actual Output"
              << std::setw(10) << "Status" << "\n";
    std::cout << std::string(86, '-') << "\n";

    for (const auto& tc : testCases) {
        bool testOk = true;
        std::string actualOutput;

        ValidationResult valRes = validateExpressionDetailed(tc.input);

        if (!tc.expectedValid) {
            // Expected validation failure
            if (valRes.isValid) {
                testOk = false;
                actualOutput = "Unexpectedly marked Valid";
            } else {
                actualOutput = "Syntax Error Detected";
                testOk = true;
            }
        } else {
            // Expected validation success
            if (!valRes.isValid) {
                testOk = false;
                actualOutput = "Validation Failed: " + valRes.errorMessage;
            } else {
                std::string postfix = infixToPostfix(tc.input);

                if (!tc.expectedPostfix.empty() && postfix != tc.expectedPostfix) {
                    testOk = false;
                    actualOutput = "Postfix Mismatch: " + postfix;
                } else if (!tc.expectEvalSuccess) {
                    // Expect evaluation runtime error (e.g. division by zero)
                    try {
                        double res = evaluatePostfix(postfix);
                        testOk = false;
                        actualOutput = "Expected Error, got " + std::to_string(res);
                    } catch (const std::exception& ex) {
                        std::string msg = ex.what();
                        if (msg.find(tc.expectedErrorSubstring) != std::string::npos) {
                            testOk = true;
                            actualOutput = "Caught Expected Error";
                        } else {
                            testOk = false;
                            actualOutput = "Wrong Error: " + msg;
                        }
                    }
                } else {
                    // Expect evaluation success
                    try {
                        double res = evaluatePostfix(postfix);
                        if (std::abs(res - tc.expectedResult) < 1e-6) {
                            testOk = true;
                            actualOutput = std::to_string(res);
                            // Also verify prefix evaluation yields identical result!
                            std::string prefix = infixToPrefix(tc.input);
                            double prefixRes = evaluatePrefix(prefix);
                            if (std::abs(prefixRes - tc.expectedResult) > 1e-6) {
                                testOk = false;
                                actualOutput += " (Prefix Mismatch: " + std::to_string(prefixRes) + ")";
                            }
                        } else {
                            testOk = false;
                            actualOutput = "Wrong Result: " + std::to_string(res);
                        }
                    } catch (const std::exception& ex) {
                        testOk = false;
                        actualOutput = "Eval Exception: " + std::string(ex.what());
                    }
                }
            }
        }

        std::string expectedStr;
        if (!tc.expectedValid) {
            expectedStr = "Syntax Error";
        } else if (!tc.expectEvalSuccess) {
            expectedStr = "Runtime Error";
        } else {
            std::ostringstream oss;
            oss << tc.expectedResult << " [" << tc.expectedPostfix << "]";
            expectedStr = oss.str();
        }

        std::cout << std::left
                  << std::setw(4) << tc.id
                  << std::setw(28) << (tc.input.empty() ? "\"\"" : tc.input)
                  << std::setw(22) << expectedStr.substr(0, 21)
                  << std::setw(22) << actualOutput.substr(0, 21)
                  << std::setw(10) << (testOk ? "[PASS]" : "[FAIL]") << "\n";

        if (testOk) passed++;
        else failed++;
    }

    std::cout << std::string(86, '-') << "\n";
    std::cout << "SUMMARY: Total: " << testCases.size()
              << " | Passed: " << passed
              << " | Failed: " << failed << "\n";
    std::cout << "======================================================\n";

    return (failed == 0) ? 0 : 1;
}
