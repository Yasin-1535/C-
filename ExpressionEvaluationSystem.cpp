/**
 * ============================================================================
 * Academic Project: Expression Evaluation System Using Stack
 * Course: Data Structures & Algorithms (Topic #15)
 * Standard: C++17
 *
 * Description:
 *   A complete, self-contained Data Structures & Algorithms project implementing:
 *   - Custom Array-based Generic Stack (LIFO)
 *   - Expression Syntax & Lexical Validation
 *   - Infix to Postfix Conversion (Shunting-Yard Algorithm)
 *   - Infix to Prefix Conversion
 *   - Postfix Expression Evaluation
 *   - Prefix Expression Evaluation
 *   - Step-by-Step Educational Stack Tracing
 *   - History Management using Fixed-Array Buffer
 *
 * Compilation:
 *   g++ -std=c++17 ExpressionEvaluationSystem.cpp -o ExpressionEvaluationSystem
 * Execution:
 *   ./ExpressionEvaluationSystem          (Linux / macOS)
 *   ExpressionEvaluationSystem.exe        (Windows)
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <ctime>

// ============================================================================
// SECTION 1: CUSTOM GENERIC ARRAY-BASED STACK DATA STRUCTURE
// ============================================================================

/**
 * @class Stack
 * @brief Custom array-based generic stack demonstrating LIFO (Last In, First Out).
 *
 * Memory Model:
 * - Dynamically allocated contiguous memory block: `T* data`
 * - Track index of topmost element: `int topIndex`
 * - Track current allocated size: `int capacity`
 */
template <typename T>
class Stack {
private:
    T* data;           ///< Dynamic array holding stack elements
    int topIndex;      ///< Top index pointer (-1 means empty)
    int capacity;      ///< Maximum elements currently allocated

    /**
     * @brief Automatically doubles capacity when stack becomes full.
     * Demonstrates dynamic array resizing with amortized O(1) push complexity.
     */
    void resize() {
        int newCapacity = (capacity == 0) ? 4 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    explicit Stack(int initialCapacity = 100)
        : topIndex(-1), capacity(initialCapacity) {
        if (capacity <= 0) capacity = 10;
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    Stack(const Stack& other)
        : topIndex(other.topIndex), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i <= topIndex; ++i) {
            data[i] = other.data[i];
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            topIndex = other.topIndex;
            data = new T[capacity];
            for (int i = 0; i <= topIndex; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    bool isFull() const {
        return topIndex == capacity - 1;
    }

    void push(const T& element) {
        if (isFull()) {
            resize();
        }
        data[++topIndex] = element;
    }

    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Attempted pop from empty stack.");
        }
        return data[topIndex--];
    }

    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Attempted peek into empty stack.");
        }
        return data[topIndex];
    }

    const T& top() const {
        return peek();
    }

    int size() const {
        return topIndex + 1;
    }

    void clear() {
        topIndex = -1;
    }

    void display() const {
        if (isEmpty()) {
            std::cout << "[ Empty Stack ]\n";
            return;
        }
        std::cout << "Stack (bottom -> top): [ ";
        for (int i = 0; i <= topIndex; ++i) {
            std::cout << data[i];
            if (i < topIndex) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    std::string toString() const {
        if (isEmpty()) return "EMPTY";
        std::ostringstream oss;
        for (int i = 0; i <= topIndex; ++i) {
            oss << data[i];
            if (i < topIndex) oss << " ";
        }
        return oss.str();
    }
};

// ============================================================================
// SECTION 2: SYNTAX & LEXICAL VALIDATION
// ============================================================================

struct ValidationResult {
    bool isValid;
    std::string errorMessage;
    int errorPosition;
};

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^');
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
    return (op == '^');
}

