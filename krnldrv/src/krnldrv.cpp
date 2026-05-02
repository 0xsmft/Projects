/**
 *
 * Module:
 *  krnldrv
 *
 * Description:
 *  Kernel Driver
*/

#define _AMD64_
#include <wdm.h>

NTSTATUS DriverEntry( DRIVER_OBJECT* pDriverObject, UNICODE_STRING* pRegistryPath ) 
{
	::DbgPrint( "Hello" );

#if defined(XY_DRV_BSOD)
	::KeBugCheckEx( 0x0000000A, 0x1, 0x2, 0x3, 0x4 );
#endif

	return STATUS_SUCCESS;
}
