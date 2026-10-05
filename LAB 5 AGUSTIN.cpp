#include <iostream>
#include <stdexcept>
#include <utility>

/**
 * @class DynamicArray
 * @brief Encapsulates a dynamic linear array with RAII memory management,
 *        bounds checking, and dynamic resizing capabilities.
 */
class DynamicArray {
private:
    int* dataArray;
    size_t currentCount;
    size_t maxCapacity;

    /**
     * @brief Resizes the heap memory buffer when capacity limit is reached.
     */
    void resize() {
        size_t newCapacity = maxCapacity * 2;
        int* temp = new int[newCapacity];

        // Copy elements to the new buffer
        for (size_t i = 0; i < currentCount; ++i) {
            temp[i] = dataArray[i];
        }

        // RAII Fix: Free previous dynamic buffer to prevent memory leaks
        delete[] dataArray;

        dataArray = temp;
        maxCapacity = newCapacity;
    }

public:
    /**
     * @brief Constructor initializing default dynamic capacity.
     */
    explicit DynamicArray(size_t initialCapacity = 5)
        : currentCount(0), maxCapacity(initialCapacity) {
        dataArray = new int[maxCapacity];
    }

    /**
     * @brief Destructor ensuring safe deallocation of allocated heap memory on exit.
     */
    ~DynamicArray() {
        delete[] dataArray;
        dataArray = nullptr;
    }

    // Rule of Three: Disable copy constructor and copy assignment to prevent dangling pointers
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    /**
     * @brief Inserts an item into the array, expanding capacity automatically if necessary.
     */
    void addItem(int val) {
        if (currentCount >= maxCapacity) {
            resize();
        }
        dataArray[currentCount] = val;
        ++currentCount;
        std::cout << "Added item: " << val << std::endl;
    }

    /**
     * @brief Removes an item at a specific index via single-pass shift logic.
     */
    void removeItemAt(int idx) {
        // Enforce strict upper and lower bounds checks
        if (idx < 0 || static_cast<size_t>(idx) >= currentCount) {
            std::cout << "Invalid index: " << idx << std::endl;
            return;
        }

        // Optimal single-pass O(n) element shift routine
        for (size_t i = static_cast<size_t>(idx); i < currentCount - 1; ++i) {
            dataArray[i] = dataArray[i + 1];
        }

        --currentCount;
        std::cout << "Item removed from index " << idx << std::endl;
    }

    /**
     * @brief Performs a linear search for a target value.
     */
    int findItem(int target) const {
        for (size_t i = 0; i < currentCount; ++i) {
            if (dataArray[i] == target) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    /**
     * @brief Displays array contents without reading past valid bounds.
     */
    void printAll() const {
        std::cout << "Current List Contents: ";
        // Corrected loop condition (i < currentCount) prevents off-by-one heap reads
        for (size_t i = 0; i < currentCount; ++i) {
            std::cout << dataArray[i] << " ";
        }
        std::cout << std::endl;
    }

    size_t getCount() const { return currentCount; }
    size_t getCapacity() const { return maxCapacity; }
};

/**
 * @brief Dynamic matrix processing function accepting custom matrix dimensions.
 */
void processMatrix(const int rows = 3, const int cols = 3) {
    int m[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int t[3][3];

    // Transpose matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            t[j][i] = m[i][j];
        }
    }

    std::cout << "Transposed Matrix printed raw:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << t[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "--- STARTING REFACTORED SUBSYSTEM ---" << std::endl;

    // Instantiation encapsulates state variables safely inside class scope
    DynamicArray list;

    list.addItem(10);
    list.addItem(20);
    list.addItem(30);
    list.addItem(40);
    list.addItem(50);
    list.addItem(60); // Safely triggers clean dynamic allocation resize

    list.printAll();

    std::cout << "Found 30 at index: " << list.findItem(30) << std::endl;

    list.removeItemAt(2);
    list.printAll();

    list.removeItemAt(99); // Upper bounds boundary test (handled safely)
    list.printAll();

    processMatrix(3, 3);

    // Destructor automatically frees dataArray upon leaving main scope (0 leaks)
    return 0;
}


