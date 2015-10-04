//-----------------------------------------------------------------------
// CSplineInterpolatorn class
// ...
//-----------------------------------------------------------------------
#ifndef SPLINE_INTERPOLATOR_H
#define SPLINE_INTERPOLATOR_H

#include "Utils\Types.h"
#include "Math\MathTypes.h"

#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

namespace Math
{
	// band matrix solver
	class CBandMatrix
	{
	private:
		std::vector< std::vector<double> > m_upper;  // upper band
		std::vector< std::vector<double> > m_lower;  // lower band
	public:
		CBandMatrix() {};                             // constructor
		CBandMatrix(int dim, int n_u, int n_l);       // constructor
		~CBandMatrix() {};                            // destructor
		void resize(int dim, int n_u, int n_l);      // init with dim,n_u,n_l
		int dim() const;                             // matrix dimension
		int num_upper() const
		{
			return m_upper.size()-1;
		}
		int num_lower() const
		{
			return m_lower.size()-1;
		}
		// access operator
		double & operator () (int i, int j);            // write
		double   operator () (int i, int j) const;      // read
		// we can store an additional diogonal (in m_lower)
		double& saved_diag(int i);
		double  saved_diag(int i) const;
		void lu_decompose();
		std::vector<double> r_solve(const std::vector<double>& b) const;
		std::vector<double> l_solve(const std::vector<double>& b) const;
		std::vector<double> lu_solve(const std::vector<double>& b,
									 bool is_lu_decomposed=false);

	};

} //namespace Math
#endif //SPLINE_INTERPOLATOR_H