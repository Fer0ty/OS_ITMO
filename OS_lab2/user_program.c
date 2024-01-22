#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 256

void get_stats(const char *sysfs_path, int delay_seconds) {
    int fd;
    char buffer[BUF_SIZE];

    while (1) {
        //open sysfs
        fd = open(sysfs_path, O_RDONLY);
        if (fd == -1) {
            perror("Error opening sysfs file");
            exit(EXIT_FAILURE);
        }

        //read
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) {
            perror("Error reading from sysfs file");
            exit(EXIT_FAILURE);
        }

        //end
        buffer[bytesRead] = '\0';

        printf("Memory stats:\n%s\n", buffer);


        close(fd);

        // delay
        sleep(delay_seconds);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <sysfs_path> <delay_seconds>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *sysfs_path = argv[1];
    int delay_seconds = atoi(argv[2]);

    get_stats(sysfs_path, delay_seconds);

    return 0;
}