ValidationResult validateExpressionDetailed(const std::string& expression) {
    // 1. Check for empty string
    size_t firstNonSpace = expression.find_first_not_of(" \t\r\n");
    if (firstNonSpace == std::string::npos) {
        return {false, "Error: Expression cannot be empty.", 0};
    }

    // 2. Character validity check
    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        if (std::isspace(static_cast<unsigned char>(ch))) continue;
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
                return {false, "Error: Unbalanced parentheses: Unexpected closing parenthesis ')'.", static_cast<int>(i)};
            }
            parenStack.pop();
            parenPosStack.pop();
        }
    }

    if (!parenStack.isEmpty()) {
        int openPos = parenPosStack.peek();
        return {false, "Error: Unbalanced parentheses: Missing closing parenthesis ')'.", openPos};
    }

    // 4. Grammar & state machine verification
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

            if (dotCount == 1 && (i - numStart == 1)) {
                return {false, "Error: Invalid decimal format (lone decimal point '.').", static_cast<int>(numStart)};
            }

            expectingOperand = false;
            hasSeenOperand = true;
            lastNonSpaceChar = '0';
            lastNonSpacePos = static_cast<int>(i - 1);
            continue;
        }

        if (ch == '(') {
            if (!expectingOperand) {
                return {false, "Error: Missing operator before opening parenthesis.", static_cast<int>(i)};
            }
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
            if (expectingOperand) {
                // Check if this is a unary plus or minus
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
                    lastNonSpaceChar = ch;
                    lastNonSpacePos = static_cast<int>(i);
                    i++;
                    continue;
                } else {
                    return {false, "Error: Invalid operator placement (consecutive operators or leading operator).", static_cast<int>(i)};
                }
            } else {
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

bool isValidExpression(const std::string& expression) {
    return validateExpressionDetailed(expression).isValid;
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
            operandCount -= 1;
        } else {
            char* endPtr = nullptr;
            std::strtod(token.c_str(), &endPtr);
            if (*endPtr != '\0') {
                return {false, "Error: Invalid token in postfix expression: '" + token + "'.", -1};
            }
            operandCount += 1;
        }
    }

    if (operandCount == 0) return {false, "Error: Empty postfix expression.", -1};
    if (operandCount > 1) return {false, "Error: Invalid postfix expression: Too many operands remaining.", -1};

    return {true, "Valid postfix expression.", -1};
}

ValidationResult validatePrefixExpression(const std::string& prefix) {
    std::istringstream iss(prefix);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) tokens.push_back(token);

    if (tokens.empty()) return {false, "Error: Empty prefix expression.", -1};

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

    if (operandCount != 1) return {false, "Error: Invalid prefix expression structure.", -1};

    return {true, "Valid prefix expression.", -1};
}

// ============================================================================
// SECTION 3: TOKENIZATION & CONVERSIONS (INFIX -> POSTFIX & PREFIX)
// ============================================================================

struct TraceStep {
    int stepNumber;
    std::string token;
    std::string action;
    std::string outputBuffer;
    std::string stackContents;
};

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

        // Unary sign check
        if ((ch == '+' || ch == '-') && expectingOperand) {
            size_t nextPos = expression.find_first_not_of(" \t\r\n", i + 1);
            if (nextPos != std::string::npos && (std::isdigit(static_cast<unsigned char>(expression[nextPos])) || expression[nextPos] == '.')) {
                std::string numToken;
                if (ch == '-') numToken += '-';
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

        // Multi-digit numbers
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

        if (isOperator(ch)) {
            tokens.push_back(std::string(1, ch));
            expectingOperand = true;
            i++;
            continue;
        }

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
            trace.push_back({stepNum++, token, "Push '(' to operator stack", makeOutputStr(output), opStack.toString()});
        } else if (token == ")") {
            while (!opStack.isEmpty() && opStack.peek() != '(') {
                output.push_back(std::string(1, opStack.pop()));
            }
            if (!opStack.isEmpty() && opStack.peek() == '(') {
                opStack.pop();
            }
            trace.push_back({stepNum++, token, "Pop operators until '(' and discard '('", makeOutputStr(output), opStack.toString()});
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
            trace.push_back({stepNum++, token, std::string("Push operator '") + op + "'", makeOutputStr(output), opStack.toString()});
        } else {
            output.push_back(token);
            trace.push_back({stepNum++, token, std::string("Send operand '") + token + "' to output", makeOutputStr(output), opStack.toString()});
        }
    }

    while (!opStack.isEmpty()) {
        char topOp = opStack.pop();
        if (topOp != '(') {
            output.push_back(std::string(1, topOp));
        }
    }

    trace.push_back({stepNum++, "END", "Pop all remaining operators from stack", makeOutputStr(output), opStack.toString()});

    outPostfix = makeOutputStr(output);
    if (outPostfix == "(empty)") outPostfix = "";
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

    std::reverse(tokens.begin(), tokens.end());

    for (std::string& t : tokens) {
        if (t == "(") t = ")";
        else if (t == ")") t = "(";
    }

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

                if (topPrec > prec || (topPrec == prec && rightAssoc)) {
                    output.push_back(std::string(1, opStack.pop()));
                } else {
                    break;
                }
            }
            opStack.push(op);
        } else {
            output.push_back(token);
        }
    }

    while (!opStack.isEmpty()) {
        char topOp = opStack.pop();
        if (topOp != '(') output.push_back(std::string(1, topOp));
    }

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

// ============================================================================
// SECTION 4: EXPRESSION EVALUATION (POSTFIX & PREFIX)
// ============================================================================

