/**
 *
 * Module:
 *  rle
 *
 * Description:
 *  Run-length encoding.
*/

#include <iostream>
#include <filesystem>
#include <fstream>

static void Compress( const std::filesystem::path& rPath ) 
{
	std::filesystem::path outPath = rPath;
	outPath.replace_extension( ".out.txt" );

	std::ofstream fout( outPath, std::ios::binary );
	std::ifstream stream( rPath, std::ios::in );

	char currentChar = stream.get();
	char lastChar = -1;
	uint64_t n = 1u;

	while( ( lastChar = stream.get() ) && currentChar != -1 )
	{
		if( currentChar == lastChar )
		{
			++n;

			// If the next increment will cause n to become a multi-byte number we must reset the counter.
			if( n == 0xFF )
			{
				fout.write( reinterpret_cast< const char* >( &n ), sizeof( uint8_t ) );
				fout << currentChar;

				// Reset counter.
				n = 1u;
			}
		}
		else
		{
			fout.write( reinterpret_cast< const char* >( &n ), sizeof( uint8_t ) );
			fout << currentChar;
			n = 1u;

			currentChar = lastChar;
		}
	}

	stream.close();
	fout.close();
}

static void Decompress( const std::filesystem::path& rPath )
{
	char numberChar = -1;
	char Char = -1;

	std::filesystem::path inPath = rPath;
	inPath.replace_extension( ".out.txt" );
	
	std::filesystem::path outPath = rPath;
	outPath.replace_extension( ".decomp.txt" );

	std::ofstream fout( outPath );
	std::ifstream stream( inPath, std::ios::binary | std::ios::in );

	while( stream.get( numberChar ) )
	{
		stream.get( Char );

		for( uint8_t i = 0; i < ( uint8_t ) numberChar; ++i )
		{
			fout << Char;
		}
	}

	stream.close();
	fout.close();
}

int main( int count, char** ppArgs ) 
{
	if( count != 3 )
	{
		std::cout << "Usage: " << ppArgs[ 0 ] << " <file_to_compress> <mode one of the following: compress OR decompress>\n";
		return EXIT_FAILURE;
	}

	if( !std::strcmp( ppArgs[ 2 ], "compress" ) )
	{
		if( !std::filesystem::exists( ppArgs[ 1 ] ) )
		{
			std::cout << "File to compress: " << ppArgs[ 1 ] << " does not exist!\n";
			return EXIT_FAILURE;
		}

		Compress( std::filesystem::path( ppArgs[ 1 ] ) );
	}
	else if( !std::strcmp( ppArgs[ 2 ], "decompress" ) )
	{
		Decompress( std::filesystem::path( ppArgs[ 1 ] ) );
	}
	else
	{
		std::cout << "Invalid mode option: '" << ppArgs[ 1 ] << "' must be compress or decompress\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
