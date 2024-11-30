 # Matrix Multiplication Algorithms
### Members
  - Chance Spreitzer
  - Cameron Bradley
    
## Types of Matrix Multiplication
  ### Naive
  This is the most self explanatory, it goes the long way to calculate the matricies in n^3 time complexity. It computes the results by iterating through rows of the first matrix and columns of the second. Now it is as simple as basic matrix multiplication but it does have a larger cost for larger matrices. 
      
  ### Strassen
  This way cuts out one of the 8 operations making it slightly faster with an approximate n^2.806 time complexity

  ## The Effects of Memory on the Programs
  
  ### Allocation Time
  The biggest problem that arises is the time it takes. While the naive doesn't have an exponential problem when the matrixes get bigger the Strassen does. This is because as the Strassen algorithm gets smaller and smaller it keeps making more arrays to fit the smaller sets of data into. This results in the time Stressen algorithm taking way longer (especially on the big matrices) because of how many arrays it has to make.
  <br>
  <br> [Naive as Vertex](CS330/Vector/NaiveMatrixMultiplicationVector.cpp)
  <br> [Strassen as Vertex](CS330/Pointed-Array/StressenMatrixMultiplicationDoublePointed.cpp)
  <br>
  
  ### Array Type
  Another problem that I found while doing this is how much slower using C++ Vectors were compared to double pointed integer arrays. I found that while you have to be more careful with the pointed arrays they are faster than using the vectors which is why there are two categories of folders that both contain the same programs just different array types.
  <br>
  <br> [Vector Folder](CS330/Vector/)
  <br> [Double Pointed Integer Array Folder](CS330/Pointed-Array/)
  <br>

## Results of these problems
  ### Using a combination
  Because of the memory restrictions the goal was to still try to use the Strassen algroithm to take advantage of it's better time complexity while still allowing the program to be faster than just the naive. Currently in the program there is a cutoff (Where it switches frrom Strassen to Naive) at <= 64x64 matrix size. I picked this size arbitrarily so I think this would also be a good thing to record some data on and see how it affects the time by moving it up or down.
  <br>
  <br> [Strassen Naive Mix Vectored](CS330/Vectored/StressenNaiveMatrixMultiplicationVectored.cpp)
  <br> [Strassen Naive Mix Pointed](CS330/Pointed-Array/StressenNaiveMatrixMultiplicationDoublePointed.cpp)
  <br>
  <br>
  <br> Please feel free to modify this as you please. Whether to add to it or make it look different I just put this here to hopefully make these programs a little more clear and why things were done the way they were.
