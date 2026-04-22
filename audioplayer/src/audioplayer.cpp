/**
 * 
 * Module:
 *  audioplayer
 *
 * Description:
 *  Basic audio player. (WAV files only)
*/

#include <filesystem>
#include <iostream>

#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

int main( int count, char** ppArgs ) 
{
	if( count <= 1 )
	{
		std::cout << "Need a path to a *.wav file!\n";
		return EXIT_FAILURE;
	}

	const std::string soundPath = ppArgs[ 1 ];
	if( !std::filesystem::exists( soundPath ) )
	{
		std::cout << "File does not exist!\n";
		return EXIT_FAILURE;
	}

	std::cout << "Attempting to play sound...\n";
	if( !::PlaySoundA( soundPath.data(), nullptr, SND_FILENAME ) )
	{
		std::cout << "Call to PlaySoundA failed!\n";
		return EXIT_FAILURE;
	}
	
	std::cout << "Completed... exiting...\n";
}
