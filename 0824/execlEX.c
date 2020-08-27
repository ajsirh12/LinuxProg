#include<unistd.h>

int main(){
	execl("/bin/sh", "/bin/sh", NULL);
}
