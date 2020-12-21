#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int fdfile, line_limit = 1000;
char output[100];
char *deadline;

void split_function (void);
int new_File (void);
void help (void);

int main(argc, argv)
int argc;
char **argv;
{
	output[0] = 'x';
	fdfile = -1;
	if(argc > 4)printf(2,"Argumen terlalu banyak\n");

	if(argv[1][0] == '-'){
		if(strcmp(argv[1], "--help") == 0) help();
		else if(strcmp(argv[1], "-l") == 0){
			if(argv[2][0] >= '0' && argv[2][0] <= '9' && line_limit == 1000){
				line_limit = atoi(argv[2]);
			}
			else
			{
				printf(2,"%s bukan parameter yang valid\n", argv[2]);
				exit();
			}
			strcpy(output, argv[3]);
			
		}
	}
	else{
		strcpy(output, argv[1]);
	}
	if ((fdfile = open(output, O_RDONLY)) < 0) {
		printf(2,"Tidak dapat membuka file. Ketik split --help untuk bantuan\n");
		exit();
	}
	strcat(output, ".aaa");

	for (deadline = output; *deadline; deadline++);
	deadline--;

	*deadline = 'a' - 1;
	split_function();
	exit();
	return(0);
}

void split_function()
{
	char buf[1024];
	register char *index, *base;
	register int n;
	int fd;
	long lines = 0L;

	fd = -1;
	while ((n = read(fdfile, buf, 1024)) > 0) {
		base = index = buf;
		while (--n >= 0) {
			if (*index++ == '\n')
				if (++lines % line_limit == 0) {
					if (fd == -1) fd = new_File();
					if (write(fd, base, (int) (index - base)) != (int) (index - base))
						exit();
					base = index;
					close(fd);
					fd = -1;
				}
		}
		if (index == base) continue;
		if (fd == -1) fd = new_File();
		if (write(fd, base, (int) (index - base)) != (int) (index - base))
			exit();
	}
	exit();
}

int new_File()
{
	int fd;

	if (++*deadline > 'z') {
		*deadline = 'a';		
		++*(deadline - 1);	
	}
	if ((fd = open(output,O_CREATE|O_RDWR)) < 0) {
		printf(2,"Cannot create new file.\n");
		exit();
	}
	return fd;
}

void help(){
    printf(1,"split : Membagi file menjadi beberapa file dengan default beberapa file tersebut tidak melebihi 1000 line\n");
    printf(1,"split [OPSI] [PARAMETER] [NAMAFILE]\n");
    printf(1,"OPSI:\n");
	printf(1,"\t-l : Membagi setiap file dengan batasan line yang ditentukan\n");
    printf(1,"\t--help : Memperlihatkan bantuan\n");
}
