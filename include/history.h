/**
 * ============================================================================
 * File: history.h
 * Description: Header for Expression History Management
 * Concept: Demonstrates Array-based records storage, circular buffer / linear
 *          list, and history audit log.
 * ============================================================================
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <string>

/**
 * @struct HistoryEntry
 * @brief Represents an evaluated mathematical expression record.
 */
struct HistoryEntry {
    int id;
    std::string infix;
    std::string postfix;
    std::string prefix;
    double result;
    bool hasResult;
    std::string timestamp;
};

/**
 * @class ExpressionHistory
 * @brief Array-based history storage system for expression evaluations.
 *
 * Demonstrates:
 * - Contiguous array allocation
 * - Linear history logging
 * - Capacity bounds and truncation/growth
 */
class ExpressionHistory {
private:
    static const int MAX_HISTORY = 100;
    HistoryEntry entries[MAX_HISTORY];
    int count;
    int nextId;

    std::string getCurrentTimestamp() const;

public:
    ExpressionHistory();

    /**
     * @brief Adds a new evaluated expression to the history log.
     * @param infix Original infix expression.
     * @param postfix Converted postfix expression.
     * @param prefix Converted prefix expression.
     * @param result Numerical evaluation result.
     */
    void addEntry(const std::string& infix, const std::string& postfix, const std::string& prefix, double result);

    /**
     * @brief Displays the formatted history log on console.
     */
    void display() const;

    /**
     * @brief Clears all stored expression history.
     */
    void clear();

    /**
     * @brief Get the number of stored history entries.
     */
    int getCount() const;
};

/**
 * @brief Global or singleton wrapper functions matching project specifications.
 */
void displayHistory();
void clearHistory();
void addHistoryRecord(const std::string& infix, const std::string& postfix, const std::string& prefix, double result);

#endif // HISTORY_H
