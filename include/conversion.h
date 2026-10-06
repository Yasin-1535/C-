/**
 * ============================================================================
 * File: conversion.h
 * Description: Header for Infix to Postfix and Infix to Prefix conversions
 * Concept: Demonstrates Shunting-Yard Algorithm using custom array-based stack,
 *          operator precedence, and associativity.
 * ============================================================================
 */

#ifndef CONVERSION_H
#define CONVERSION_H

#include <string>
#include <vector>

/**
 * @struct TraceStep
 * @brief Represents a single step in the step-by-step educational trace.
 */
struct TraceStep {
    int stepNumber;
    std::string token;
    std::string action;
    std::string outputBuffer;
    std::string stackContents;
};

/**
 * @brief Tokenizes an expression string into distinct lexical tokens.
 * Correctly distinguishes multi-digit numbers, decimals, operators, and parentheses.
 * Handles unary minus / plus preceding numbers.
 * @param expression The raw input string.
 * @return Vector of individual string tokens.
 */
std::vector<std::string> tokenizeExpression(const std::string& expression);

/**
 * @brief Converts an infix mathematical expression to postfix (Reverse Polish) notation.
 * Tokens in the output string are space-separated.
 * Uses custom Stack<char> and Shunting-Yard algorithm.
 * @param expression The infix string.
 * @return Space-separated postfix string.
 */
std::string infixToPostfix(const std::string& expression);

/**
 * @brief Converts infix expression to postfix while recording detailed trace information.
 * Used for educational demonstrations and viva explanations.
 * @param expression The infix string.
 * @param outPostfix Reference where final postfix string is stored.
 * @return Vector of TraceStep records.
 */
std::vector<TraceStep> infixToPostfixWithTrace(const std::string& expression, std::string& outPostfix);

/**
 * @brief Converts an infix mathematical expression to prefix (Polish) notation.
 * Uses stack-based token-reversal algorithm with adapted operator associativity.
 * Tokens in output are space-separated.
 * @param expression The infix string.
 * @return Space-separated prefix string.
 */
std::string infixToPrefix(const std::string& expression);

/**
 * @brief Formats and displays the step-by-step conversion trace table to console.
 * @param trace Vector of trace records.
 */
void displayTraceTable(const std::vector<TraceStep>& trace);

#endif // CONVERSION_H
