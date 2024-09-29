#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

void generateRandomArray(std::vector<int>& arr, int size, int min = 0, int max = 10000) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr.push_back(rand() % (max - min + 1) + min);
    }
}

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    int dp = arr.size()/100; //done percentage
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
        if(i%dp == 0)
          std::cout<< i/dp << "%" << std::endl;
    }
}

void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int l, int r) {
    if (l >= r) return;

    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main() {
    std::vector<int> arr;
    int size = 100000;
    generateRandomArray(arr, size);
    
    mergeSort(arr, 0, arr.size() - 1);
    //bubbleSort(arr);
    
    std::cout << "Sorted array: \n";
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    return 0;
}

