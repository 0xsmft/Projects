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
	uint64_t numberOfBytesInCurrentRow = 0;

	std::cout << "Offset   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 Decoded Text\n";
	std::cout << std::format( "{:08} ", offset );

	std::array<char, 16> last16Chars{ 0 };

	char currentChar = -1;
	while( true )
	{
		currentChar = stream.get();
		if( stream.eof() )
		{
			// To get the number of hex number we have written in this row we can simply
			// do numberOfBytesInCurrentRow * 2, e.g. 7 number will become 14 hex digits
			// then we need to add the number of spaces which is just the number of bytes we have written.
			const uint64_t numberOfCharsInRow = ( ( numberOfBytesInCurrentRow * 2 ) + numberOfBytesInCurrentRow );

			// 16 spaces (between hex values) + 1 space to next col
			// 32 hex digits
			// 17 + 32 = 49
			const uint64_t numberRemaingUntilNextCol = 49ull - numberOfCharsInRow;

			std::cout << std::setw( numberRemaingUntilNextCol );

			for( size_t i = 0; i < numberOfBytesInCurrentRow; ++i )
			{
				const auto c = last16Chars[ i ];
				if( c <= 31 || c == 127 )
					std::cout << '.';
				else
					std::cout << last16Chars[ i ];
			}

			break;
		}

		if( numberOfBytesInCurrentRow == 16 )
		{
			offset += 16;
			numberOfBytesInCurrentRow = 0;

			for( size_t i = 0; i < 16; ++i )
			{
				const auto c = last16Chars[ i ];
				if( c <= 31 || c == 127 )
					std::cout << '.';
				else
					std::cout << last16Chars[ i ];
			}

			last16Chars.fill( 0 );

			std::cout << "\n";
			std::cout << std::format( "{:08} ", offset );
		}

		last16Chars[ numberOfBytesInCurrentRow ] = currentChar;

		std::cout << std::format( "{:02X} ", currentChar );
		++numberOfBytesInCurrentRow;
	}

	stream.close();

	return EXIT_SUCCESS;
}
