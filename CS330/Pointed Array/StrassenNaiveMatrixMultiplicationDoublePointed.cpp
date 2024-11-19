#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cstdlib>

using namespace std;

int** createArray(int height, int width){
    int **array = new int*[height];
    for (int i = 0; i < height; ++i) {
        array[i] = new int[width]();
    }
    return array;
}

// Helper function to fill a matrix with sample values
void fillArray(int **A, int height, int width, int hi) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            A[i][j] = rand() % hi + 0;
        }
    }
}

// Function to deallocate a matrix
void deleteMatrix(int** matrix, int height) {
    for (int i = 0; i < height; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Naive matrix multiplication
int** Naivemultiply(int **A, int **B, int height) {
    int **C = createArray(height, height);
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < height; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Add two matrices
int** add(int **A, int **B, int height) {
    int **C = createArray(height, height);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < height; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Subtract two matrices
int** subtract(int **A, int **B, int height) {
    int **C = createArray(height, height);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < height; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Recursive Strassen's algorithm
int** strassen(int** A, int** B, int height) {
    if (height <= 64) return Naivemultiply(A, B, height);

    int newSize = height / 2;
    int **C = createArray(height, height);

    int **A11 = createArray(newSize, newSize);
    int **A12 = createArray(newSize, newSize);
    int **A21 = createArray(newSize, newSize);
    int **A22 = createArray(newSize, newSize);
    int **B11 = createArray(newSize, newSize); 
    int **B12 = createArray(newSize, newSize);
    int **B21 = createArray(newSize, newSize); 
    int **B22 = createArray(newSize, newSize);

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

    int **M1 = strassen(add(A11, A22, newSize), add(B11, B22, newSize), newSize);
    int **M2 = strassen(add(A21, A22, newSize), B11, newSize);
    int **M3 = strassen(A11, subtract(B12, B22, newSize), newSize);
    int **M4 = strassen(A22, subtract(B21, B11, newSize), newSize);
    int **M5 = strassen(add(A11, A12, newSize), B22, newSize);
    int **M6 = strassen(subtract(A21, A11, newSize), add(B11, B12, newSize), newSize);
    int **M7 = strassen(subtract(A12, A22, newSize), add(B21, B22, newSize), newSize);

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + newSize] = M3[i][j] + M5[i][j];
            C[i + newSize][j] = M2[i][j] + M4[i][j];
            C[i + newSize][j + newSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    deleteMatrix(A11, newSize);
    deleteMatrix(A12, newSize);
    deleteMatrix(A21, newSize);
    deleteMatrix(A22, newSize);
    deleteMatrix(B11, newSize);
    deleteMatrix(B12, newSize);
    deleteMatrix(B21, newSize);
    deleteMatrix(B22, newSize);
    deleteMatrix(M1, newSize);
    deleteMatrix(M2, newSize);
    deleteMatrix(M3, newSize);
    deleteMatrix(M4, newSize);
    deleteMatrix(M5, newSize);
    deleteMatrix(M6, newSize);
    deleteMatrix(M7, newSize);

    return C;
}

// Main Strassen function for any size matrix
int** strassenMultiplyAnySize(int** A, int** B, int FirstHeight, int FirstWidth, int SecondWidth) {
    int maxDim = max({FirstHeight, FirstWidth, SecondWidth});
    int newSize = pow(2, ceil(log2(maxDim)));

    int **paddedA = createArray(newSize, newSize);
    int **paddedB = createArray(newSize, newSize);

    for (int i = 0; i < FirstHeight; ++i)
        for (int j = 0; j < FirstWidth; ++j)
            paddedA[i][j] = A[i][j];

    for (int i = 0; i < FirstWidth; ++i)
        for (int j = 0; j < SecondWidth; ++j)
            paddedB[i][j] = B[i][j];

    int **paddedC = strassen(paddedA, paddedB, newSize);

    deleteMatrix(paddedA, newSize);
    deleteMatrix(paddedB, newSize);

    return paddedC;
}

int main() {
    srand(time(0));
    int FirstHeight, FirstWidth, SecondWidth, hi;

    std::cout << "Enter First Matrix Height: ";
    cin >> FirstHeight;
    std::cout << "Enter First Matrix Width: ";
    cin >> FirstWidth;
    std::cout << "Enter Second Matrix Width: ";
    cin >> SecondWidth;
    std::cout << "Enter the highest number in the arrays to be: ";
    cin >> hi;
    
    int SecondHeight = FirstWidth;

    int **FirstMatrix = createArray(FirstHeight, FirstWidth);
    int **SecondMatrix = createArray(SecondHeight, SecondWidth);

    fillArray(FirstMatrix, FirstHeight, FirstWidth, hi);
    fillArray(SecondMatrix, SecondHeight, SecondWidth, hi);

    auto start = chrono::high_resolution_clock::now();

    int **ResultMatrix = strassenMultiplyAnySize(FirstMatrix, SecondMatrix, FirstHeight, FirstWidth, SecondWidth);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    //Print all three matrix
    cout << "\n\n\n Printing first matrix: " << endl;
     for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < FirstWidth; ++j) {
            cout << FirstMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n Printing second matrix: " << endl;
     for (int i = 0; i < SecondHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            cout << SecondMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n Printing results matrix: " << endl;
     for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            cout << ResultMatrix[i][j] << " ";
        }
        cout << endl;
    }

    std::cout << "\n Completed in " << duration.count() << " seconds.\n";

    // Clean up memory
    deleteMatrix(FirstMatrix, FirstHeight);
    deleteMatrix(SecondMatrix, SecondHeight);
    deleteMatrix(ResultMatrix, FirstHeight);

    return 0;
}
