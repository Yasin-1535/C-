/**
 * ============================================================================
 * File: validation.h
 * Description: Header for mathematical expression syntax validation
 * Concept: Demonstrates lexical analysis, state checking, operator rules,
 *          and stack-based parenthesis balancing.
 * ============================================================================
 */

#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

/**
 * @struct ValidationResult
 * @brief Encapsulates validation status, error description, and error index.
 */
struct ValidationResult {
    bool isValid;
    std::string errorMessage;
    int errorPosition; // -1 if not position-specific
};

/**
 * @brief Checks if a character is one of the supported operators: +, -, *, /, %, ^
 * @param ch The character to inspect.
 * @return true if ch is an operator, false otherwise.
 */
bool isOperator(char ch);

/**
 * @brief Checks if a character is a decimal digit [0-9] or decimal point '.'.
 * @param ch The character to inspect.
 * @return true if ch is part of an operand literal, false otherwise.
 */
bool isOperandChar(char ch);

/**
 * @brief Checks if a character is an alphanumeric or digit operand indicator.
 */
bool isOperand(char ch);

/**
 * @brief Returns the mathematical precedence rank of an operator.
 * Precedence table:
 *   ^          : 3 (highest)
 *   *, /, %    : 2 (medium)
 *   +, -       : 1 (lowest)
 *   other      : 0
 * @param op The operator character.
 * @return Integer precedence value.
 */
int precedence(char op);

/**
 * @brief Checks whether an operator is right-associative.
 * In this system, only exponentiation ('^') is right-to-left associative.
 * All other operators (+, -, *, /, %) are left-to-right associative.
 * @param op The operator character.
 * @return true if right-associative, false if left-associative.
 */
bool isRightAssociative(char op);

/**
 * @brief Validates an expression string, returning boolean status.
 * Required signature by academic guidelines.
 * @param expression The mathematical expression to validate.
 * @return true if valid, false otherwise.
 */
bool isValidExpression(const std::string& expression);

/**
 * @brief Performs comprehensive syntactic and lexical validation of an expression.
 * Returns detailed diagnostic information including message and character index.
 * @param expression The input string.
 * @return ValidationResult structure.
 */
ValidationResult validateExpressionDetailed(const std::string& expression);

/**
 * @brief Validates whether a postfix expression has correct token format and operand counts.
 * @param postfix Space-separated postfix string.
 * @return ValidationResult structure.
 */
ValidationResult validatePostfixExpression(const std::string& postfix);

/**
 * @brief Validates whether a prefix expression has correct token format and operand counts.
 * @param prefix Space-separated prefix string.
 * @return ValidationResult structure.
 */
ValidationResult validatePrefixExpression(const std::string& prefix);

#endif // VALIDATION_H
