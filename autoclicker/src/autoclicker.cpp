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

enum class MouseButton : uint8_t
{
	Left,
	Right,
	Middle,
	Extra1,
};

static unsigned long s_DelayBetweenClicks = 140ul;
static MouseButton s_MouseButton = MouseButton::Left;

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
	switch( s_MouseButton )
	{
		default:
		case MouseButton::Left:
			::SendButtonInput( MOUSEEVENTF_LEFTDOWN );
			break;

		case MouseButton::Right:
			::SendButtonInput( MOUSEEVENTF_RIGHTDOWN );
			break;

		case MouseButton::Middle:
			::SendButtonInput( MOUSEEVENTF_MIDDLEDOWN );
			break;

		case MouseButton::Extra1:
			::SendButtonInput( MOUSEEVENTF_XDOWN );
			break;
	}
}

static void SendButtonInputRelease() 
{
	switch( s_MouseButton )
	{
		default:
		case MouseButton::Left:
			::SendButtonInput( MOUSEEVENTF_LEFTUP );
			break;

		case MouseButton::Right:
			::SendButtonInput( MOUSEEVENTF_RIGHTUP );
			break;

		case MouseButton::Middle:
			::SendButtonInput( MOUSEEVENTF_MIDDLEUP );
			break;

		case MouseButton::Extra1:
			::SendButtonInput( MOUSEEVENTF_XUP );
			break;
	}
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
		std::cout << "Enter the mouse button number, (0 = Left, 1 = Right, 2 = Middle, 3 = Extra1)\n";

		uint16_t btn = 0u;
		if( std::cin >> btn && btn != std::numeric_limits<uint8_t>::max() && btn <= 3 )
		{
			s_MouseButton = ( MouseButton ) btn;
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
