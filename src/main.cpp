/**
 * ============================================================================
 * Project: Expression Evaluation System Using Stack
 * Course: Data Structures & Algorithms (Academic Project #15)
 * File: main.cpp
 * Description: Interactive console-based menu driver application
 * ============================================================================
 */

#include "../include/conversion.h"
#include "../include/evaluation.h"
#include "../include/history.h"
#include "../include/stack.h"
#include "../include/validation.h"

#include <iostream>
#include <limits>
#include <string>

/**
 * @brief Displays the interactive system main menu.
 */
void displayMenu() {
    std::cout << "\n====================================================\n";
    std::cout << "          EXPRESSION EVALUATION SYSTEM             \n";
    std::cout << "====================================================\n";
    std::cout << "1. Evaluate Infix Expression\n";
    std::cout << "2. Convert Infix to Postfix\n";
    std::cout << "3. Evaluate Postfix Expression\n";
    std::cout << "4. Validate Expression\n";
    std::cout << "5. Convert Infix to Prefix\n";
    std::cout << "6. Evaluate Prefix Expression\n";
    std::cout << "7. View Expression History\n";
    std::cout << "8. Clear Expression History\n";
    std::cout << "9. Display DSA Information\n";
    std::cout << "10. Exit\n";
    std::cout << "====================================================\n";
    std::cout << "Enter your choice: ";
}

/**
 * @brief Helper function to safely read an entire line of text from std::cin.
 */
std::string readLineTrimmed() {
    std::string line;
    std::getline(std::cin, line);
    size_t first = line.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = line.find_last_not_of(" \t\r\n");
    return line.substr(first, (last - first + 1));
}

/**
 * @brief Prompts user for a yes/no choice.
 */
bool askYesNo(const std::string& prompt) {
    std::cout << prompt << " (y/n): ";
    std::string response;
    std::getline(std::cin, response);
    return (!response.empty() && (response[0] == 'y' || response[0] == 'Y'));
}

/**
 * @brief Prints an error message with optional indicator caret pointing to the syntax error.
 */
void printValidationError(const std::string& expr, const ValidationResult& res) {
    std::cout << "\n[!] " << res.errorMessage << "\n";
    if (res.errorPosition >= 0 && res.errorPosition < static_cast<int>(expr.length())) {
        std::cout << "    Expression: " << expr << "\n";
        std::cout << "    Position:   ";
        for (int i = 0; i < res.errorPosition; ++i) {
            std::cout << " ";
        }
        std::cout << "^\n";
    }
}

/**
 * @brief Handles Option 1: Evaluate Infix Expression
 */
