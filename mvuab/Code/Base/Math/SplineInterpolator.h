//-----------------------------------------------------------------------
// CCSplineInterpolatorInterpolatorn class
// ...
//-----------------------------------------------------------------------
#ifndef CSplineInterpolator_INTERPOLATOR_H
#define CSplineInterpolator_INTERPOLATOR_H

#include "Utils\Types.h"
#include "Math\MathTypes.h"

#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

namespace Math
{
	// CSplineInterpolator interpolation
	class CSplineInterpolator
	{
	public:
		enum bd_type {
			first_deriv = 1,
			second_deriv = 2
		};

	private:
		std::vector<double> m_x,m_y;            // x,y coordinates of points
		// interpolation parameters
		// f(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i
		std::vector<double> m_a,m_b,m_c;        // CSplineInterpolator coefficients
		double  m_b0, m_c0;                     // for left extrapol
		bd_type m_left, m_right;
		double  m_left_value, m_right_value;
		bool    m_force_linear_extrapolation;

	public:
		// set default boundary condition to be zero curvature at both ends
		CSplineInterpolator(): m_left(second_deriv), m_right(second_deriv),
			m_left_value(0.0), m_right_value(0.0),
			m_force_linear_extrapolation(false)
		{
			;
		}

		// optional, but if called it has to come be before set_points()
		void set_boundary(bd_type left, double left_value,
						  bd_type right, double right_value,
						  bool force_linear_extrapolation=false);
		void set_points(const std::vector<double>& x,
						const std::vector<double>& y, bool cubic_CSplineInterpolator=true);
		double operator() (double x) const;
	};
} //namespace Math
#endif //CSplineInterpolator_INTERPOLATOR_H