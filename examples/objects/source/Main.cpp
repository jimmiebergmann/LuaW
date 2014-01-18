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
#include <Object.hpp>
#include <iostream>
#include <vector>

static const std::string g_ScriptPath = "../script/Objects.lua";
static std::vector<Object*> g_Objects;

int main( )
{
	LuaW::Script Lua;

	// Run the Lua file
	if( Lua.RunFile( g_ScriptPath.c_str( ) ) != LuaW::ERROR_NONE )
	{
		std::cout << "[Error]: " << Lua.GetLastError( ) << std::endl;
		std::cin.get( );
		return 0;
	}
	
	// Unload Lua
	Lua.Unload( );
	
	// Close the application
	std::cin.get( );
	return 0;
}