void handleEvaluateInfix() {
    std::cout << "\n--- Evaluate Infix Expression ---\n";
    std::cout << "Enter Infix Expression (e.g., (25 + 5) * 3 - 10): ";
    std::string expr = readLineTrimmed();

    ValidationResult valRes = validateExpressionDetailed(expr);
    if (!valRes.isValid) {
        printValidationError(expr, valRes);
        return;
    }

    try {
        std::string postfix;
        std::vector<TraceStep> trace = infixToPostfixWithTrace(expr, postfix);
        std::string prefix = infixToPrefix(expr);

        bool showTrace = askYesNo("Would you like to view step-by-step conversion trace?");
        if (showTrace) {
            displayTraceTable(trace);
        }

        std::vector<EvalStep> evalTrace;
        double result = evaluatePostfixWithTrace(postfix, evalTrace);

        bool showEvalTrace = askYesNo("Would you like to view step-by-step evaluation trace?");
        if (showEvalTrace) {
            displayEvalTraceTable(evalTrace);
        }

        std::cout << "\n================ RESULT ================\n";
        std::cout << "Infix Expression:   " << expr << "\n";
        std::cout << "Postfix Expression: " << postfix << "\n";
        std::cout << "Prefix Expression:  " << prefix << "\n";
        std::cout << "Calculated Result:  " << result << "\n";
        std::cout << "========================================\n";

        addHistoryRecord(expr, postfix, prefix, result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

/**
 * @brief Handles Option 2: Convert Infix to Postfix
 */
void handleInfixToPostfix() {
    std::cout << "\n--- Convert Infix to Postfix ---\n";
    std::cout << "Enter Infix Expression (e.g., 10 + 5 * 2): ";
    std::string expr = readLineTrimmed();

    ValidationResult valRes = validateExpressionDetailed(expr);
    if (!valRes.isValid) {
        printValidationError(expr, valRes);
        return;
    }

    std::string postfix;
    std::vector<TraceStep> trace = infixToPostfixWithTrace(expr, postfix);

    bool showTrace = askYesNo("Would you like to view step-by-step conversion trace?");
    if (showTrace) {
        displayTraceTable(trace);
    }

    std::cout << "\nInfix Expression:   " << expr << "\n";
    std::cout << "Postfix Expression: " << postfix << "\n";
}

/**
 * @brief Handles Option 3: Evaluate Postfix Expression
 */
void handleEvaluatePostfix() {
    std::cout << "\n--- Evaluate Postfix Expression ---\n";
    std::cout << "Enter Postfix Expression with space-separated tokens\n";
    std::cout << "(e.g., 25 5 + 3 * 10 -): ";
    std::string postfix = readLineTrimmed();

    ValidationResult valRes = validatePostfixExpression(postfix);
    if (!valRes.isValid) {
        std::cout << "\n[!] " << valRes.errorMessage << "\n";
        return;
    }

    try {
        std::vector<EvalStep> trace;
        double result = evaluatePostfixWithTrace(postfix, trace);

        bool showTrace = askYesNo("Would you like to view step-by-step evaluation trace?");
        if (showTrace) {
            displayEvalTraceTable(trace);
        }

        std::cout << "\nPostfix Expression: " << postfix << "\n";
        std::cout << "Calculated Result:  " << result << "\n";

        addHistoryRecord("[Direct Postfix]", postfix, "", result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

/**
 * @brief Handles Option 4: Validate Expression
 */
void handleValidateExpression() {
    std::cout << "\n--- Validate Expression ---\n";
    std::cout << "Enter Expression to Validate: ";
    std::string expr = readLineTrimmed();

    ValidationResult res = validateExpressionDetailed(expr);
    if (res.isValid) {
        std::cout << "\n[SUCCESS] The expression is syntactically VALID.\n";
        std::cout << "Expression: " << expr << "\n";
    } else {
        printValidationError(expr, res);
    }
}

/**
 * @brief Handles Option 5: Convert Infix to Prefix
 */
void handleInfixToPrefix() {
    std::cout << "\n--- Convert Infix to Prefix ---\n";
    std::cout << "Enter Infix Expression (e.g., (10 + 5) * 2): ";
    std::string expr = readLineTrimmed();

    ValidationResult valRes = validateExpressionDetailed(expr);
    if (!valRes.isValid) {
        printValidationError(expr, valRes);
        return;
    }

    std::string prefix = infixToPrefix(expr);
    std::cout << "\nInfix Expression:  " << expr << "\n";
    std::cout << "Prefix Expression: " << prefix << "\n";
}

/**
 * @brief Handles Option 6: Evaluate Prefix Expression
 */
void handleEvaluatePrefix() {
    std::cout << "\n--- Evaluate Prefix Expression ---\n";
    std::cout << "Enter Prefix Expression with space-separated tokens\n";
    std::cout << "(e.g., * + 10 5 2): ";
    std::string prefix = readLineTrimmed();

    ValidationResult valRes = validatePrefixExpression(prefix);
    if (!valRes.isValid) {
        std::cout << "\n[!] " << valRes.errorMessage << "\n";
        return;
    }

    try {
        std::vector<EvalStep> trace;
        double result = evaluatePrefixWithTrace(prefix, trace);

        bool showTrace = askYesNo("Would you like to view step-by-step evaluation trace?");
        if (showTrace) {
            displayEvalTraceTable(trace);
        }

        std::cout << "\nPrefix Expression: " << prefix << "\n";
        std::cout << "Calculated Result: " << result << "\n";

        addHistoryRecord("[Direct Prefix]", "", prefix, result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

/**
 * @brief Handles Option 9: Display DSA Information
 */
void handleDisplayDSAInfo() {
    std::cout << "\n====================================================\n";
    std::cout << "         DATA STRUCTURES & ALGORITHMS OVERVIEW      \n";
    std::cout << "====================================================\n";
    std::cout << "\n1. WHY STACK?\n";
    std::cout << "   - Stack follows the LIFO (Last In, First Out) principle.\n";
    std::cout << "   - In expression parsing, higher precedence operators and nested\n";
    std::cout << "     parenthesized expressions must be processed before outer ones.\n";
    std::cout << "   - When converting Infix to Postfix, operators are held in stack\n";
    std::cout << "     until all operands and higher-precedence operations are resolved.\n";
    std::cout << "   - During evaluation, operands are pushed and the most recently\n";
    std::cout << "     placed operands are popped immediately when an operator appears.\n";

    std::cout << "\n2. WHY CUSTOM ARRAY IMPLEMENTATION?\n";
    std::cout << "   - Avoids relying as a black-box on std::stack from STL.\n";
    std::cout << "   - Visibly demonstrates contiguous memory management, index tracking,\n";
    std::cout << "     dynamic array expansion (doubling strategy), and bounds checking.\n";
    std::cout << "   - Guarantees O(1) time complexity for push, pop, and peek operations.\n";

    std::cout << "\n3. COMPLEXITY ANALYSIS:\n";
    std::cout << "   +---------------------------------+-----------------+-----------------+\n";
    std::cout << "   | Operation                       | Time Complexity | Space Complexity|\n";
    std::cout << "   +---------------------------------+-----------------+-----------------+\n";
    std::cout << "   | Expression Validation           | O(N)            | O(N)            |\n";
    std::cout << "   | Tokenization                    | O(N)            | O(N)            |\n";
    std::cout << "   | Infix to Postfix (Shunting-Yard)| O(N)            | O(N)            |\n";
    std::cout << "   | Postfix Evaluation              | O(N)            | O(N)            |\n";
    std::cout << "   | Infix to Prefix Conversion      | O(N)            | O(N)            |\n";
    std::cout << "   | Prefix Evaluation               | O(N)            | O(N)            |\n";
    std::cout << "   | Stack push(), pop(), peek()     | O(1) amortized  | O(1) per elem   |\n";
    std::cout << "   | History Log (Insert / Display)  | O(1) / O(H)     | O(H)            |\n";
    std::cout << "   +---------------------------------+-----------------+-----------------+\n";
    std::cout << "   * Where N = length of expression / token count, H = history capacity.\n\n";
}

int main() {
    int choice = 0;

    std::cout << "\n****************************************************\n";
    std::cout << "*     WELCOME TO EXPRESSION EVALUATION SYSTEM      *\n";
    std::cout << "*   College-Level Data Structures & Algorithms     *\n";
    std::cout << "****************************************************\n";

    while (true) {
        displayMenu();

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\n[End of Input detected. Exiting.]\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[!] Invalid input. Please enter an integer between 1 and 10.\n";
            continue;
        }

        // Consume remaining newline
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                handleEvaluateInfix();
                break;
            case 2:
                handleInfixToPostfix();
                break;
            case 3:
                handleEvaluatePostfix();
                break;
            case 4:
                handleValidateExpression();
                break;
            case 5:
                handleInfixToPrefix();
                break;
            case 6:
                handleEvaluatePrefix();
                break;
            case 7:
                displayHistory();
                break;
            case 8:
                clearHistory();
                break;
            case 9:
                handleDisplayDSAInfo();
                break;
            case 10:
                std::cout << "\nExiting Expression Evaluation System. Thank you!\n";
                return 0;
            default:
                std::cout << "\n[!] Invalid choice: " << choice << ". Please choose between 1 and 10.\n";
                break;
        }
    }

    return 0;
}
