#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>

using namespace std;

typedef int** Matrix;

// Helper function to create an empty matrix of given size
Matrix createMatrix(int rows, int cols) {
    Matrix matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

// Helper function to delete a matrix
void deleteMatrix(Matrix matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Add two matrices
Matrix add(const Matrix A, const Matrix B, int size) {
    Matrix C = createMatrix(size, size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Subtract two matrices
Matrix subtract(const Matrix A, const Matrix B, int size) {
    Matrix C = createMatrix(size, size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Recursive Strassen's algorithm
Matrix strassen(const Matrix A, const Matrix B, int size) {
    if (size == 1) {
        Matrix C = createMatrix(1, 1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = size / 2;
    Matrix A11 = createMatrix(newSize, newSize), A12 = createMatrix(newSize, newSize),
           A21 = createMatrix(newSize, newSize), A22 = createMatrix(newSize, newSize);
    Matrix B11 = createMatrix(newSize, newSize), B12 = createMatrix(newSize, newSize),
           B21 = createMatrix(newSize, newSize), B22 = createMatrix(newSize, newSize);

    // Divide matrices into 4 submatrices
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

    // Calculate the 7 products using Strassen’s formula
    Matrix M1 = strassen(add(A11, A22, newSize), add(B11, B22, newSize), newSize);
    Matrix M2 = strassen(add(A21, A22, newSize), B11, newSize);
    Matrix M3 = strassen(A11, subtract(B12, B22, newSize), newSize);
    Matrix M4 = strassen(A22, subtract(B21, B11, newSize), newSize);
    Matrix M5 = strassen(add(A11, A12, newSize), B22, newSize);
    Matrix M6 = strassen(subtract(A21, A11, newSize), add(B11, B12, newSize), newSize);
    Matrix M7 = strassen(subtract(A12, A22, newSize), add(B21, B22, newSize), newSize);

    // Combine the 7 products into the result matrix C
    Matrix C = createMatrix(size, size);
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + newSize] = M3[i][j] + M5[i][j];
            C[i + newSize][j] = M2[i][j] + M4[i][j];
            C[i + newSize][j + newSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    // Free memory for submatrices and intermediate products
    deleteMatrix(A11, newSize); deleteMatrix(A12, newSize); deleteMatrix(A21, newSize); deleteMatrix(A22, newSize);
    deleteMatrix(B11, newSize); deleteMatrix(B12, newSize); deleteMatrix(B21, newSize); deleteMatrix(B22, newSize);
    deleteMatrix(M1, newSize); deleteMatrix(M2, newSize); deleteMatrix(M3, newSize); deleteMatrix(M4, newSize);
    deleteMatrix(M5, newSize); deleteMatrix(M6, newSize); deleteMatrix(M7, newSize);

    return C;
}

// Function to pad matrix to the next power of 2
Matrix padMatrix(const Matrix A, int originalSize, int newSize) {
    Matrix padded = createMatrix(newSize, newSize);
    for (int i = 0; i < originalSize; ++i)
        for (int j = 0; j < originalSize; ++j)
            padded[i][j] = A[i][j];
    return padded;
}

// Function to unpad matrix back to original size
Matrix unpadMatrix(const Matrix A, int originalRows, int originalCols) {
    Matrix unpadded = createMatrix(originalRows, originalCols);
    for (int i = 0; i < originalRows; ++i)
        for (int j = 0; j < originalCols; ++j)
            unpadded[i][j] = A[i][j];
    return unpadded;
}

// Function to print a matrix
void printMatrix(const Matrix M, int rows, int cols, const string &name) {
    cout << name << " matrix:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

// Main Strassen function for any size matrix
Matrix strassenMultiplyAnySize(const Matrix A, const Matrix B, int rowsA, int colsA, int colsB) {
    int maxDim = max({rowsA, colsA, colsB});
    int newSize = pow(2, ceil(log2(maxDim))); // Smallest power of 2 greater than maxDim

    Matrix paddedA = padMatrix(A, rowsA, newSize);
    Matrix paddedB = padMatrix(B, colsA, newSize);

    Matrix paddedC = strassen(paddedA, paddedB, newSize);

    Matrix C = unpadMatrix(paddedC, rowsA, colsB);

    deleteMatrix(paddedA, newSize);
    deleteMatrix(paddedB, newSize);
    deleteMatrix(paddedC, newSize);

    return C;
}

// Main function
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

    Matrix A = createMatrix(FirstHeight, FirstWidth);
    Matrix B = createMatrix(FirstWidth, SecondWidth);

    // Initialize A and B with some values (you can replace this with manual input)
    for (int i = 0; i < FirstHeight; ++i)
        for (int j = 0; j < FirstWidth; ++j)
            A[i][j] = rand() % hi + 0;  // Sample values for A
    for (int i = 0; i < FirstWidth; ++i)
        for (int j = 0; j < SecondWidth; ++j)
            B[i][j] = rand() % hi + 0;  // Sample values for B

    // Start timer
    auto start = chrono::high_resolution_clock::now();

    // Calculate Matrix C (result)
    Matrix C = strassenMultiplyAnySize(A, B, FirstHeight, FirstWidth, SecondWidth);

    // Stop timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // Print the matrices
    printMatrix(A, FirstHeight, FirstWidth, "First");
    printMatrix(B, FirstWidth, SecondWidth, "Second");
    printMatrix(C, FirstHeight, SecondWidth, "Results");
    cout << "Matrix multiplication completed in " << duration.count() << " seconds.\n";

    deleteMatrix(A, FirstHeight);
    deleteMatrix(B, FirstWidth);
    deleteMatrix(C, FirstHeight);

    return 0;
}
