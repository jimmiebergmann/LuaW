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

// Lua wrapper

#ifndef LUA_W_HPP
#define LUA_W_HPP

#include <lua.hpp>
#include <string>

namespace LuaW
{
	// Forward declaractions
	class Script;


	// Error codes for the Lua wrapper
	enum eError
	{
		ERROR_NONE = 0,
		ERROR_RUNTIME = 1,
		ERROR_SYNTAX = 2,
		ERROR_YEILD = 3,
		ERROR_STACK = 4,
		ERROR_MEMORY = 5,
		ERROR_MESSAGE_HANDLER = 6,
		ERROR_GARBAGE_COLLECTOR = 7
	};


	// Typedefs
	typedef int ( * CFunction )( Script * );


	// Lua script class
	class Script
	{

	public:

		// Constructor/destructor
		Script( );
		Script( lua_State * p_pState );
		~Script( );

		// Public functions
		eError Call( int p_Arguments, int p_ReturnValues );
		void RegisterFunction( const char * p_pName, lua_CFunction p_Function );
		eError RunFile( const char * p_pFilePath );
		eError RunString( const char * p_pString );
		void Unload( );

		// Foo test
		void FooTest( );

		// Get globals functions
		bool GetGlobalBoolean( const char * p_pName );
		lua_Integer GetGlobalInteger( const char * p_pName );
		lua_Number GetGlobalNumber( const char * p_pName );
		std::string GetGlobalString( const char * p_pName );

		// Set globals functions
		void SetGlobalBoolean( const char * p_pName, const bool p_Boolean );
		void SetGlobalInteger( const char * p_pName, const lua_Integer p_Integer );
		void SetGlobalNumber( const char * p_pName, const lua_Number p_Number );
		void SetGlobalString( const char * p_pName, const std::string & p_String );
	
		// Stack functions
		void ClearStack( );
		int GetStackSize( ) const;
		void DumpStack( );
		
		// Stack push functions
		void Push( );
		void PushBoolean( const bool p_Boolean );
		void PushGlobal( const char * p_pName );
		void PushInteger( const lua_Number p_Integer );
		void PushNumber( const lua_Number p_Number );
		void PushString( const std::string & p_String );
		void PushValue( const int p_Index ); // Push copy from the stack
		
		// Stack pop functions
		void Pop( );
		void Pop( const int p_ElementCount );
		bool PopBoolean( );
		lua_Integer PopInteger( );
		lua_Number PopNumber( );
		std::string PopString( );
	
		// Get stack functions
		bool GetBoolean( );
		bool GetBoolean( const int p_Index );
		lua_Integer GetInteger( );
		lua_Integer GetInteger( const int p_Index );
		lua_Number GetNumber( );
		lua_Number GetNumber( const int p_Index );
		std::string GetString( );
		std::string GetString( const int p_Index );

		// General get functions
		lua_State * GetState( ) const;
		const std::string & GetLastError( ) const;

	private:

		// Private functions
		eError ConvertErrorCode( int p_Code ); // Converts from int to eError

		// Private variables
		lua_State * m_pState;
		std::string m_ErrorMessage;

	};

};


#endif