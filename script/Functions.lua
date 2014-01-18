-- Call C function
print( "(Lua) Result from Sum( 1, 2, 3, 4 ): " .. Sum( 1, 2, 3, 4 ) ) -- Should print 10

-- Calling this function via C
function Foo( x )
	return 2 * x
end