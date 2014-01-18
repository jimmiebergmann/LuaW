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

static const std::string g_ScriptPath = "../script/Settings.lua";

int main( )
{
	// Create and run the script
	LuaW::Script Lua;

	if( Lua.RunFile( g_ScriptPath.c_str( ) ) != LuaW::ERROR_NONE )
	{
		std::cout << Lua.GetLastError( ) << std::endl;
		std::cin.get( );
		return 0;
	}

	// Get the global variables from the lua script
	bool Flag = Lua.GetGlobalBoolean( "flag" );
	int Size = Lua.GetGlobalInteger( "size" );
	double Progress = Lua.GetGlobalNumber( "progress" );
	std::string Title = Lua.GetGlobalString( "title" );

	// Unload Lua
	Lua.Unload( );

	// Print the settings from the script file
	std::cout << "Flag: " << Flag << std::endl;
	std::cout << "Size: " << Size << std::endl;
	std::cout << "Progress: " << Progress << std::endl;
	std::cout << "Title: " << Title << std::endl;
	std::cout << std::endl;
	
	// Close the application
	std::cin.get( );
	return 0;
}
