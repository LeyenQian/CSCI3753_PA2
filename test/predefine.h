#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define LAKKA_DEVICE_FILE "/dev/lakka_character_device_file"
#define IO_SUCCESS  0
#define IO_FAILURE -1

#define ULONG unsigned long
#define PCHAR char*
#define CHAR char
#define BOOL int
#define FALSE 0
#define TRUE !(FALSE)
#define MAX_CMD_LEN			( 4096 * sizeof(CHAR) )
#define MAX_SUB_CMD_LEN		( 10 * 1024 * sizeof(CHAR) )


PCHAR src_buff = NULL;
PCHAR dst_buff = NULL;
BOOL exit_flag = FALSE;
int device_handle = -1;


ULONG StrSplit ( PCHAR DesStr, ULONG MaxSegmNum, ULONG SegmStep, PCHAR Delimiter, ULONG DelimiterNum, PCHAR SrcStr, ULONG SrcStrLen )
{
	ULONG CurSegmNum = 0;
	ULONG CurSegmLen = 1;
	PCHAR SegmStart = NULL;
	PCHAR SegmEnd = NULL;
	ULONG i = 0;

	if ( SrcStrLen == 0 ) SrcStrLen = strlen( SrcStr );

	for ( i = 0; i < SrcStrLen && CurSegmNum < MaxSegmNum; i ++, CurSegmLen ++ )
	{
		for ( ULONG j = 0; j < DelimiterNum; j ++ )
		{
			if ( SrcStr[i] == Delimiter[j] )
			{
				if ( CurSegmLen > 1 )
				{
					for( SegmStart = &SrcStr[i-CurSegmLen+1]; SegmStart < &SrcStr[i] && *SegmStart == ' '; SegmStart ++ );
					for( SegmEnd = &SrcStr[i-1]; SegmEnd >= SegmStart && *SegmEnd == ' '; SegmEnd -- );

					if( ( CurSegmLen = SegmEnd - SegmStart + 1 ) > SegmStep ) CurSegmLen = SegmStep;
					( (PCHAR)memcpy( &DesStr[CurSegmNum++*SegmStep], SegmStart, CurSegmLen ) )[CurSegmLen] = '\0';
				}

				CurSegmLen = 0;
				break;
			}
		}
	}

	if ( CurSegmLen > 1 && CurSegmNum < MaxSegmNum )
	{
		for( SegmStart = &SrcStr[i-CurSegmLen+1]; SegmStart < &SrcStr[i] && *SegmStart == ' '; SegmStart ++ );
		for( SegmEnd = &SrcStr[i-1]; SegmEnd >= SegmStart && *SegmEnd == ' '; SegmEnd -- );

		if( ( CurSegmLen = SegmEnd - SegmStart + 1 ) > SegmStep ) CurSegmLen = SegmStep;
		( (PCHAR)memcpy( &DesStr[CurSegmNum++*SegmStep], SegmStart, CurSegmLen ) )[CurSegmLen] = '\0';
	}

	return CurSegmNum;
}