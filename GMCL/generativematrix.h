/******************************************************************************/
/*                                                                            */
/*   GGGG  EEEEE  N    N  EEEEE  RRRR      AA     TTTTTTT  I  V      V  EEEE  */
/*  G      E      NN   N  E      R  RR    A  A       T     I  V      V  E     */
/*  G  GG  EEE    N NN N  EEE    RRR     A    A      T     I   V    V   EEE   */
/*  G   G  E      N   NN  E      R RR   AAAAAAAA     T     I    V  V    E     */
/*   GGGG  EEEEE  N    N  EEEEE  R  RR  AA    AA     T     I     VV     EEEE  */
/*                                                                            */
/*                                                                            */
/*                   MM   MM     AA     TTTTTTT  RRRR   I  X   X              */
/*                   M M M M    A  A       T     R  RR  I   X X               */
/*                   M  M  M   A    A      T     RRR    I    X                */
/*                   M     M  AAAAAAAA     T     R RR   I   X X               */
/*                   M     M  AA    AA     T     R  RR  I  X   X              */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*  This general include file includes all the necessary files of the         */
/*  generative matrix package in the appropriate order.                       */
/*  Some variants are implemented in subfolders. Each of these subfolders     */
/*  contains its own GenerativeMatrix.h file to include the necessary header  */
/*  files for the respective variant. Some of the alternatives are mentioned  */
/*  here in comments.                                                         */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

// disable some warnings that often arise with generative programming
#pragma warning( disable : 4786)
#pragma warning( disable : 4660)
#pragma warning( disable : 4503)


// helper classes
#include <iostream>
#include <assert.h>
#include "IF.h"
#include "equal.h"
#include "promote.h"
#include "MaxMin.h"


// DSL
#include "DSL.h"


// ICCL declaration
#include "ICCL.h"


// Matrix generator

// either MatrixGenerator (consisting of DSLParser, DSLAssignDefaults,
// DSL to ICCL and MatrixGenerator) ...
#include "DSLParser.h"
#include "DSLAssignDefaults.h"
#include "DSL to ICCL.h"
#include "MatrixGenerator.h"

// ... or CompactMatrixGenerator
// #include "CompactGenerator\CompactMatrixGenerator.h"


// ICCL components
#include "MemoryAllocErrorNotifier.h"
#include "Containers.h"
#include "ScalarValue.h"
#include "Diags.h"
#include "Ext.h"
#include "Dictionaries.h"
#include "Formats.h"
#include "BoundsChecker.h"
#include "TopWrapper.h"
#include "CompatChecker.h"
#include "CommaInitializer.h"


// operations
#include "MatrixAssignment.h"
#include "MatrixTypePromotion.h"

// either MatrixLazyOperations or MatrixOperations or MatrixSimpleLazyOperations
#include "MatrixLazyOperations.h"
// #include "NotLazy/MatrixOperations.h"
// #include "SimpleLazy/MatrixSimpleLazyOperations.h"
