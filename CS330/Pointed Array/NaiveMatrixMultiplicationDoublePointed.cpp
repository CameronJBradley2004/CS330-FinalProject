#include <iostream>
#include <chrono>
#include <cstdlib>
using namespace std;

void multiply(int **A, int **B, int **C, int height, int match, int width) {
for(int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
        C[i][j] = 0;
        for (int k = 0; k < match; k++)
        {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
}



int main() {
    srand(time(0));
    int FirstHeight, FirstWidth, SecondHeight, SecondWidth, hi;

    cout << "\nEnter the height of first matrix: ";
    cin >> FirstHeight;
    cout << "\nEnter the width of first matrix: ";
    cin >> FirstWidth;
    cout << "\nEnter the width of second matrix: ";
    cin >> SecondWidth;
    cout << "Enter the highest number in the arrays to be: ";
    cin >> hi;

    SecondHeight = FirstWidth;

    //Generate rows for the matrix using pointers and the input
    int **FirstMatrix = new int*[FirstHeight];
    int **SecondMatrix = new int*[SecondHeight];
    int **ResultMatrix = new int*[FirstHeight];

    //Generate columns based on the pointed rows and the input
    for (int i = 0; i < FirstHeight; ++i) {
        FirstMatrix[i] = new int[FirstWidth];
    }
    for (int i = 0; i < SecondHeight; ++i) {
        SecondMatrix[i] = new int[SecondWidth];
    }
    for (int i = 0; i < FirstHeight; ++i) {
        ResultMatrix[i] = new int[SecondWidth];
    }

    //Populate all the matrix with generated numbers for time being
    for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < FirstWidth; ++j) {
            FirstMatrix[i][j] = rand() % hi + 0;
        }
    }
    for (int i = 0; i < SecondHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            SecondMatrix[i][j] = rand() % hi + 0;
        }
    }
    for (int i = 0; i < FirstHeight; ++i) {
        for (int j = 0; j < SecondWidth; ++j) {
            ResultMatrix[i][j] = 0;
        }
    }

    // Start timer
    auto start = chrono::high_resolution_clock::now();
    
    multiply(FirstMatrix, SecondMatrix, ResultMatrix, FirstHeight, FirstWidth, SecondWidth);
    
    // Stop timer
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
    
    cout << "Matrix multiplication completed in " << duration.count() << " seconds.\n";    

    //Free up the memory from the arrays
    delete [] FirstMatrix;
    delete [] SecondMatrix;
    delete [] ResultMatrix;

    return 0;
}

