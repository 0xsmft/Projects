/**
 *
 * Module:
 *  filewatcher
 *
 * Description:
 *  File Watcher -- watches for events in a given directory.
*/

#include <iostream>
#include <filesystem>
#include <string>

#include <Windows.h>

int main( int count, char** ppArgs ) 
{
	if( count != 2 )
	{
		std::cout << "Usage: " << ppArgs[ 0 ] << " <directory>\n";
		return EXIT_FAILURE;
	}

	std::filesystem::path path = std::string( ppArgs[ 1 ] );

	HANDLE dirHandle = ::CreateFileW( path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr );

	if( dirHandle == INVALID_HANDLE_VALUE )
	{
		std::cout << "Invalid handle returned from CreateFile()!\n";
		return EXIT_FAILURE;
	}

	OVERLAPPED overlapped{};
	overlapped.hEvent = ::CreateEvent( nullptr, false, false, nullptr );

	uint8_t pBuffer[ 1024 ]{};
	::ReadDirectoryChangesW( dirHandle, pBuffer, 1024, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, nullptr, &overlapped, nullptr );

	// Watching loop.
	while( true )
	{
		if( !::ReadDirectoryChangesW( dirHandle, pBuffer, 1024, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, nullptr, &overlapped, nullptr ) )
		{
			std::cout << "ReadDirectoryChangesW failed....\n";
			return EXIT_FAILURE;
		}

		DWORD waitStatus = ::WaitForSingleObject( overlapped.hEvent, INFINITE );

		DWORD bytesMoved = 0;
		::GetOverlappedResult( dirHandle, &overlapped, &bytesMoved, false );

		FILE_NOTIFY_INFORMATION* pEvent =  reinterpret_cast< FILE_NOTIFY_INFORMATION* >( pBuffer );

		// Processing loop.
		while( true )
		{
			std::wstring name( pEvent->FileName,
				pEvent->FileNameLength / sizeof( WCHAR ) );

			switch( pEvent->Action )
			{
				case FILE_ACTION_ADDED:
				{
					std::wcout << "File Added: " << name << "\n";
				} break;

				case FILE_ACTION_REMOVED:
				{
					std::wcout << "File Removed: " << name << "\n";
				} break;

				case FILE_ACTION_MODIFIED:
				{
					std::wcout << "File Modified: " << name << "\n";
				} break;

				case FILE_ACTION_RENAMED_OLD_NAME:
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					std::wcout << "File Renamed: " << name << "\n";
				} break;

				default:
					std::wcout << pEvent->Action << "\n";
					break;
			}

			if( !pEvent->NextEntryOffset )
				break;

			pEvent = reinterpret_cast< FILE_NOTIFY_INFORMATION* >( reinterpret_cast<BYTE*>( pEvent ) + pEvent->NextEntryOffset );
		}
	}
}
