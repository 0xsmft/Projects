/**
 *
 * Module:
 *  hexdump
 *
 * Description:
 *  Console output hexdump.
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <array>

int main( int count, char** ppArgs ) 
{
	if( count != 2 )
	{
		std::cout << "Usage " << ppArgs[ 0 ] << " <in_file>\n";
		return EXIT_FAILURE;
	}

	std::filesystem::path inFile( ppArgs[ 1 ] );
	if( !std::filesystem::exists( inFile ) )
	{
		std::cout << "File " << ppArgs[ 1 ] << " does not exist!\n";
		return EXIT_FAILURE;
	}	

	std::ifstream stream( inFile, std::ios::in | std::ios::binary );

	uint64_t offset = 0;
	uint64_t numberSinceLastOffset = 0;

	std::cout << "Offset   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 Decoded Text\n";
	std::cout << std::format( "{:08} ", offset );

	std::array<char, 16> last16Chars{ 0 };

	char currentChar = -1;
	while( true )
	{
		currentChar = stream.get();
		if( stream.eof() )
		{
			// 16 spaces
			// 32 hex digits
			// 16 + 32 = 47
			const uint64_t numberRemaingUntilNextCol = 47ull - numberSinceLastOffset;

			for( size_t i = 0; i < numberSinceLastOffset; ++i )
			{
				const auto c = last16Chars[ i ];
				if( c == 0 || c == '\n' || c == '\r' || c == '\t' )
					std::cout << std::setw( numberRemaingUntilNextCol + i ) << '.';
				else
					std::cout << std::setw( numberRemaingUntilNextCol + i ) << last16Chars[ i ];
			}

			break;
		}

		if( numberSinceLastOffset == 16 )
		{
			offset += 16;
			numberSinceLastOffset = 0;

			for( size_t i = 0; i < 16; ++i )
			{
				const auto c = last16Chars[ i ];
				if( c == 0 || c == '\n' || c == '\r' || c == '\t' )
					std::cout << '.';
				else
					std::cout << last16Chars[ i ];
			}

			last16Chars.fill( 0 );

			std::cout << "\n";
			std::cout << std::format( "{:08} ", offset );
		}

		last16Chars[ numberSinceLastOffset ] = currentChar;

		std::cout << std::format( "{:02X} ", currentChar );
		++numberSinceLastOffset;
	}

	stream.close();

	return EXIT_SUCCESS;
}
