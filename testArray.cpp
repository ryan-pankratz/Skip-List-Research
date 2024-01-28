#include <iostream>

// Function to insert a value at a specific index in an array
template <typename T, std::size_t N>
void insert(T (&a)[N], const T& value, std::size_t index, std::size_t size) {
    // Insert at index
    for (std::size_t c = size; c > index; --c) {
        a[c] = a[c - 1];
    }

    a[index] = value;
}

// Function to remove a value at a specific index from an array
template <typename T, std::size_t N>
void remove(T (&a)[N], std::size_t index, std::size_t size) {
    for (std::size_t c = index; c < size - 1; ++c) {
        a[c] = a[c + 1];
    }
}

int main() {
    const std::size_t size = 10;
    int a[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Before Insert:" << std::endl;
    for (const auto& value : a) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    insert(a, 99, 3, size);

    std::cout << "After Insert:" << std::endl;
    for (const auto& value : a) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    remove(a, 5, size);

    std::cout << "After Remove:" << std::endl;
    for (const auto& value : a) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}