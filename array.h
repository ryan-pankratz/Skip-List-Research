#include <iostream>
#include <stdexcept>

int binarySearch(float a[], float k, int size) {
    int n = size;
    
    if(k>a[n-1]) return n;
    
    int l=0, r=n-1;
    
    while(l<r){
        int m=l+(r-l)/2;
        
        if(a[m]>=k){
            r=m;
        }else{
            l=m+1;
        }
    }
    return l; // smallest index with value >= k
}

void insert(float a[], float value, int index, int size) {
    // Insert at index
    for (int c = index; c < size - 1; c++) {
        a[c+1] = a[c];
    }

    a[index] = value;
}

void insort(float a[], float value, int size) {
    insert(a, value, binarySearch(a, value, size), size);
}

void erase(float a[], int index, int size) {
    for (int c = index; c < size - 1; c++) {
        a[c] = a[c + 1];
    }
}

void removeVal(float a[], float value, int size) {
    int index = binarySearch(a, value, size);
    if (a[index] == value) {
        erase(a, index, size);
    }
    else {
        throw std::invalid_argument("value is not in array");
    }

}



