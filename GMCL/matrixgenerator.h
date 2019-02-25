/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MatrixGenerator.h"                  */
/*                                                                            */
/*                                                                            */
/*  Category:   Matrix Generator                                              */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - MATRIX_GENERATOR                                                        */
/*                                                                            */
/*                                                                            */
/*  MATRIX_GENERATOR takes a DSL description and processes it according to    */
/*  the parameter WhatToDo using the metafunctions MATRIX_DSL_PARSER,         */
/*  MATRIX_DSL_ASSIGN_DEFAULTS and MATRIX_ASSEMBLE_COMPONENTS.                */
/*  Possible operations are:                                                  */
/*  - assembling components (assemble_components),                            */
/*  - assigning defaults_and_assembling components (assign_defaults),         */
/*  - parsing_and_assigning defaults_and_assembling components (parse).       */
/*  do_nothing is a base class for these classes. It provides some IDs to     */
/*  enable an easy identification of the given argument.                      */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_GENERATOR_H
#define DB_MATRIX_GENERATOR_H


//******************************************************************************
//***************************** Matrix Generator *******************************
//******************************************************************************

// WhatToDo: parse | assign_defaults | assemble_components
struct do_nothing
{
   enum
   {
      do_nothing_id= -1,
      parse_id,
      assign_defaults_id,
      assemble_components_id,

      id= do_nothing_id
   };
};

struct parse               : public do_nothing {enum {id= parse_id};};
struct assign_defaults     : public do_nothing {enum {id= assign_defaults_id};};
struct assemble_components : public do_nothing {enum {id=
                                                      assemble_components_id};};


//***************************** Matrix Generator *******************************

//  The generator calls the parser, default assigner and the assembler in this
//  order and defines the MatrixType (RET) and the config description (Config).

template<class InputDSL= matrix<>, class WhatToDo= parse >
class MATRIX_GENERATOR
{
   typedef IF<EQUAL<WhatToDo::id, WhatToDo::parse_id>::RET,
               InputDSL,
               matrix<double> >::RET DSLDescription;


   typedef MATRIX_DSL_PARSER<DSLDescription>::RET ParsedDSL_;

   typedef IF<EQUAL<WhatToDo::id, WhatToDo::parse_id>::RET,
               ParsedDSL_,

           IF<EQUAL<WhatToDo::id, WhatToDo::assign_defaults_id>::RET,
               InputDSL,

           EmptyParsedDSLDescription >::RET>::RET ParsedDSL;

   typedef MATRIX_DSL_ASSIGN_DEFAULTS<ParsedDSL>::RET DSLFeatures_;

   typedef IF< EQUAL<WhatToDo::id, WhatToDo::parse_id>::RET ||
               EQUAL<WhatToDo::id, WhatToDo::assign_defaults_id>::RET,
                  DSLFeatures_,
                  InputDSL>::RET DSLFeatures;

   typedef MATRIX_ASSEMBLE_COMPONENTS<DSLFeatures> ICCL;

public:
   typedef ICCL::RET RET;
};


#endif   // DB_MATRIX_GENERATOR_H
