/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DSL TypeInfo.h"                     */
/*                                                                            */
/*                                                                            */
/*  Category:   encore                                                        */
/*                                                                            */
/*  Operations:                                                               */
/*  - operator<<()                                                            */
/*  - PrintDSLFeaturesInfo                                                    */
/*                                                                            */
/*  This file can be used to retrieve the DSL information from a matrix       */
/*  variable (use PrintDSLFeaturesInfo) or from a DSL description (use        */
/*  operator<<() in combination with the DSLFeaturesInfo), and to send it     */
/*  to a file or to cout.                                                     */
/*                                                                            */
/*  examples:                                                                 */
/*                                                                            */
/*  cout << DSLFeaturesInfo<MyMatrixType::Config::DSLFeatures>() << endl;     */
/*  PrintDSLFeaturesInfo(cout, MyMatrix);                                     */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DSLTYPEINFO_H
#define DB_MATRIX_DSLTYPEINFO_H

#include <typeinfo.h>

using namespace MatrixDSL;

template<class AFeature>
struct DSLFeatureInfo
{
   typedef AFeature DSLFeature;

   static ostream& display(ostream& out)
   {
      switch (DSLFeature::id)
      {
         case DSLFeature::unspecified_DSL_feature_id:
                          out << "unspecified_DSL_feature"; break;
         case DSLFeature::structure_id: out << "structure"; break;

         // Shape IDs:
         case DSLFeature::rect_id:      out << "rect";      break;
         case DSLFeature::diag_id:      out << "diag";      break;
         case DSLFeature::scalar_id:    out << "scalar";    break;
         case DSLFeature::ident_id:     out << "ident";     break;
         case DSLFeature::zero_id:      out << "zero";      break;
         case DSLFeature::lower_triang_id: out << "lower_triang"; break;
         case DSLFeature::upper_triang_id: out << "upper_triang"; break;
         case DSLFeature::symm_id:      out << "symm";      break;
         case DSLFeature::band_diag_id: out << "band_diag"; break;
         case DSLFeature::lower_band_triang_id: out << "lower_band_triang"; break;
         case DSLFeature::upper_band_triang_id: out << "upper_band_triang"; break;

         // Format IDs
         case DSLFeature::array_id:  out << "array";  break;
         case DSLFeature::vector_id: out << "vector"; break;
         case DSLFeature::CSR_id:    out << "CSR";    break;
         case DSLFeature::CSC_id:    out << "CSC";    break;
         case DSLFeature::COO_id:    out << "COO";    break;
         case DSLFeature::DIA_id:    out << "DIA";    break;
         case DSLFeature::SKY_id:    out << "SKY";    break;

         // ArrOrder IDs
         case DSLFeature::c_like_id:       out << "c_like";       break;
         case DSLFeature::fortran_like_id: out << "fortran_like"; break;

         // Dictionary IDs
         case DSLFeature::hash_dict_id: out << "hash_dict"; break;
         case DSLFeature::list_dict_id: out << "list_dict"; break;

         // Density IDs
         case DSLFeature::dense_id:  out << "dense";  break;
         case DSLFeature::sparse_id: out << "sparse"; break;

         // Malloc IDs
         case DSLFeature::fix_id: out << "fix"; break;
         case DSLFeature::dyn_id: out << "dyn"; break;

         // MallocErrChecking IDs
         case DSLFeature::check_malloc_err_id: out << "check_malloc_err"; break;
         case DSLFeature::no_malloc_err_checking_id:
                                         out << "no_malloc_err_checking"; break;

         // OptFlag IDs
         case DSLFeature::speed_id: out << "speed"; break;
         case DSLFeature::space_id: out << "space"; break;

         // ErrFlag IDs
         case DSLFeature::check_as_default_id: out << "check_as_default"; break;
         case DSLFeature::no_checking_id:      out << "no_checking";      break;

         // BoundsChecking IDs
         case DSLFeature::check_bounds_id: out << "check_bounds"; break;
         case DSLFeature::no_bounds_checking_id:
                                     out << "no_bounds_checking"; break;

         // CompatChecking IDs
         case DSLFeature::check_compat_id: out << "check_compat"; break;
         case DSLFeature::no_compat_checking_id:
                                     out << "no_compat_checking"; break;

         // Rows, Cols, Order, Diags and ScalarValue IDs
         case DSLFeature::stat_val_id: out << "stat_val"; break;
         case DSLFeature::dyn_val_id:  out << "dyn_val";  break;

         default: out << "unknown";
      }
      return out;
   }
};

