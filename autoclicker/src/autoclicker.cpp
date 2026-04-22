/**
 *
 * Module:
 *  autoclicker
 *
 * Description:
 *  Mouse autoclicker
*/

#define NOMINMAX
#include <Windows.h>

#include <iostream>

static unsigned long s_DelayBetweenClicks = 140ul;

static void SendButtonInput( DWORD flags ) 
{
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dx = 0;
	input.mi.dy = 0;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dwFlags = flags;
	input.mi.dwExtraInfo = 0;

	::SendInput( 1, &input, sizeof( INPUT ) );
}

static void SendButtonInputDown() 
{
	SendButtonInput( MOUSEEVENTF_LEFTDOWN );
}

static void SendButtonInputRelease() 
{
	SendButtonInput( MOUSEEVENTF_LEFTUP );
}

int main( int count, char** ppArgs )
{
	while( true )	
	{	
		// std::numeric_limits<unsigned long>::max() - 1 because we want std::numeric_limits<unsigned long>::max() to an invalid number as
		// if the user types -1 into the console it will underflow to std::numeric_limits<unsigned long>::max() and we know thats not allowed.
		std::cout << "Enter delay in milliseconds e.g. 1500ms == 1.5s, max: " << std::numeric_limits<unsigned long>::max() - 1 << "ms \n";
		
		if( std::cin >> s_DelayBetweenClicks && s_DelayBetweenClicks != std::numeric_limits<unsigned long>::max() )
		{
			if( s_DelayBetweenClicks > 0 )
				break;
		}
		else
		{
			std::cout << "Invalid input.\n";
			std::cin.clear();
		}

		std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	}

	while( true )
	{
		SendButtonInputDown();

		::Sleep( s_DelayBetweenClicks );

		SendButtonInputRelease();
	}
}
