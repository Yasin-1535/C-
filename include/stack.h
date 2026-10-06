/**
 * ============================================================================
 * File: stack.h
 * Description: Generic Array-Based Stack Data Structure Implementation
 * Concept: Demonstrates Stack (LIFO - Last In, First Out) implemented using
 *          a dynamically allocated contiguous array in C++.
 * ============================================================================
 */

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

/**
 * @class Stack
 * @brief Custom array-based generic stack implementation demonstrating DSA fundamentals.
 *
 * Internal Representation:
 * - A contiguous array of type T allocated on heap: `data`
 * - An integer index tracking the topmost element: `topIndex`
 * - Maximum capacity of the current buffer: `capacity`
 *
 * LIFO Property:
 * - Elements are inserted at `topIndex + 1` (push)
 * - Elements are removed from `topIndex` (pop)
 * - The most recently added element is always the first one to be removed.
 */
template <typename T>
class Stack {
private:
    T* data;           ///< Contiguous array storing stack elements
    int topIndex;      ///< Index of the current top element (-1 when empty)
    int capacity;      ///< Current maximum capacity of the array

    /**
     * @brief Helper function to double the capacity when the stack is full.
     * Demonstrates dynamic array growth (amortized O(1) push).
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
    /**
     * @brief Constructor initializing the stack with a specified initial capacity.
     * @param initialCapacity Starting size of the internal array (default = 100).
     */
    explicit Stack(int initialCapacity = 100)
        : topIndex(-1), capacity(initialCapacity) {
        if (capacity <= 0) {
            capacity = 10;
        }
        data = new T[capacity];
    }

    /**
     * @brief Destructor to safely release heap-allocated array memory.
     */
    ~Stack() {
        delete[] data;
    }

    /**
     * @brief Copy Constructor for deep copying another stack.
     */
    Stack(const Stack& other)
        : topIndex(other.topIndex), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i <= topIndex; ++i) {
            data[i] = other.data[i];
        }
    }

    /**
     * @brief Copy Assignment Operator for deep copy assignment.
     */
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

    /**
     * @brief Check whether the stack is empty.
     * @return true if topIndex == -1, false otherwise.
     * Time Complexity: O(1)
     */
    bool isEmpty() const {
        return topIndex == -1;
    }

    /**
     * @brief Check whether the internal array is at maximum capacity.
     * @return true if topIndex == capacity - 1, false otherwise.
     * Time Complexity: O(1)
     */
    bool isFull() const {
        return topIndex == capacity - 1;
    }

    /**
     * @brief Push an element onto the top of the stack.
     * Automatically resizes if capacity is exceeded.
     * @param element The value to be added.
     * Time Complexity: Amortized O(1)
     */
    void push(const T& element) {
        if (isFull()) {
            resize();
        }
        data[++topIndex] = element;
    }

    /**
     * @brief Remove and return the top element of the stack.
     * @throws std::underflow_error if the stack is empty.
     * @return The popped element.
     * Time Complexity: O(1)
     */
    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Attempted to pop from an empty stack.");
        }
        return data[topIndex--];
    }

    /**
     * @brief Inspect the top element without removing it.
     * @throws std::underflow_error if the stack is empty.
     * @return Reference to the top element.
     * Time Complexity: O(1)
     */
    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Attempted to peek into an empty stack.");
        }
        return data[topIndex];
    }

    /**
     * @brief Alternate alias for peek() adhering to standard C++ convention.
     */
    const T& top() const {
        return peek();
    }

    /**
     * @brief Get the current number of elements in the stack.
     * @return Integer count of active elements.
     * Time Complexity: O(1)
     */
    int size() const {
        return topIndex + 1;
    }

    /**
     * @brief Clear all elements from the stack.
     * Resets topIndex to -1.
     * Time Complexity: O(1)
     */
    void clear() {
        topIndex = -1;
    }

    /**
     * @brief Display the stack contents on standard output (from bottom to top).
     */
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

    /**
     * @brief Format stack elements as a single string for step-by-step algorithm trace.
     * Example output: "[ + * ]" or "EMPTY"
     */
    std::string toString() const {
        if (isEmpty()) {
            return "EMPTY";
        }
        std::ostringstream oss;
        for (int i = 0; i <= topIndex; ++i) {
            oss << data[i];
            if (i < topIndex) oss << " ";
        }
        return oss.str();
    }

    /**
     * @brief Export stack elements into a standard vector (for test validations).
     */
    std::vector<T> toVector() const {
        std::vector<T> result;
        for (int i = 0; i <= topIndex; ++i) {
            result.push_back(data[i]);
        }
        return result;
    }
};

#endif // STACK_H