template<class A>
ostream& operator<<(ostream& out, const DSLFeatureInfo<A>& f)
{
   return f.display(out);
}


template<class DSLFeatures>
struct DSLFeaturesInfo
{
   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::RowsNumber>::RET,
               int_number<int, -1>,
               DSLFeatures::RowsNumber>::RET RowsNumber;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::ColsNumber>::RET,
               int_number<int, -1>,
               DSLFeatures::ColsNumber>::RET ColsNumber;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::OrderNumber>::RET,
               int_number<int, -1>,
               DSLFeatures::OrderNumber>::RET OrderNumber;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::DiagsNumber>::RET,
               int_number<int, -1>,
               DSLFeatures::DiagsNumber>::RET DiagsNumber;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::ScalarValueNumber>::RET,
               int_number<int, -1>,
               DSLFeatures::ScalarValueNumber>::RET ScalarValueNumber;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::Size>::RET,
               int_number<int, -1>,
               DSLFeatures::Size>::RET Size;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::Ratio>::RET,
               float_number<double, -1000>,
               DSLFeatures::Ratio>::RET Ratio;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::Growing>::RET,
               float_number<double, -1000>,
               DSLFeatures::Growing>::RET Growing;

   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::HashWidth>::RET,
               int_number<int, -1>,
               DSLFeatures::HashWidth>::RET HashWidth;


   static ostream& display(ostream& out)
   {
      out << "ElementType:       " << typeid(DSLFeatures::ElementType).name() << endl;
      out << "Structure:         " << DSLFeatureInfo<DSLFeatures::Structure>() << endl;
      out << "Shape:             " << DSLFeatureInfo<DSLFeatures::Shape>() << endl;
      out << "Density:           " << DSLFeatureInfo<DSLFeatures::Density>() << endl;
      out << "Malloc:            " << DSLFeatureInfo<DSLFeatures::Malloc>() << endl;
      out << "DictFormat:        " << DSLFeatureInfo<DSLFeatures::DictFormat>() << endl;
      out << "HashWidth:         " << DSLFeatureInfo<DSLFeatures::HashWidth>() << endl;
      out << "ArrOrder:          " << DSLFeatureInfo<DSLFeatures::ArrOrder>() << endl;
      out << "OptFlag:           " << DSLFeatureInfo<DSLFeatures::OptFlag>() << endl;
      out << "ErrFlag:           " << DSLFeatureInfo<DSLFeatures::ErrFlag>() << endl;
      out << "IndexType:         " << typeid(DSLFeatures::IndexType).name() << endl;
      out << "Rows:              " << DSLFeatureInfo<DSLFeatures::Rows>() << endl;
      out << "Cols:              " << DSLFeatureInfo<DSLFeatures::Cols>() << endl;
      out << "Order:             " << DSLFeatureInfo<DSLFeatures::Order>() << endl;
      out << "Diags:             " << DSLFeatureInfo<DSLFeatures::Diags>() << endl;
      out << "ScalarValue:       " << DSLFeatureInfo<DSLFeatures::ScalarValue>() << endl;
      out << "RowsNumber:        " << RowsNumber::value << endl;
      out << "ColsNumber:        " << ColsNumber::value << endl;
      out << "OrderNumber:       " << OrderNumber::value << endl;
      out << "DiagsNumber:       " << DiagsNumber::value << endl;
      out << "ScalarValueNumber: " << ScalarValueNumber::Value() << endl;
      out << "Size:              " << Size::value << endl;
      out << "Ratio:             " << Ratio::Value() << endl;
      out << "Growing:           " << Growing::Value() << endl;
      out << "MallocErrChecking: " << DSLFeatureInfo<DSLFeatures::MallocErrChecking>() << endl;
      out << "BoundsChecking:    " << DSLFeatureInfo<DSLFeatures::BoundsChecking>() << endl;
      out << "CompatChecking:    " << DSLFeatureInfo<DSLFeatures::CompatChecking>() << endl;
      out << "Format:            " << DSLFeatureInfo<DSLFeatures::Format>() << endl;
      return out;
   }
};

template<class A>
ostream& operator<<(ostream& out, const DSLFeaturesInfo<A>& f)
{
   return f.display(out);
}


template<class A>
void PrintDSLFeaturesInfo(ostream& out, const A& a)
{
   out << DSLFeaturesInfo<A::Config::DSLFeatures>();
};


#endif   // DB_MATRIX_DSLTYPEINFO_H
