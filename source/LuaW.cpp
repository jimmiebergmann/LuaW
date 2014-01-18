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

namespace LuaW
{

	// Constructor/destructor
	Script::Script( ) :
		m_pState( NULL ),
		m_ErrorMessage( "" )
	{
		// Create a new Lua state
		m_pState = luaL_newstate( );

		// Open the Lua libraries
		luaL_openlibs( m_pState );
	}

	Script::Script( lua_State * p_pState ) :
		m_pState( NULL ),
		m_ErrorMessage( "" )
	{
		if( p_pState )
		{
			m_pState = p_pState;
		}
	}

	Script::~Script( )
	{
	}

	// Public functions
	eError Script::Call( int p_Arguments, int p_ReturnValues )
	{
		// Make sure the stack size is ok
		int StackSize = lua_gettop( m_pState );

		if( StackSize < p_Arguments + 1 ) // Num arguments + function
		{
			return ERROR_STACK;
		}

		// Call the function at the stack.
		int Error = LUA_OK;
		if( ( Error = lua_pcall( m_pState, p_Arguments, LUA_MULTRET, 0 ) ) != LUA_OK )
		{
			// Something went wrong
			// Get the stack size again.
			StackSize = lua_gettop( m_pState );

			// Is there any error message on the stack?
			if( StackSize )
			{
				// Get the error message, put it into a std::string
				m_ErrorMessage = lua_tostring( m_pState, -1 );

				// Pop the error message from the stack.
				lua_pop( m_pState, 1 );
			}

			// Return the error code
			return ConvertErrorCode( Error );
		}

		// Everything is okey.
		return ERROR_NONE;
	}

	void Script::RegisterFunction( const char * p_pName, lua_CFunction p_Function )
	{
		lua_register( m_pState, p_pName, p_Function );
	}

	eError Script::RunFile( const char * p_pFilePath )
	{
		// Load and run the file for a first time
		if( luaL_dofile( m_pState, p_pFilePath ) != false )
		{
			// Something messed up
			// Get the stack size again.
			int StackSize = lua_gettop( m_pState );

			// Is there any error message on the stack?
			if( StackSize )
			{
				// Get the error message, put it into a std::string
				m_ErrorMessage = lua_tostring( m_pState, -1 );

				// Pop the error message from the stack.
				lua_pop( m_pState, 1 );
			}

			return ERROR_RUNTIME;
		}
		return ERROR_NONE;
	}

	eError Script::RunString( const char * p_pString )
	{
		// Load and run the string for a first time
		int Error = LUA_OK;
		if( ( Error = luaL_dostring( m_pState, p_pString ) ) != LUA_OK )
		{
			// Something messed up
			// Get the stack size again.
			int StackSize = lua_gettop( m_pState );

			// Is there any error message on the stack?
			if( StackSize )
			{
				// Get the error message, put it into a std::string
				m_ErrorMessage = lua_tostring( m_pState, -1 );

				// Pop the error message from the stack.
				lua_pop( m_pState, 1 );
			}

			return ERROR_RUNTIME;
		}
		return ERROR_NONE;
	}

	void Script::Unload( )
	{
		if( m_pState )
		{
			lua_close( m_pState );
			m_pState = NULL;
		}
	}

	// Foo Test
	class Foo
	{

	public:

		Foo( int p_Value ) :
			m_Value( p_Value )	
		{ }

		int GetData( ) const
		{
			return m_Value;
		}

	private:

		int m_Value;

	};


	int FooNewConstructor( lua_State * p_pState )
	{
		//std::cout << "[FooNewConstructor]" << std::endl;


		LuaW::Script Lua( p_pState );
		int Parameter = Lua.GetInteger( -1 );

		Foo ** ppData = (Foo**)lua_newuserdata( p_pState, sizeof(Foo * ));
		*ppData = new Foo( Parameter );

		luaL_getmetatable( p_pState, "Foo_Table" );

		lua_setmetatable( p_pState, -2 );

		return 1;
	}

	int FooGetData( lua_State * p_pState )
	{
		//std::cout << "[FooGetData]" << std::endl;
		LuaW::Script Lua( p_pState );

		Foo * pFoo = *(Foo**)luaL_checkudata( p_pState, 1, "Foo_Table" );
	
		if( pFoo == NULL )
		{
			Lua.PushInteger( 0 );
		}
		else
		{
			Lua.PushInteger( pFoo->GetData( ) );
		}

		
		//Lua.DumpStack( );

		return 1;
	}

