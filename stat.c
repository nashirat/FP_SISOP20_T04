#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
 
int main(int argc, char **argv){
    if(argc != 2)    
        printf(2, "error.");
 
    struct stat fileStat;
    if(stat(argv[1],&fileStat) < 0)    
        printf(2, "not found.");

    char *path = argv[1];

    printf(1, "Information for %s\n", path);
    printf(1, "---------------------------\n");
    printf(1, "File name: \t\t%s\n", path);
    printf(1, "File Size: \t\t%d bytes\n",fileStat.size);
    printf(1, "Number of Links: \t%d\n",fileStat.nlink);
    printf(1, "File inode: \t\t%d\n",fileStat.ino);
    printf(1, "Device: \t\t%d\n",fileStat.dev);

    if(fileStat.type == 1){
        printf(1, "Type: \t\t\tDirectory\n");
    } else if(fileStat.type == 2){
        printf(1, "Type: \t\t\tFile\n");
    } else if(fileStat.type == 3){
        printf(1, "Type: \t\t\tDevice\n");
    }

    printf(1, "File Permissions: \t");

    if(fileStat.type == 1)
        printf(1, "d");
    else
        printf(1, "-");

    // printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    // printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    // printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    // printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    // printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    // printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    // printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    // printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    // printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    // printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    // printf("\n\n");
 
    // printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
 
    return 0;
}