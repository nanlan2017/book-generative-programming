/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DemoMain.h"                         */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*  This file contains some examples to demonstrate how to use the matrix     */
/*  package. More information can be found in "Readme.txt" in the matrix      */
/*  folder.                                                                   */
/*                                                                            */
/*  If troubles arise compiling this file (e.g. "internal compiler error")    */
/*  try the following:                                                        */
/*  - make sure that option /Zm1000 is set at                                 */
/*      Project->Settings->C/C++ ->ProjectOptions                             */
/*  - quit the MS Developer Studio                                            */
/*  - delete the contents of the compiler-generated "Release"-folder          */
/*  - start the MS Develper Studio and try again                              */
/*                                                                            */
/*  If you do not succeed this way try splitting the file and compile only    */
/*  one example at a time.                                                    */
/*                                                                            */
/*  Note: Compiling in debug mode may not work even if compilation in release */
/*  mode runs perfectly. This may be just another bug of the VC 5.0 compiler. */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#include "GenerativeMatrix.h" // generative matrix includes
#include "DSLTypeInfo.h"      // DSL type information
#include <fstream>          // file stream operations

// Define a general rectangular matrix with element type double.
typedef MATRIX_GENERATOR<
                matrix< double,
                        structure<  rect<>
                        >
                >
            >::RET RectMatrixType;
/*
// Define a scalar matrix with 3 rows and 3 columns; scalar value is 3.4.
typedef MATRIX_GENERATOR
        <   matrix
            <  double,
               structure<  scalar<  stat_val<int_number<int, 3> >,
                                    stat_val<float_number<double, 3400> >// float#
                                 >
                        >
            >
        >::RET ScalarMatrixType;

// Define a sparse rectangular matrix using format compressed sparse row
// to store the elements
typedef MATRIX_GENERATOR
   <  matrix<  double,
               structure<  rect<dyn_val<>, dyn_val<>, CSR<> >,
                           sparse<>
                        >
            >
   >::RET SparseMatrixType;

// Define a general square matrix with static rows and columns and element 
// type int
typedef MATRIX_GENERATOR
   <  matrix<  int,
               structure<  rect  <  stat_val<int_number<int, 7> >,
                                    stat_val<int_number<int, 7> >
                                 >,
                           dense<>,
                           dyn<>
                        >,
               speed<>,
               no_checking<>
            >
   >::RET SquareMatrixType;

typedef RectMatrixType::Config::DSLFeatures DSL1;
typedef SparseMatrixType::Config::DSLFeatures DSL2;
typedef MULTIPLY_RESULT_DSL<DSL1, DSL2>::RET ResultDSL;

*/
void main()
{
   // Output file
   ofstream fout("Result.txt");

   // Define some matrices
   RectMatrixType RectMatrix1(5, 3), RectMatrix2(3, 3), RectMatrix3(3, 3);
   /*
   ScalarMatrixType ScalarMatrix(3, 3, 5, 3.4);

   // Examples for using the "parameters by name" technique to initialize a matrix
   SparseMatrixType::ArgumentType args1;
   SparseMatrixType SparseMatrix(args1.rows(3).cols(5));

   SquareMatrixType::ArgumentType args2;
   SquareMatrixType    A(args2.value(3)), B(args2.value(7)), C(args2.value(12)),
                       D(args2.value(9)), E(args2.value(17)), R;

   SparseMatrixType ResultMatrix(args1.order(5));

   // Initialization of a dense matrix
   RectMatrix1=    1,  2,  3,
                   4,  5,  6,
                   7,  8,  9,
                  10, 11, 12,
                  13, 14, 15;

   // Initialization of some elements of a sparse matrix
   SparseMatrix=  3, 0, 0,
                  7, 2, 4;

   // Addition of two matrices
   // The operands and the result matrix can have different types
   RectMatrix2= ScalarMatrix + ScalarMatrix;

   // Assign a value to the matrix element at position (0, 2)
   RectMatrix2.setElement(0, 2, 7);

   // Multiplication of two matrices
   // The two operands may be of different types, too
   RectMatrix3= ScalarMatrix * RectMatrix2;

   // Multiplication of a dense and a sparse matrix
   // To test the problem described in "Readme.txt" in chapter 4
   // simply replace the format CSR in the DSL description by COO.
   ResultMatrix= RectMatrix1 * SparseMatrix;

   // Addition of three matrices using an expression
   // The calculation is performed lazily, i. e. no temporary results are 
   // calculated.
   // Element (1 ; 1) of R is calculated adding the elements (1 ; 1) of A, C
   // and E. Element (1 ; 2) of R is calculated adding the elements (1 ; 2) 
   // of A, C, and E, and so on.
   R= A + C + E;

   // Print the results to a file
   fout << "RectMatrix1 = " << endl << RectMatrix1 << endl;
   fout << "RectMatrix2 = " << endl << RectMatrix2 << endl;
   fout << "RectMatrix3 = " << endl << RectMatrix3 << endl;
   fout << "ScalarMatrix = " << endl << ScalarMatrix << endl;
   fout << "SparseMatrix = " << endl << SparseMatrix << endl;
   fout << "ResultMatrix = " << endl << ResultMatrix << endl;
   fout << "R = " << endl << R << endl;

   // Print some elements of an expression
   // Only these elements are calculated instead of the whole resulting matrix.
   fout << "The first row of B + C + D:" << endl;
   for (int j= 0; j<7; ++j) fout << (B+C+D).getElement(0, j) << "	";
   fout << endl;

   // Get the resulting DSL of matrix multiplication manually and print it to
   // the output file (a value of -1 means that the feature is unspecified).
   fout << "Resulting DSL when a matrix of type RectMatrixType is "
           "multiplicated with";
   fout << "a matrix of type SparseMatrixType:" << endl;
   fout << DSLFeaturesInfo<ResultDSL>() << endl;

   // Print the DSL of matrix ResultMatrix
   fout << "DSL of ResultMatrix:" << endl;
   PrintDSLFeaturesInfo(fout, ResultMatrix);
   */
}