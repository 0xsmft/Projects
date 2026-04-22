/**
 *
 * Module:
 *  passgen
 *
 * Description:
 *  Very very basic password generator.
*/

#include <string>
#include <random>
#include <iostream>

static std::random_device s_RandomDevice;
static std::mt19937_64 s_RandomEngine( s_RandomDevice() );

// Note: char, signed char, unsigned char, char8_t, int8_t, and uint8_t are not allowed
// so we have to use a short here, smallest size we can use.
static std::uniform_int_distribution<uint16_t> s_AsciiTableDist( 33, 126 );
static std::uniform_int_distribution<uint16_t> s_BooleanDist( 0, 1 );
static std::uniform_int_distribution<uint16_t> s_AsciiNumberDist( 0x30, 0x39 );

static std::uniform_int_distribution<std::size_t> s_MaxLenDist( 16, 64 );
static std::size_t s_MaxLength = s_MaxLenDist( s_RandomEngine );

static std::string GeneratePass() 
{
	std::string out;
	out.resize( s_MaxLength );

	for( size_t i = 0; i < s_MaxLength; ++i )
	{
		const bool isNumber = s_BooleanDist( s_RandomEngine );
		if( isNumber )
			out[ i ] = ( char ) s_AsciiNumberDist( s_RandomEngine );
		else
			out[ i ] = ( char ) s_AsciiTableDist( s_RandomEngine );
	}

	return out;
}

int main( int count, char** ppArgs ) 
{
	std::cout << "Password is: " << GeneratePass() << "\n";
}
