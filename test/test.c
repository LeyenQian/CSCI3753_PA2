#include "predefine.h"


void show_help()
{
    printf("r <# of bytes>               [read number of bytes from the device]\n");
    printf("w <# of bytes> <content>     [write number of bytes to the device]\n");
    printf("s <offset bytes> <whence>    [set io offset of the device]\n");
    printf("  <whence> accepts following values:\n");
    printf("           0: SEEK_SET\n");
    printf("           1: SEEK_CUR\n");
    printf("           2: SEEK_END\n");
    printf("e                            [terminate current program]\n");
}


void seek_device(ssize_t offset, int whence)
{
    if( whence < 0 || whence > 2)
    {
        printf("Unsupported whence\n");
        return;
    }

    if ( lseek(device_handle, offset, whence) == IO_FAILURE )
    {
        printf("Unable to set new offset\n");
    }
}


void read_device(void* buff, size_t buff_size)
{
    if( buff_size <= 0 || buff_size > 1024)
    {
        printf("Illegal buff size\n");
        return;
    }

    ssize_t io_size = read(device_handle, buff, buff_size);
    printf("Total read bytes: %lu\n", io_size);
    printf("Content: %s\n", (PCHAR)buff);
}


void write_device(void* buff, size_t buff_size)
{
    if( buff_size <= 0 || buff_size > 1024)
    {
        printf("Illegal buff size\n");
        return;
    }

    ssize_t io_size = write(device_handle, buff, buff_size);
    printf("Total write bytes: %lu\n", io_size);
}


void dispatch_cmd(ULONG dst_cmd_num)
{
    if( dst_cmd_num == 1 && strncasecmp(dst_buff, "?", 1) == 0 )
    {
        show_help();
    }
    else if( dst_cmd_num == 1 && strncasecmp(dst_buff, "e", 1) == 0 )
    {
        exit_flag = TRUE;
    }
    else if( dst_cmd_num == 2 && strncasecmp(dst_buff, "r", 1) == 0 )
    {
        size_t size = atol(&dst_buff[1024]);
        read_device((void*)dst_buff, size);
    }
    else if( dst_cmd_num == 3 && strncasecmp(dst_buff, "w", 1) == 0 )
    {
        size_t size = atol(&dst_buff[1024]);
        PCHAR content = &dst_buff[2048];
        write_device((void*)content, size);
    }
    else if( dst_cmd_num == 3 && strncasecmp(dst_buff, "s", 1) == 0 )
    {
        ssize_t offset = atol(&dst_buff[1024]);
        int whence = atoi(&dst_buff[2048]);
        seek_device(offset, whence);
    }
}


void main()
{
	src_buff = (PCHAR)malloc( MAX_CMD_LEN );
	dst_buff = (PCHAR)malloc( MAX_SUB_CMD_LEN );
    if( src_buff == NULL || dst_buff == NULL ) return;

    device_handle = open(LAKKA_DEVICE_FILE, O_RDWR);
    if( device_handle == -1 ) return;

    for( ;; )
    {
        printf( " > " );
        fgets(src_buff, MAX_CMD_LEN, stdin);
		dispatch_cmd(StrSplit( dst_buff, 10, 1024, " | ", 3, src_buff, 0 ));
        if( exit_flag == TRUE ) break;

        memset(src_buff, 0, MAX_CMD_LEN);
        memset(dst_buff, 0, MAX_SUB_CMD_LEN);
    }

    close(device_handle);
}