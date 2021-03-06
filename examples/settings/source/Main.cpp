// ///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2013 Jimmie Bergmann - jimmiebergmann@gmail.com
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but
//    is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any
//    source distribution.
// ///////////////////////////////////////////////////////////////////////////

#include <LuaW.hpp>
#include <iostream>

static const std::string g_ScriptPath = "../script/Functions.lua";

int SumFunction( lua_State * p_pState )
{
	// Pass the state to a new lua class
	LuaW::Script Lua( p_pState );

	// Get the paramters
	int StackSize = Lua.GetStackSize( );
	int Sum = 0;

	for( int i = 0; i < StackSize; i++ )
	{
		Sum += Lua.PopInteger( );
	}

	// Push the result to the lua stack(makes it into a return value for the lua function)
	Lua.PushInteger( Sum );

	// return the number of return values
	return 1; 
}

int main( )
{
	LuaW::Script Lua;

	// First function example
	// Register function for Lua
	Lua.RegisterFunction( "Sum", SumFunction );

	// Run the Lua file
	if( Lua.RunFile( g_ScriptPath.c_str( ) ) != LuaW::ERROR_NONE )
	{
		std::cout << "[Error]: " << Lua.GetLastError( ) << std::endl;
		std::cin.get( );
		return 0;
	}

	// Another function example
	// Call Lua function
	const int Paramter = 15;
	Lua.PushGlobal( "Foo" );
	Lua.PushInteger( Paramter ); // Pass paramter to lua function
	
	Lua.Call( 1, 1 ); // call the function with 1 argument and 1 return value
	const int Result = Lua.PopInteger( ); // Should be 2 * 15 = 30
	std::cout << "(C)   Result from Foo( " << Paramter << " ): " << Result << std::endl;
	
	// Unload Lua
	Lua.Unload( );
	
	// Close the application
	std::cin.get( );
	return 0;
}