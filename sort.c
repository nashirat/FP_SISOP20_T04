#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int unique(char **v, int dim){
	printf(1, "u%d\n", dim);
	dim--;
	for(int i=0;i<dim;i++){
		for(int j=i+1;j<dim;){
			if(strcmp(v[j],v[i])==0){
				for(int k=j;k<dim;k++){
					v[k]=malloc(sizeof(char)*strlen(v[k+1]));
					strcpy(v[k],v[k+1]);
				}
				dim--;
			} else
				j++;
		}
	}
	return dim++;
}

int compare(char* first, char* second){
	char fstr[256];
	char sstr[256];
	int fskip=0;
	int sskip=0;
	strcpy(fstr, first);
	strcpy(sstr, second);
	int a = strlen(fstr);
	int b = strlen(sstr);
	int i=0;
	int flag=0;
	for(i=0;i<a;i++){
		if(!(fstr[i] < 48 || (fstr[i] >57 && fstr[i] < 65) || (fstr[i]>90 && fstr[i]<97) || (fstr[i] > 122)) && flag==0){
			flag = 1;
			fskip = i;
		}

		if(fstr[i] >= 'a' && fstr[i] <= 'z'){
			fstr[i] = fstr[i] - 32;
		}
	}
	flag = 0;
	for(i=0;i<b;i++){
		if(!(sstr[i] < 48 || (sstr[i] >57 && sstr[i] < 65) || (sstr[i]>90 && sstr[i]<97) || (sstr[i] > 122)) && flag==0){
			flag = 1;
			sskip = i;
		}
		
        if(sstr[i] >= 'a' && sstr[i] <= 'z'){
			sstr[i] = sstr[i] - 32;
		}
	}
	return strcmp(fstr+fskip, sstr+sskip);
}

void sort(char* path, int mode, char* dest){
	int fd;
	fd = open(path, 0);
	char line[1];
	int status;
	long long int total=0;
	fd = open(path, 0);

	if(fd < 0){
		printf(1, "Error: cannot open %s\n", path);
		return;
	}

	while((status = read(fd, line, sizeof(line))) > 0){
		if(line[0] == '\n')
		total++;
	}

	if(status<0){
		printf(1, "Error: cannot read %s\n", path);
		return;
	}

	char** arr;
	arr = (char**)malloc(total * sizeof(char*));
	close(fd);
	int i=0;
	for(i=0;i<total;i++){
		arr[i] = (char*)malloc(256 * sizeof(char));
	}

	i=0;
	int j=0;
	int fd3;
	fd3 = open(path, O_RDWR);
	while((status = read(fd3, line, sizeof(line))) > 0){

		arr[i][j] = line[0];
		j++;
		if(line[0] == '\n'){
			arr[i][j] = '\0';
			i++;
			j=0;
		}
	}
	if(status<0){
		printf(1, "Error: cannot read %s\n", path);
		return;
	}

	int flag=0;

	if(mode == 3){
		for(int i=0;i<total-1;i++){
			if(strcmp(arr[i],arr[i+1])<0){
				printf(1,"%s ok\n",arr[i]);
				flag++;
			}	
		}
		if(total-1==flag){
			printf(1, "udah ke sort\n");
		} else {
			printf(1, "belum ke sort\n");
		}
		free(arr);
		exit();
	}

	char temp[256];
	for(i=0;i<total-1;i++){
		for(j=0;j<(total-i-1);j++){
			int k=j+1;
			if(compare(arr[j],arr[k]) > 0){
				strcpy(temp, arr[j]);
				strcpy(arr[j], arr[k]);
				strcpy(arr[k], temp);
			}
		}
	}

	if(mode == 0){
		for(i=total;i>0;i--){
			printf(1, "%s", arr[i-1]);
		}
	}
	else if(mode == 1){
		for(i=0;i<total;i++){
			printf(1, "%s", arr[i]);
		}
	}
	else if(mode == 2){
		int fd2=-1;
		fd2 = open(dest, O_CREATE | O_WRONLY);
		if(fd2 < 0){
			printf(1, "Error: cannot open %s\n", dest);
			close(fd);
			return;
		}
		for(i=0;i<total;i++){
			write(fd2, arr[i], strlen(arr[i]));
		}
		close(fd2);
	} else if(mode == 4){
		total = unique(arr, total);

		for(i=0; i<=total; i++){
			printf(1, "%s", arr[i]);
		}
	}
	close(fd3);
	return;
}

void bubbleSort(char *path, int dim){
	int fd;
	fd=open(path, 0);
	char tmp[255];
	char *v[255];

	if(fd<0){
		printf(2,"sort: No such file or directory");close(fd);exit();
	}

	int readed = read(fd, tmp, sizeof(tmp));
	close(fd);
	int n1=0;
	char row[255];

	for(int i=0;i<readed;i++){

		if(tmp[i]!='\n'){
			row[n1]=tmp[i];
			n1++;
		} else {   
			row[n1]=0;
			v[dim]=malloc(sizeof(char)*n1);
			strcpy(v[dim],row);
			dim++;
			n1=0;
		}
	}

	int i=0;
	int total=dim;

	int check=0;
	while(dim>1 && !check){
		check=1;
		for(i=0;i<dim-1;i++)
			if(atoi(v[i])>atoi(v[i+1])){
				char tmp[255];
				strcpy(tmp,v[i]); 
				v[i]=malloc(sizeof(char)*(strlen(v[i+1])));

				strcpy(v[i],v[i+1]);
				v[i+1]=malloc(sizeof(char)*strlen(tmp));
				strcpy(v[i+1],tmp);

				check=0;
				}
		dim--;
	}

	for(int i=0;i<total;i++)
		printf(1,"%s\n", v[i]);
}


void sortmonth(char *path){
	int fd;
	fd = open(path, 0);
	char line[1];
	int status;
	long long int total=0;
	fd = open(path, 0);

	if(fd < 0){
		printf(1, "Error: cannot open %s\n", path);
		return;
	}

	while((status = read(fd, line, sizeof(line))) > 0){
		if(line[0] == '\n')
		total++;
        printf(1, "read\n");
	}

	printf(1, "%lld\n", total);

	if(status<0){
		printf(2, "Error: cannot read %s\n", path);
		return;
	}
}

int main(int argc, char* argv[]){
	if(argc < 2)
	printf(1, "usage: sort <mode> <filename>");
	else {
		if(argv[1][0] == '-'){
			if(argv[1][1] == 'o'){
				sort(argv[3], 2, argv[2]);
			} else if(argv[1][1] == 'r'){
				sort(argv[2], 0, argv[1]);
			} else if(argv[1][1] == 'n') {
				int dim=0;
				bubbleSort(argv[2], dim);
            } else if(argv[1][1] == 'k' && argv[1][2] == '2'){ //kolom2
				
            } else if(argv[1][1] == 'c'){ //cek uda sort apa blm
				sort(argv[2], 3, argv[1]);
            } else if(argv[1][1] == 'u'){ //sort&hapus kl ada duplikasi
				sort(argv[2], 4, argv[1]);
            } else if(argv[1][1] == 'M'){ //bulan
				sortmonth(argv[2]);
            } else {
				printf(1, "Error: mode undefined\n");
			}
        } else if(argc == 2){
		    sort(argv[1], 1, argv[1]);
        }
	}
	exit();
}