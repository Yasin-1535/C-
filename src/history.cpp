/**
 * ============================================================================
 * File: history.cpp
 * Description: Implementation of Expression History Management using Array
 * ============================================================================
 */

#include "../include/history.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace {
    ExpressionHistory globalHistoryInstance;
}

std::string ExpressionHistory::getCurrentTimestamp() const {
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

ExpressionHistory::ExpressionHistory() : count(0), nextId(1) {}

void ExpressionHistory::addEntry(const std::string& infix, const std::string& postfix, const std::string& prefix, double result) {
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
        // Shift left by 1 to discard oldest entry (sliding window in fixed array)
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

void ExpressionHistory::display() const {
    std::cout << "\n====================================================\n";
    std::cout << "                 EXPRESSION HISTORY                 \n";
    std::cout << "====================================================\n";

    if (count == 0) {
        std::cout << "\n[ No history available. No expressions evaluated yet. ]\n\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        std::cout << "\n[" << entries[i].id << "] " << entries[i].timestamp << "\n";
        std::cout << "  Expression: " << entries[i].infix << "\n";
        std::cout << "  Postfix:    " << entries[i].postfix << "\n";
        if (!entries[i].prefix.empty()) {
            std::cout << "  Prefix:     " << entries[i].prefix << "\n";
        }
        std::cout << "  Result:     " << std::setprecision(10) << entries[i].result << "\n";
        std::cout << "----------------------------------------------------\n";
    }
    std::cout << "Total recorded entries: " << count << " / " << MAX_HISTORY << "\n\n";
}

void ExpressionHistory::clear() {
    count = 0;
    std::cout << "\n[ History successfully cleared. ]\n\n";
}

int ExpressionHistory::getCount() const {
    return count;
}

void displayHistory() {
    globalHistoryInstance.display();
}

void clearHistory() {
    globalHistoryInstance.clear();
}

void addHistoryRecord(const std::string& infix, const std::string& postfix, const std::string& prefix, double result) {
    globalHistoryInstance.addEntry(infix, postfix, prefix, result);
}
