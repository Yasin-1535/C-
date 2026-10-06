/**
 * ============================================================================
 * File: evaluation.h
 * Description: Header for Postfix and Prefix expression evaluation using Stack
 * Concept: Demonstrates Stack-based expression evaluation, operand ordering,
 *          mathematical operations, and numerical error handling.
 * ============================================================================
 */

#ifndef EVALUATION_H
#define EVALUATION_H

#include <string>
#include <vector>

/**
 * @struct EvalStep
 * @brief Represents a single execution step during expression evaluation.
 */
struct EvalStep {
    int stepNumber;
    std::string token;
    std::string action;
    std::string stackContents;
};

/**
 * @brief Evaluates a postfix (Reverse Polish) expression.
 * Uses custom Stack<double> for operand management.
 * Throws std::runtime_error on division-by-zero or malformed expression.
 * @param postfix Space-separated postfix tokens.
 * @return Calculated numerical result (double).
 */
double evaluatePostfix(const std::string& postfix);

/**
 * @brief Evaluates a postfix expression while recording educational trace steps.
 * @param postfix Space-separated postfix tokens.
 * @param outTrace Reference where trace steps are recorded.
 * @return Calculated numerical result (double).
 */
double evaluatePostfixWithTrace(const std::string& postfix, std::vector<EvalStep>& outTrace);

/**
 * @brief Evaluates a prefix (Polish) expression.
 * Scans tokens from right to left using custom Stack<double>.
 * Throws std::runtime_error on division-by-zero or malformed expression.
 * @param prefix Space-separated prefix tokens.
 * @return Calculated numerical result (double).
 */
double evaluatePrefix(const std::string& prefix);

/**
 * @brief Evaluates a prefix expression while recording educational trace steps.
 * @param prefix Space-separated prefix tokens.
 * @param outTrace Reference where trace steps are recorded.
 * @return Calculated numerical result (double).
 */
double evaluatePrefixWithTrace(const std::string& prefix, std::vector<EvalStep>& outTrace);

/**
 * @brief Displays evaluation trace steps as a formatted console table.
 * @param trace Vector of EvalStep records.
 */
void displayEvalTraceTable(const std::vector<EvalStep>& trace);

#endif // EVALUATION_H
