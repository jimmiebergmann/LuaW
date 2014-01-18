#ifndef EXAMPLE_OBJECT_HPP
#define EXAMPLE_OBJECT_HPP

#include <Vector2.hpp>

class Object
{

public:

	// Constructors
	Object( ) :
		m_Position( 0.0f, 0.0f ),
		m_Size( 0.0f )
	{ }

	Object( const Vector2 & p_Position,
			const double p_Size ) :
		m_Position( p_Position ),
		m_Size( p_Size )
	{ }

	// Set functions
	void SetPosition( const Vector2 & p_Position )
	{
		m_Position = p_Position;
	}

	void SetSize( const double p_Size )
	{
		m_Size = p_Size;
	}

	// Get functions
	Vector2 GetPosition( ) const
	{
		return m_Position;
	}

	double GetSize( ) const
	{
		return m_Size;
	}

private:

	// Private variables
	Vector2 m_Position;
	double m_Size;

};

#endif