	luaL_Reg FooFuncs[ ] =
	{
		{ "New", FooNewConstructor },
		{ "GetData", FooGetData },
		{ NULL, NULL }
	};

	void Script::FooTest( )
	{
		// Create a new table
		luaL_newmetatable( m_pState, "Foo_Table" );

		//lua_newtable( m_pState );
		luaL_setfuncs( m_pState, FooFuncs, 0 );

		// Set the __index field by pushing the meta table and set the field( pops it)
		lua_pushvalue( m_pState, -1 );
		lua_setfield( m_pState, -1, "__index" );

		// Set the meta table and make it global
		lua_setglobal( m_pState, "Foo" );
	}


	// Get globals functions
	bool Script::GetGlobalBoolean( const char * p_pName )
	{
		// Push the value by the variable name
		lua_getglobal( m_pState, p_pName );

		// Get the int
		bool Ret = static_cast<bool>( lua_toboolean( m_pState, -1 ) );
		lua_pop( m_pState, 1 );
		return Ret;
	}

	lua_Integer Script::GetGlobalInteger( const char * p_pName )
	{
		// Push the value by the variable name
		lua_getglobal( m_pState, p_pName );

		// Get the int
		lua_Integer Ret = lua_tointeger( m_pState, -1 );
		lua_pop( m_pState, 1 );
		return Ret;
	}

	lua_Number Script::GetGlobalNumber( const char * p_pName )
	{
		// Push the value by the variable name
		lua_getglobal( m_pState, p_pName );

		// Get the number
		lua_Number Ret = lua_tonumber( m_pState, -1 );
		lua_pop( m_pState, 1 );
		return Ret;
	}

	std::string Script::GetGlobalString( const char * p_pName )
	{
		// Push the value by the variable name
		lua_getglobal( m_pState, p_pName );

		// Get the string
		std::string Ret = lua_tostring( m_pState, -1 );
		lua_pop( m_pState, 1 );
		return Ret;
	}

	// Set globals functions
	void Script::SetGlobalBoolean( const char * p_pName, const bool p_Boolean )
	{
		lua_pushboolean( m_pState, static_cast<int>( p_Boolean ) );
		lua_setglobal( m_pState, p_pName );
	}

	void Script::SetGlobalInteger( const char * p_pName, const lua_Integer p_Integer )
	{
		lua_pushinteger( m_pState, p_Integer );
		lua_setglobal( m_pState, p_pName );
	}

	void Script::SetGlobalNumber( const char * p_pName, const lua_Number p_Number )
	{
		lua_pushnumber( m_pState, p_Number );
		lua_setglobal( m_pState, p_pName );
	}

	void Script::SetGlobalString( const char * p_pName, const std::string & p_String )
	{
		lua_pushstring( m_pState, p_String.c_str( ) );
		lua_setglobal( m_pState, p_pName );
	}


	// Stack functions
	void Script::ClearStack( )
	{
		int StackSize = lua_gettop( m_pState );

		if( StackSize )
		{
			lua_pop( m_pState, -1 );
		}
	}

	int Script::GetStackSize( ) const
	{
		return lua_gettop( m_pState );
	}

	void Script::DumpStack( )
	{
		int StackSize = lua_gettop( m_pState );

		std::cout << "Lua stack dump (" << StackSize << "):" << std::endl;
		for( int i = 1 ; i <= StackSize; i++ )
		{
			std::cout << i << ": ";
			
			// Get the type
			//std::cout << "(" << lua_typename( m_pState, i ) << ")  ";
			int Type = lua_type( m_pState, i );

			// LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
			switch( Type )
			{
				case LUA_TNIL:
					std::cout << "(nil)";
					break;
				case LUA_TNUMBER:
					std::cout << "(number) " <<  lua_tonumber( m_pState, i );
					break;
				case LUA_TBOOLEAN:
					std::cout << "(boolean) " << lua_toboolean( m_pState, i );
					break;
				case LUA_TSTRING:
					std::cout << "(string) " << lua_tostring( m_pState, i );
					break;
				case LUA_TTABLE:
					std::cout << "(table) ...";
					break;
				case LUA_TFUNCTION:
					std::cout << "(function) ...";
					break;
				case LUA_TUSERDATA:
					std::cout << "(user data) ...";
					break;
				case LUA_TTHREAD:
					std::cout << "(thread) ...";
					break;
				case LUA_TLIGHTUSERDATA:
					std::cout << "(light user data) ...";
					break;
				default:
					std::cout << "(Unknown) ...";
					break;
			}

			std::cout << std::endl;

		}
		std::cout << std::endl;
	}