struct EvalStep {
    int stepNumber;
    std::string token;
    std::string action;
    std::string stackContents;
};

namespace {
    std::string formatResult(double val) {
        if (std::abs(val) < 1e-12) return "0";
        std::ostringstream oss;
        oss << std::setprecision(12) << val;
        std::string s = oss.str();
        if (s.find('.') != std::string::npos) {
            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
            if (!s.empty() && s.back() == '.') {
                s.pop_back();
            }
        }
        return s;
    }

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
            act << "Pop " << formatResult(op2) << ", " << formatResult(op1) << " -> Compute " << formatResult(op1) << " " << op << " " << formatResult(op2) << " = " << formatResult(res) << " -> Push " << formatResult(res);
            outTrace.push_back({stepNum++, token, act.str(), valStack.toString()});
        } else {
            char* endPtr = nullptr;
            double val = std::strtod(token.c_str(), &endPtr);
            if (*endPtr != '\0') {
                throw std::runtime_error(std::string("Error: Invalid numeric token '") + token + "' in postfix expression.");
            }
            valStack.push(val);

            std::ostringstream act;
            act << "Push operand " << formatResult(val);
            outTrace.push_back({stepNum++, token, act.str(), valStack.toString()});
        }
    }

    if (valStack.isEmpty()) throw std::runtime_error("Error: Postfix expression is empty.");
    if (valStack.size() > 1) throw std::runtime_error("Error: Invalid postfix expression: Too many operands remaining on stack.");

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
    while (iss >> token) tokens.push_back(token);

    if (tokens.empty()) throw std::runtime_error("Error: Prefix expression is empty.");

    Stack<double> valStack(50);
    int stepNum = 1;

    // Scan tokens from Right to Left
    for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; --i) {
        const std::string& t = tokens[i];
        if (t.length() == 1 && isOperator(t[0])) {
            char op = t[0];
            if (valStack.size() < 2) {
                throw std::runtime_error(std::string("Error: Invalid prefix expression: Insufficient operands for operator '") + op + "'.");
            }
            double op1 = valStack.pop(); // left operand in right-to-left scan
            double op2 = valStack.pop(); // right operand in right-to-left scan
            double res = applyOperation(op, op1, op2);
            valStack.push(res);

            std::ostringstream act;
            act << "Pop " << formatResult(op1) << ", " << formatResult(op2) << " -> Compute " << formatResult(op1) << " " << op << " " << formatResult(op2) << " = " << formatResult(res) << " -> Push " << formatResult(res);
            outTrace.push_back({stepNum++, t, act.str(), valStack.toString()});
        } else {
            char* endPtr = nullptr;
            double val = std::strtod(t.c_str(), &endPtr);
            if (*endPtr != '\0') {
                throw std::runtime_error(std::string("Error: Invalid numeric token '") + t + "' in prefix expression.");
            }
            valStack.push(val);

            std::ostringstream act;
            act << "Push operand " << formatResult(val);
            outTrace.push_back({stepNum++, t, act.str(), valStack.toString()});
        }
    }

    if (valStack.size() != 1) throw std::runtime_error("Error: Invalid prefix expression: Operand count mismatch.");

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

// ============================================================================
// SECTION 5: ARRAY-BASED HISTORY MANAGER
// ============================================================================

struct HistoryEntry {
    int id;
    std::string infix;
    std::string postfix;
    std::string prefix;
    double result;
    bool hasResult;
    std::string timestamp;
};

class ExpressionHistory {
private:
    static const int MAX_HISTORY = 100;
    HistoryEntry entries[MAX_HISTORY];
    int count;
    int nextId;

    std::string getCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm lTime{};
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&lTime, &nowTime);
#else
        localtime_r(&nowTime, &lTime);
#endif
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &lTime);
        return std::string(buf);
    }

