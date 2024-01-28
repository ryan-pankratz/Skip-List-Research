#include <iostream>

template <typename T, std::size_t N>
void moveKValuesToFront(T (&destination)[N], T source[], std::size_t k) {
    // Move existing values to make space for k values at the front
    for (std::size_t i = N - k - 1; i < N; --i) {
        destination[i + k] = destination[i];
    }

    // Copy k values from the source array to the front of the destination array
    for (std::size_t i = 0; i < k; ++i) {
        destination[i] = source[i];
    }
}

template <typename T, std::size_t N>
void moveKValuesToEnd(T (&destination)[N], T source[], std::size_t k, std::size_t startIndex) {
    // Move existing values towards the front
    for (std::size_t i = 0; i < startIndex; ++i) {
        destination[i] = destination[i];
    }

    // Copy k values from the source array to the specified index in the destination array
    for (std::size_t i = 0; i < k; ++i) {
        destination[startIndex + i] = source[N - k + i];
        source[N - k + i] = source[N - k + i - k];
    }
}

int main() {
    const std::size_t size = 10;
    int destination[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int source[] = {11, 12, 13, 14};

    std::cout << "Before Move:" << std::endl;
    std::cout << "Source: ";
    for (std::size_t i = 0; i < 4; ++i) {
        std::cout << source[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Destination: ";
    for (const auto& value : destination) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    moveKValuesToEnd(destination, source, 3, 2); // Start placing values at index 2

    std::cout << "After Move:" << std::endl;
    std::cout << "Source: ";
    for (std::size_t i = 0; i < 4; ++i) {
        std::cout << source[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Destination: ";
    for (const auto& value : destination) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
