/*
   Copyright (c) 2009-2014, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"

#include "./Trsv/LN.hpp"
#include "./Trsv/LT.hpp"
#include "./Trsv/UN.hpp"
#include "./Trsv/UT.hpp"

namespace El {

template<typename F>
void Trsv
( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag,
  const Matrix<F>& A, Matrix<F>& x )
{
    DEBUG_ONLY(
        CallStackEntry cse("Trsv");
        if( x.Height() != 1 && x.Width() != 1 )
            LogicError("x must be a vector");
        if( A.Height() != A.Width() )
            LogicError("A must be square");
        const Int xLength = ( x.Width()==1 ? x.Height() : x.Width() );
        if( xLength != A.Height() )
            LogicError("x must conform with A");
    )
    const char uploChar = UpperOrLowerToChar( uplo );
    const char transChar = OrientationToChar( orientation );
    const char diagChar = UnitOrNonUnitToChar( diag );
    const Int m = A.Height();
    const Int incx = ( x.Width()==1 ? 1 : x.LDim() );
    blas::Trsv
    ( uploChar, transChar, diagChar, m,
      A.LockedBuffer(), A.LDim(), x.Buffer(), incx );
}

template<typename F>
void Trsv
( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag,
  const DistMatrix<F>& A, DistMatrix<F>& x )
{
    DEBUG_ONLY(CallStackEntry cse("Trsv"))
    if( uplo == LOWER )
    {
        if( orientation == NORMAL )
            trsv::LN( diag, A, x );
        else
            trsv::LT( orientation, diag, A, x );
    }
    else
    {
        if( orientation == NORMAL )
            trsv::UN( diag, A, x );
        else
            trsv::UT( orientation, diag, A, x );
    }
}

#define PROTO(F) \
  template void Trsv \
  ( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag, \
    const Matrix<F>& A, Matrix<F>& x ); \
  template void Trsv \
  ( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag, \
    const DistMatrix<F>& A, DistMatrix<F>& x );

#define EL_NO_INT_PROTO
#include "El/macros/Instantiate.h"

} // namespace El