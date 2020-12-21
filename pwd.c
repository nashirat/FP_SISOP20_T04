#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

#define size 512
#define NULL ((void*)0)

static int getcwd(char* result_Path);
static char* look_Up(int ino, char* parent_Path, char* result_Path);
static int find_Dir(int fd, int ino, char* p);

int main(int argc, char *argv[]) {
    char result_Path[size];
    if (getcwd(result_Path)) printf(1, "%s\n", result_Path);
    else printf(2, "pwd failed");
    exit();
}

static int getcwd(char* result_Path) { //function returns current working directory
	result_Path[0] = '\0';

	char parent_Path[size];
	strcpy(parent_Path, ".");

	struct stat st;
	if (stat(parent_Path, &st) < 0) return 0;

	char* p = look_Up(st.ino, parent_Path, result_Path);
	if (p == NULL) return 0;
	if (result_Path[0] == '\0'){
        strcpy(result_Path, "/");
    }
	return 1;
}

static char* look_Up(int ino, char* parent_Path, char* result_Path) {
    //function goes to parent directory for parent_Path and find the path wirh recursive
	strcpy(parent_Path + strlen(parent_Path), "/" "..");
	struct stat st;
	if (stat(parent_Path, &st) < 0) return NULL;

	if (st.ino == ino) {
		// No parent directory exists: must be the root.
		return result_Path;
	}

	char* found_Path = NULL;
	int fd = open(parent_Path, O_RDONLY);
	if (fd >= 0) {
		char* p = look_Up(st.ino, parent_Path, result_Path);
		if (p != NULL) {
		strcpy(p, "/");
		p += sizeof("/") - 1;

		// Find current directory.
		if (find_Dir(fd, ino, p))
			found_Path = p + strlen(p);
		}
		close(fd);
	}
	return found_Path;
}

static int find_Dir(int fd, int ino, char* p) {
    //function finds file name based on the inode of the directory
	struct dirent de;
	while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		if (de.inum == 0) continue;
		if (de.inum == ino) {
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0';
            return 1;
		}
	}
	return 0;
}