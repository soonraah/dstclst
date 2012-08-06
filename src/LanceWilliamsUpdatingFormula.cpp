/**************************************************************************//**
 *	@file LanceWilliamsUpdatingFormula.cpp
 *****************************************************************************/


#include <cmath>
#include "LanceWilliamsUpdatingFormula.h"


using namespace dstclst;


//! 更新式の本体
float LanceWilliamsUpdatingFormula::formula(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	float alpha_a,
	float alpha_b,
	float beta,
	float gamma
) const
{
	return ( alpha_a * dist_1a2 ) + ( alpha_b * dist_1b2 ) + ( beta * dist_1a1b ) + ( gamma * abs( dist_1a2 - dist_1b2 ) );
}


//! 最短距離法
float FormulaNearest::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	return formula( dist_1a2, dist_1b2, dist_1a1b, 0.5f, 0.5f, 0.0f, (-0.5f) );
}


//! 最長距離法
float FormulaFurthest::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	return formula( dist_1a2, dist_1b2, dist_1a1b, 0.5f, 0.5f, 0.0f, 0.5f );
}


//! 群平均法
float FormulaGroupAvg::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	float alpha_a = static_cast< float >( num_1a ) / ( num_1a + num_1b );
	float alpha_b = static_cast< float >( num_1b ) / ( num_1a + num_1b );
	return formula( dist_1a2, dist_1b2, dist_1a1b, alpha_a, alpha_b, 0.0f, 0.0f );
}


//! Ward法
float FormulaWard::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	unsigned int n = num_1a + num_1b + num_2;
	float alpha_a = static_cast< float >( num_1a + num_2 ) / n;
	float alpha_b = static_cast< float >( num_1b + num_2 ) / n;
	float beta = ( -1.0f ) * num_2 / n;
	return formula( dist_1a2, dist_1b2, dist_1a1b, alpha_a, alpha_b, beta, 0.0f );
}


//! セントロイド法
float FormulaCentroid::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	unsigned int n1 = num_1a + num_1b;
	float alpha_a = static_cast< float >( num_1a ) / n1;
	float alpha_b = static_cast< float >( num_1b ) / n1;
	float beta = ( -1.0f ) * ( num_1a * num_1b ) / ( n1 * n1 );
	return formula( dist_1a2, dist_1b2, dist_1a1b, alpha_a, alpha_b, beta, 0.0f );
}


//! メジアン法
float FormulaMedian::operator()(
	float dist_1a2,
	float dist_1b2,
	float dist_1a1b,
	unsigned int num_1a,
	unsigned int num_1b,
	unsigned int num_2
) const
{
	return formula( dist_1a2, dist_1b2, dist_1a1b, 0.5f, 0.5f, -0.25f, 0.0f );
}

