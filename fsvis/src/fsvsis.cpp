/**
 *
 * Module:
 *  fsvis
 *
 * Description:
 *  Directory file size visualisation.
*/

#include <iostream>
#include <filesystem>
#include <ranges>
#include <unordered_map>

struct FileInformation
{
	std::wstring Name;
	uint64_t SizeInBytes = 0;
};

int main( int count, char** ppArgs )
{
	if( count != 2 )
	{
		std::cout << "Usage: " << ppArgs[ 0 ] << " <dir_to_analyse>\n";
		return EXIT_FAILURE;
	}

	std::filesystem::path path = std::filesystem::path( ppArgs[ 1 ] );
	if( !std::filesystem::exists( path ) )
	{
		std::cout << "File: " << path << " does not exist!\n";
		return EXIT_FAILURE;
	}

	std::vector<FileInformation> files;
	files.reserve( 1024 );

	// TODO: Access right checking...
	for( const auto& rEntry : std::filesystem::recursive_directory_iterator( path ) | std::ranges::views::filter( []( const auto& rCandidate ) { return !rCandidate.is_directory(); } ) )
	{
		auto& rPath = rEntry.path();
		files.emplace_back( rPath.wstring(), std::filesystem::file_size( rPath ) );
	}

	std::sort( files.begin(), files.end(), 
		[]( const auto& rA, const auto& rB ) 
	{
		return rA.SizeInBytes > rB.SizeInBytes;
	} );

	for( const auto& [rName, rSize] : files )
	{
		std::wcout << rName << "\n";
	
		std::string unit = " bytes ";
		float printedSize = rSize;

		// KB
		if( printedSize > 1024 ) 
		{
			printedSize /= 1024;
			unit = " KiB ";
		}

		// MB
		if( printedSize > 1024 ) 
		{
			printedSize /= 1024;
			unit = " MiB ";
		}

		// GB
		if( printedSize > 1024 )
		{
			printedSize /= 1024;
			unit = " GiB ";
		}

		std::cout << printedSize << unit;

		const auto numBlocks = std::max( std::log10( rSize ), 1.0 );

		for( size_t i = 0; i < numBlocks; ++i )
		{
			std::cout << ( uint8_t ) 219u;
		}
		
		std::cout << "\n\n";
	}

	return EXIT_SUCCESS;
}
