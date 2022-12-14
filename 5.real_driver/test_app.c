#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main(int argc, char *argv[])
{
    int fd;
    char option;
    char tmp_str[] = "XXXXXXXXXYYYYYYYYYYYY";

    printf("***********************\n");
    printf("***********************\n");


    fd = open("/dev/etx_device", O_RDWR);
    if(fd < 0 ){
        printf("Cannot open device file...\n");
        return 0;
    }

    while(argc<=1){
        printf("*****Enter Option*********\n");   
        printf("    1. Write    \n");  
        printf("    2. Read    \n");  
        printf("    3. Exit    \n");  
        scanf("  %c", &option);
        printf("Your Option: %c\n", option);

        switch(option){
            case '1':
                // If a command-line argument was not provided, prompt the user to enter a string
                if (argc != 2) {
                    printf("Enter the string to write into driver: %c\n", option);
                    scanf(" %[^\t\n]s", write_buf);
                }
                printf("Data Writing...");
                write(fd, write_buf, strlen(write_buf)+1);
                printf("Done !\n");
                break;
            case '2':
                printf("Data Reading...");
                read(fd, read_buf, 1024);
                printf("Done !\n");
                printf("Read Data = %s\n\n", read_buf);
                break;
            case '3':
                close(fd);
                exit(1);
                break;         
            default:
                printf("Enter valid option: %c\n", option);
                break;                   
        }
    }

if (argc>1)
{
    write_buf=(int8_t*)"12345678901234567890aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbb";
    write(fd, write_buf, strlen(write_buf)+1);
    read(fd, read_buf, 1024);
}
    close(fd);
}
