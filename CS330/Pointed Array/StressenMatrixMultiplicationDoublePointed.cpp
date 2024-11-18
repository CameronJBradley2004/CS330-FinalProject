#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Helper function to create an empty matrix of given size
int** createMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols]();
    }
    return matrix;
}

// Function to delete a matrix
void deleteMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Add two matrices
int** add(int** A, int** B, int size) {
    int** C = createMatrix(size, size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Subtract two matrices
int** subtract(int** A, int** B, int size) {
    int** C = createMatrix(size, size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Recursive Strassen's algorithm
int** strassen(int** A, int** B, int size) {
    if (size == 1) {
        int** C = createMatrix(1, 1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = size / 2;
    int** A11 = createMatrix(newSize, newSize);
    int** A12 = createMatrix(newSize, newSize);
    int** A21 = createMatrix(newSize, newSize);
    int** A22 = createMatrix(newSize, newSize);
    int** B11 = createMatrix(newSize, newSize);
    int** B12 = createMatrix(newSize, newSize);
    int** B21 = createMatrix(newSize, newSize);
    int** B22 = createMatrix(newSize, newSize);

    // Divide matrices into submatrices
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Calculate 7 products using Strassen’s algorithm
    int** M1 = strassen(add(A11, A22, newSize), add(B11, B22, newSize), newSize);
    int** M2 = strassen(add(A21, A22, newSize), B11, newSize);
    int** M3 = strassen(A11, subtract(B12, B22, newSize), newSize);
    int** M4 = strassen(A22, subtract(B21, B11, newSize), newSize);
    int** M5 = strassen(add(A11, A12, newSize), B22, newSize);
    int** M6 = strassen(subtract(A21, A11, newSize), add(B11, B12, newSize), newSize);
    int** M7 = strassen(subtract(A12, A22, newSize), add(B21, B22, newSize), newSize);

    // Combine products into result matrix
    int** C = createMatrix(size, size);
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + newSize] = M3[i][j] + M5[i][j];
            C[i + newSize][j] = M2[i][j] + M4[i][j];
            C[i + newSize][j + newSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    // Free memory for submatrices
    deleteMatrix(A11, newSize); deleteMatrix(A12, newSize);
    deleteMatrix(A21, newSize); deleteMatrix(A22, newSize);
    deleteMatrix(B11, newSize); deleteMatrix(B12, newSize);
    deleteMatrix(B21, newSize); deleteMatrix(B22, newSize);
    deleteMatrix(M1, newSize); deleteMatrix(M2, newSize);
    deleteMatrix(M3, newSize); deleteMatrix(M4, newSize);
    deleteMatrix(M5, newSize); deleteMatrix(M6, newSize);
    deleteMatrix(M7, newSize);

    return C;
}

// Function to pad a matrix to the next power of 2
int** padMatrix(int** A, int rows, int cols, int newSize) {
    int** padded = createMatrix(newSize, newSize);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            padded[i][j] = A[i][j];
    return padded;
}

// Function to unpad a matrix back to original size
int** unpadMatrix(int** A, int originalRows, int originalCols) {
    int** unpadded = createMatrix(originalRows, originalCols);
    for (int i = 0; i < originalRows; ++i)
        for (int j = 0; j < originalCols; ++j)
            unpadded[i][j] = A[i][j];
    return unpadded;
}

// Main Strassen function for any size matrix
int** strassenMultiplyAnySize(int** A, int** B, int FirstHeight, int FirstWidth, int SecondWidth) {
    int maxDim = max({FirstHeight, FirstWidth, SecondWidth});
    int newSize = pow(2, ceil(log2(maxDim)));

    int** paddedA = padMatrix(A, FirstHeight, FirstWidth, newSize);
    int** paddedB = padMatrix(B, FirstWidth, SecondWidth, newSize);

    int** paddedC = strassen(paddedA, paddedB, newSize);

    int** result = unpadMatrix(paddedC, FirstHeight, SecondWidth);

    // Free padded matrices
    deleteMatrix(paddedA, newSize);
    deleteMatrix(paddedB, newSize);
    deleteMatrix(paddedC, newSize);

    return result;
}

int main() {
    srand(time(0));
    int FirstHeight, FirstWidth, SecondWidth, hi;

    cout << "Enter the height of the first matrix: ";
    cin >> FirstHeight;
    cout << "Enter the width of the first matrix: ";
    cin >> FirstWidth;
    cout << "Enter the width of the second matrix: ";
    cin >> SecondWidth;
    cout << "Enter the highest number in the arrays to be: ";
    cin >> hi;
    int SecondHeight = FirstWidth;
    
    int** A = createMatrix(FirstHeight, FirstWidth);
    int** B = createMatrix(FirstWidth, SecondWidth);

    // Initialize matrices A and B
    for (int i = 0; i < FirstHeight; ++i)
        for (int j = 0; j < FirstWidth; ++j)
            A[i][j] = rand() % hi + 0;  // Sample values for A
    for (int i = 0; i < FirstWidth; ++i)
        for (int j = 0; j < SecondWidth; ++j)
            B[i][j] = rand() % hi + 0;  // Sample values for B

    auto start = chrono::high_resolution_clock::now();
    int** C = strassenMultiplyAnySize(A, B, FirstHeight, FirstWidth, SecondWidth);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    //Print all three matrix
    cout << "\n\n\n Printing first matrix: " << endl;
     for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < FirstWidth; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n Printing second matrix: " << endl;
     for (int i = 0; i < SecondHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n Printing results matrix: " << endl;
     for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Matrix multiplication completed in " << duration.count() << " seconds.\n";

    // Free matrices
    deleteMatrix(A, FirstHeight);
    deleteMatrix(B, FirstWidth);
    deleteMatrix(C, FirstHeight);

    return 0;
}
