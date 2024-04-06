# ML_Math_Foundations
This repo represents my journey of stepping into machine learning, this repo will include 2 files one for matrix algebra and another for probability & statistics (later in the future) (Feel free to use any code :) ) 
-the matrix algebra c++ file has more than 40 matrix operations from simple addition and assignment till inverse and lu factorization and row reduced echelon form feel free to use it the way you want 
they are all tested if there are any bugs please let me know 
feel free to use any code :)
-update:I've updated some core functions like gauss_down or previously called utri to work in both types of matrices aka the ones that have pivots on the main diagonal and the ones that have the pivots on any col and the rank is updated to calculate rank for all types of matrices 
previously i thought it was only for square matrices but i was wrong 
by tomorrow i will add the fourth and final space aka nullspace of the column space
-huge upadte : the calculations for the four subspaces aka row space ,col space , nullspace ,left nullspace are now available in the library.
 Feel free to use any code :)
-update : added fix_pivots() this function rearranges the matrix so that the rows contatining the pivots are on top and the rest of rows at bottom 
its crucial when using functions like lu_fact not really since i added permutation matrix earlier  but if you use elementary matrix from null_rows() and tried to test of elementary * matrix = rref(matrix) this sometimes isn't true since elementary matrix doesn't record the changes in rows that happens during the rref of the matrix its advisable to use after you initialize the matrix but i wouldn't force that its on you :) .
