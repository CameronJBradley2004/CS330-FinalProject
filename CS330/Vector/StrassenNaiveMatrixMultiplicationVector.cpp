#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

using namespace std;

typedef vector<vector<int>> Matrix;

Matrix createMatrix(int, int);
Matrix Naivemultiply(const Matrix&, const Matrix&);
Matrix add(const Matrix&, const Matrix&);
Matrix subtract(const Matrix&, const Matrix&);
Matrix strassen(const Matrix&, const Matrix&);
Matrix padMatrix(const Matrix&, int);
Matrix unpadMatrix(const Matrix&, int, int);
void printMatrix(const Matrix&, const string&);
Matrix strassenMultiplyAnySize(const Matrix&, const Matrix&);

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

    for (int i = 0; i < FirstHeight; ++i)
        for (int j = 0; j < FirstWidth; ++j)
            A[i][j] = rand() % hi;

    for (int i = 0; i < FirstWidth; ++i)
        for (int j = 0; j < SecondWidth; ++j)
            B[i][j] = rand() % hi;

    auto start = chrono::high_resolution_clock::now();
    Matrix C = strassenMultiplyAnySize(A, B);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    printMatrix(A, "First");
    printMatrix(B, "Second");
    printMatrix(C, "Results");

    cout << "Matrix multiplication completed in " << duration.count() << " seconds.\n";

    return 0;
}

Matrix createMatrix(int rows, int cols) {
    return Matrix(rows, vector<int>(cols, 0));
}

Matrix Naivemultiply(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C = createMatrix(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

Matrix add(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C = createMatrix(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

Matrix subtract(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C = createMatrix(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

Matrix strassen(const Matrix &A, const Matrix &B) {
    int n = A.size();
    if (n == 1) {
        return Matrix{{A[0][0] * B[0][0]}};
    }
    if (n > 256) {
        int newSize = n / 2;
        Matrix A11 = createMatrix(newSize, newSize), A12 = createMatrix(newSize, newSize),
               A21 = createMatrix(newSize, newSize), A22 = createMatrix(newSize, newSize);
        Matrix B11 = createMatrix(newSize, newSize), B12 = createMatrix(newSize, newSize),
               B21 = createMatrix(newSize, newSize), B22 = createMatrix(newSize, newSize);

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

        Matrix M1 = strassen(add(A11, A22), add(B11, B22));
        Matrix M2 = strassen(add(A21, A22), B11);
        Matrix M3 = strassen(A11, subtract(B12, B22));
        Matrix M4 = strassen(A22, subtract(B21, B11));
        Matrix M5 = strassen(add(A11, A12), B22);
        Matrix M6 = strassen(subtract(A21, A11), add(B11, B12));
        Matrix M7 = strassen(subtract(A12, A22), add(B21, B22));

        Matrix C = createMatrix(n, n);
        for (int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
                C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
                C[i][j + newSize] = M3[i][j] + M5[i][j];
                C[i + newSize][j] = M2[i][j] + M4[i][j];
                C[i + newSize][j + newSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
            }
        }
        return C;
    } else {
        return Naivemultiply(A, B);
    }
}

Matrix padMatrix(const Matrix &A, int newSize) {
    Matrix padded = createMatrix(newSize, newSize);
    for (int i = 0; i < A.size(); ++i)
        for (int j = 0; j < A[0].size(); ++j)
            padded[i][j] = A[i][j];
    return padded;
}

Matrix unpadMatrix(const Matrix &A, int originalRows, int originalCols) {
    Matrix unpadded = createMatrix(originalRows, originalCols);
    for (int i = 0; i < originalRows; ++i)
        for (int j = 0; j < originalCols; ++j)
            unpadded[i][j] = A[i][j];
    return unpadded;
}

void printMatrix(const Matrix &M, const string &name) {
    cout << name << " matrix:\n";
    for (const auto &row : M) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

Matrix strassenMultiplyAnySize(const Matrix &A, const Matrix &B) {
    int maxDim = max({A.size(), A[0].size(), B.size(), B[0].size()});
    int newSize = pow(2, ceil(log2(maxDim)));

    Matrix paddedA = padMatrix(A, newSize);
    Matrix paddedB = padMatrix(B, newSize);

    Matrix paddedC = strassen(paddedA, paddedB);

    return unpadMatrix(paddedC, A.size(), B[0].size());
}
