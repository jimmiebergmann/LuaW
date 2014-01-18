#ifndef EXAMPLE_VECTOR2_HPP
#define EXAMPLE_VECTOR2_HPP

class Vector2
{

public:

	// Constructors
	Vector2( ) :
		x( 0.0f ), y( 0.0f )
	{ }

	Vector2( const Vector2 & p_Vector ) :
		x( p_Vector.x ),
		y( p_Vector.y )
	{ }

	Vector2(	const double p_X,
				const double p_Y ) :
		x( p_X ), y( p_Y )
	{ }

	// Public variables
	double x;
	double y;

};

#endif