public:
    ExpressionHistory() : count(0), nextId(1) {}

    void addEntry(const std::string& infix, const std::string& postfix, const std::string& prefix, double result) {
        if (count < MAX_HISTORY) {
            entries[count].id = nextId++;
            entries[count].infix = infix;
            entries[count].postfix = postfix;
            entries[count].prefix = prefix;
            entries[count].result = result;
            entries[count].hasResult = true;
            entries[count].timestamp = getCurrentTimestamp();
            count++;
        } else {
            for (int i = 0; i < MAX_HISTORY - 1; ++i) {
                entries[i] = entries[i + 1];
            }
            entries[MAX_HISTORY - 1].id = nextId++;
            entries[MAX_HISTORY - 1].infix = infix;
            entries[MAX_HISTORY - 1].postfix = postfix;
            entries[MAX_HISTORY - 1].prefix = prefix;
            entries[MAX_HISTORY - 1].result = result;
            entries[MAX_HISTORY - 1].hasResult = true;
            entries[MAX_HISTORY - 1].timestamp = getCurrentTimestamp();
        }
    }

    void display() const {
        std::cout << "\n====================================================\n";
        std::cout << "                 EXPRESSION HISTORY                 \n";
        std::cout << "====================================================\n";

        if (count == 0) {
            std::cout << "\n[ No history available. No expressions evaluated yet. ]\n\n";
            return;
        }

        for (int i = 0; i < count; ++i) {
            std::cout << "\n" << (i + 1) << ".\n";
            std::cout << "Expression: " << entries[i].infix << "\n";
            std::cout << "Postfix:    " << entries[i].postfix << "\n";
            if (!entries[i].prefix.empty()) {
                std::cout << "Prefix:     " << entries[i].prefix << "\n";
            }
            std::cout << "Result:     " << formatResult(entries[i].result) << "\n";
        }
        std::cout << "\nTotal recorded entries: " << count << " / " << MAX_HISTORY << "\n\n";
    }

    void clear() {
        count = 0;
        std::cout << "\n[ History successfully cleared. ]\n\n";
    }

    int getCount() const { return count; }
};

namespace {
    ExpressionHistory globalHistory;
}

void displayHistory() {
    globalHistory.display();
}

void clearHistory() {
    globalHistory.clear();
}

void addHistoryRecord(const std::string& infix, const std::string& postfix, const std::string& prefix, double result) {
    globalHistory.addEntry(infix, postfix, prefix, result);
}

// ============================================================================
// SECTION 6: CONSOLE UI & MENU HANDLERS
// ============================================================================

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

std::string readLineTrimmed() {
    std::string line;
    std::getline(std::cin, line);
    size_t first = line.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = line.find_last_not_of(" \t\r\n");
    return line.substr(first, (last - first + 1));
}

bool askYesNo(const std::string& prompt) {
    std::cout << prompt << " (y/n): ";
    std::string response;
    std::getline(std::cin, response);
    return (!response.empty() && (response[0] == 'y' || response[0] == 'Y'));
}

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
        std::cout << "Calculated Result:  " << formatResult(result) << "\n";
        std::cout << "========================================\n";

        addHistoryRecord(expr, postfix, prefix, result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

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
        std::cout << "Calculated Result:  " << formatResult(result) << "\n";

        addHistoryRecord("[Direct Postfix]", postfix, "", result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

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
        std::cout << "Calculated Result: " << formatResult(result) << "\n";

        addHistoryRecord("[Direct Prefix]", "", prefix, result);
    } catch (const std::exception& ex) {
        std::cout << "\n[!] Evaluation Failed: " << ex.what() << "\n";
    }
}

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
    std::cout << "   - Operations:\n";
    std::cout << "       push() : O(1) amortized, O(n) worst case when resizing\n";
    std::cout << "       pop()  : O(1)\n";
    std::cout << "       peek() : O(1)\n";

    std::cout << "\n3. COMPLEXITY ANALYSIS:\n";
    std::cout << "   +---------------------------------+--------------------------------------+-----------------+\n";
    std::cout << "   | Operation                       | Time Complexity                      | Space Complexity|\n";
    std::cout << "   +---------------------------------+--------------------------------------+-----------------+\n";
    std::cout << "   | Expression Validation           | O(N)                                 | O(N)            |\n";
    std::cout << "   | Tokenization                    | O(N)                                 | O(N)            |\n";
    std::cout << "   | Infix to Postfix (Shunting-Yard)| O(N)                                 | O(N)            |\n";
    std::cout << "   | Postfix Evaluation              | O(N)                                 | O(N)            |\n";
    std::cout << "   | Infix to Prefix Conversion      | O(N)                                 | O(N)            |\n";
    std::cout << "   | Prefix Evaluation               | O(N)                                 | O(N)            |\n";
    std::cout << "   | Stack push()                    | O(1) amortized, O(N) worst (resize)  | O(1) per elem   |\n";
    std::cout << "   | Stack pop(), peek()             | O(1)                                 | O(1)            |\n";
    std::cout << "   | History insertion               | O(1) normally, O(H) when buffer full | O(H)            |\n";
    std::cout << "   | History display                 | O(H)                                 | O(1)            |\n";
    std::cout << "   +---------------------------------+--------------------------------------+-----------------+\n";
    std::cout << "   * Where N = length of expression / token count, H = history capacity (MAX_HISTORY = 100).\n\n";
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