	// Stack push functions
	void Script::Push( )
	{
		lua_pushnil( m_pState );
	}

	void Script::PushBoolean( const bool p_Boolean )
	{
		lua_pushboolean( m_pState, static_cast<int>( p_Boolean ) );
	}

	void Script::PushGlobal( const char * p_pName )
	{
		lua_getglobal( m_pState, p_pName );
	}

	void Script::PushInteger( const lua_Number p_Integer )
	{
		lua_pushinteger( m_pState, p_Integer );
	}

	void Script::PushNumber( const lua_Number p_Number )
	{
		lua_pushnumber( m_pState, p_Number );
	}

	void Script::PushString( const std::string & p_String )
	{
		lua_pushstring( m_pState, p_String.c_str( ) );
	}

	void Script::PushValue( const int p_Index )
	{
		lua_pushvalue( m_pState, p_Index );
	}

	// Stack pop functions
	void Script::Pop( )
	{
		lua_pop( m_pState, 1 );
	}

	void Script::Pop( const int p_ElementCount )
	{
		lua_pop( m_pState, p_ElementCount );
	}

	bool Script::PopBoolean( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the boolean
			bool Ret = static_cast<bool>( lua_toboolean( m_pState, -1 ) );
			lua_pop( m_pState, 1 );
			return Ret;
		}

		return false;
	}

	lua_Integer Script::PopInteger( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			lua_Integer Ret = lua_tointeger( m_pState, -1 );
			lua_pop( m_pState, 1 );
			return Ret;
		}

		return 0;
	}

	lua_Number Script::PopNumber( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			lua_Number Ret = lua_tonumber( m_pState, -1 );
			lua_pop( m_pState, 1 );
			return Ret;
		}

		return 0;
	}

	std::string Script::PopString( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			std::string Ret = lua_tostring( m_pState, -1 );
			lua_pop( m_pState, 1 );
			return Ret;
		}

		return "";
	}

			
	// Get stack functions
	bool Script::GetBoolean( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the boolean
			bool Ret = static_cast<bool>( lua_toboolean( m_pState, -1 ) );
			return Ret;
		}

		return false;
	}

	bool Script::GetBoolean( const int p_Index )
	{
		// Get the boolean
		return static_cast<bool>( lua_toboolean( m_pState, p_Index ) );
	}

	lua_Integer Script::GetInteger( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			lua_Integer Ret = lua_tointeger( m_pState, -1 );
			return Ret;
		}

		return 0;
	}

	lua_Integer Script::GetInteger( const int p_Index )
	{
		// Get the integer
		return lua_tointeger( m_pState, p_Index );
	}

	lua_Number Script::GetNumber( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			lua_Number Ret = lua_tonumber( m_pState, -1 );
			return Ret;
		}

		return 0;
	}

	lua_Number Script::GetNumber( const int p_Index )
	{
		// Get the number
		return lua_tonumber( m_pState, p_Index );
	}

	std::string Script::GetString( )
	{
		// Get stack size
		int StackSize = lua_gettop( m_pState );

		// Is there any value to full from the stack?
		if( StackSize )
		{
			// Get the integer
			std::string Ret = lua_tostring( m_pState, -1 );
			return Ret;
		}

		return "";
	}

	std::string Script::GetString( const int p_Index )
	{
		// Get the number
		return lua_tostring( m_pState, p_Index );
	}


	// General get functions
	lua_State * Script::GetState( ) const
	{
		return m_pState;
	}

	const std::string & Script::GetLastError( ) const
	{
		return m_ErrorMessage;
	}

	// Private functions
	eError Script::ConvertErrorCode( int p_Code )
	{
		switch( p_Code )
		{
			case LUA_YIELD:
			{
				return ERROR_YEILD;
			}
			break;
			case LUA_ERRRUN:
			{
				return ERROR_RUNTIME;
			}
			break;
			case LUA_ERRSYNTAX:
			{
				return ERROR_SYNTAX;
			}
			break;
			case LUA_ERRMEM:
			{
				return ERROR_MEMORY;
			}
			break;
			case LUA_ERRGCMM:
			{
				return ERROR_GARBAGE_COLLECTOR;
			}
			break;
			case LUA_ERRERR:
			{
				return ERROR_MESSAGE_HANDLER;
			}
			break;
		}

		// LUA_OK code, obviously
		return ERROR_NONE;
	}


}