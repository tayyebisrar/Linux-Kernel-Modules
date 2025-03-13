#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char **argv) {
	int fd;
	char c;

	fd = open("/dev/hello0", O_RDWR);

	if (fd < 0){
		perror("Opening error for /dev/hello0");
		return fd;
	}

	while (read(fd, &c, 1)){
		putchar(c);
	}

	close(fd);
	return 0;
}
