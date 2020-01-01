
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define TCP_PORT 8080
#define MULTI_PORT1 5450
#define MULTI_PORT2 5451
#define MULTI_PORT3 5452
#define MULTI_PORT4 5453

#define INFO_PORT1 5460
#define INFO_PORT2 5461
#define INFO_PORT3 5462
#define INFO_PORT4 5463
#define INFO_PORT5 5464

#define RADIO_1 "239.192.1.10"
#define RADIO_2 "239.192.1.11"
#define RADIO_3 "239.192.1.12"
#define RADIO_4 "239.192.1.13"
#define RADIO_5 "239.192.1.14"

#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS 20
#define CLADDR_LEN 100

#define BUF_SIZE1 256
#define BUF_SIZE2 512
#define BUF_SIZE3 1024
#define BUF_SIZE4 2048
#define BUF_SIZE5 4096
#define BUF_SIZE6 10240

int d_flag[5];

/*------------------ STRUCTURES DEFINATIONS ----------------------------*/
typedef struct station_info_request {
    uint8_t type;
} station_req;

typedef struct station_list {
    uint8_t station_number;
    uint8_t station_name_size;
    char station_name[100];
    uint32_t multicast_address;
    uint16_t data_port;
    uint16_t info_port;
    uint32_t bit_rate;
} station_info;

typedef struct site_info {

    uint8_t type;
    uint8_t site_name_size;
    char site_name[100];
    uint8_t site_desc_size;
    char site_desc[100];
    uint8_t station_count;
    station_info station_list[5];

} Site_Info;

typedef struct song_info {

    uint8_t type;
    uint8_t song_name_size;
    char song_name[100];
    uint16_t remaining_time_in_sec;
    uint8_t next_song_name_size;
    char next_song_name[100];
} Song_info;
/*--------------------------------------------------------------------*/

void delay(int milliseconds) {
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause) {
        now = clock();
    }
}

struct args {
    char RADIO[17];
    int MULTI_PORT;
    int INFO_PORT;
    int BUF_SIZE;
    char video_name[200];
    char duration_file_name[100];
};

struct args2 {

    char song_name[100];
    uint16_t total_time_in_sec;
    char next_song_name[100];
};

void *threadfunctionInfo(void *input) {
    Song_info songs_playlist;

    char radio[17], video_name[200], duration_file_name[100];
    int info_port, buf_SIZE;

    info_port = ((struct args *)input)->INFO_PORT;
    strcpy(radio, ((struct args *)input)->RADIO);
    buf_SIZE = ((struct args *)input)->BUF_SIZE;
    strcpy(video_name, ((struct args *)input)->video_name);
    strcpy(duration_file_name, ((struct args *)input)->duration_file_name);

    printf("%s\t%d\t%d\t%s\n", radio, info_port, buf_SIZE, duration_file_name);

    /*----------------------    SOCKET MULTI-CAST   --------------------*/
    int multi_sockfd;
    struct sockaddr_in servaddr;

    if ((multi_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("server : socket");
        exit(TRUE);
    }

    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(radio);
    servaddr.sin_port = htons(info_port);
    /*------------------------------------------------------------------*/
    /*----------------------    BUFFER DECLARATIONS	  ------------------*/
    int send_status;
    /*------------------------------------------------------------------*/
    int totTime = 60;

    songs_playlist.type = 12;
    songs_playlist.song_name_size = 13;
    strcpy(songs_playlist.song_name, "Let her go");
    songs_playlist.remaining_time_in_sec = totTime;

    songs_playlist.next_song_name_size = 13;
    strcpy(songs_playlist.next_song_name, "Avengers_clip");

    while (songs_playlist.remaining_time_in_sec != 0) {
        printf("Here Error.....");
        if ((send_status = sendto(
                 multi_sockfd, &songs_playlist, sizeof(songs_playlist), 0,
                 (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1) {
            printf("printing\n");
            perror("sender: sendto");
            exit(1);
        }
        printf(
            "Remaining time:--------------------------------------------- %d\n",
            songs_playlist.remaining_time_in_sec);

        //	sleep(1);
        songs_playlist.remaining_time_in_sec--;
    }

    return NULL;
}

void *threadfunctionTCP(void *input) {
    station_req station_info_req;
    Site_Info slist;
    slist.type = 10;
    slist.site_name_size = 29;
    strcpy(slist.site_name, "Ross Hannoman's Internet Radio");
    slist.site_desc_size = 3;
    strcpy(slist.site_desc, "IDK");
    slist.station_count = 3;

    slist.station_list[0].station_number = 1;
    slist.station_list[0].station_name_size = 9;
    strcpy(slist.station_list[0].station_name, "RADIO ONE");
    slist.station_list[0].multicast_address = inet_addr(RADIO_1);
    slist.station_list[0].data_port = MULTI_PORT1;
    slist.station_list[0].info_port = INFO_PORT1;
    slist.station_list[0].bit_rate = BUF_SIZE5;

    slist.station_list[1].station_number = 2;
    slist.station_list[1].station_name_size = 9;
    strcpy(slist.station_list[1].station_name, "RADIO TWO");
    slist.station_list[1].multicast_address = inet_addr(RADIO_2);
    slist.station_list[1].data_port = MULTI_PORT2;
    slist.station_list[1].info_port = INFO_PORT2;
    slist.station_list[1].bit_rate = BUF_SIZE5;

    slist.station_list[2].station_number = 3;
    slist.station_list[2].station_name_size = 11;
    strcpy(slist.station_list[2].station_name, "RADIO THREE");
    slist.station_list[2].multicast_address = inet_addr(RADIO_3);
    slist.station_list[2].data_port = MULTI_PORT3;
    slist.station_list[2].info_port = INFO_PORT3;
    slist.station_list[2].bit_rate = BUF_SIZE5;

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(TCP_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {

            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, &station_info_req, sizeof(station_req));
        printf("%d\n", station_info_req.type);
        send(new_socket, &slist, sizeof(Site_Info), 0);
        printf("Hello message sent\n");
    }
    return NULL;
}

void *threadfunctionUDP(void *input) {

    // usleep(1000);
    char radio[17], video_name[200], duration_file_name[100];
    int multiport, buf_SIZE;

    multiport = ((struct args *)input)->MULTI_PORT;
    strcpy(radio, ((struct args *)input)->RADIO);
    buf_SIZE = ((struct args *)input)->BUF_SIZE;
    strcpy(video_name, ((struct args *)input)->video_name);
    strcpy(duration_file_name, ((struct args *)input)->duration_file_name);

    printf("%s\t%d\t%d\t%s\n", radio, multiport, buf_SIZE, duration_file_name);

    clock_t start, mid, end;
    double execTime;

    FILE *duration_file;
    duration_file = fopen(duration_file_name, "re");

    /*----------------------    SOCKET MULTI-CAST   --------------------*/
    int multi_sockfd;
    struct sockaddr_in servaddr;

    if ((multi_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("server : socket");
        exit(TRUE);
    }

    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(radio);
    servaddr.sin_port = htons(multiport);
    /*-------------------------------------------------------------------*/

    /*----------------------  BUFFER DECLARATIONS	 --------------------*/
    char buffer[buf_SIZE];
    int send_status;
    /*------------------------------------------------------------------*/

    /*----------------------   FILE DECLARATIONS	--------------------*/
    FILE *mediaFile;
    int filesize, packet_index, read_size, total_sent;
    packet_index = 1;
    total_sent = 0;
    printf("VIDEO NAME: %s\n\n", video_name);
    mediaFile = fopen(video_name, "re");
    printf("Getting Picture Size\n");

    if (mediaFile == NULL) {
        printf("Error Opening Image File");
    }

    fseek(mediaFile, 0, SEEK_END);
    filesize = ftell(mediaFile);
    fseek(mediaFile, 0, SEEK_SET);
    printf("Total Picture size: %i\n", filesize);

    char string[100] = {0};
    int ret = 0, hour = 0, min = 0, sec = 0;
    fread(string, 1, 100, duration_file);
    sscanf(string, "%d:%d:%d", &hour, &min, &sec);
    int total_time = 3600 * hour + 60 * min + sec;
    int num_pac_sec = (filesize / total_time) / buf_SIZE;

    printf("---------- %d , %s\n", num_pac_sec, duration_file_name);

    /*-------------------------------------------------------------------*/
    execTime = 0;
    start = clock();
    while (!feof(mediaFile)) {

        read_size = fread(buffer, 1, buf_SIZE, mediaFile);
        total_sent += read_size;
        printf("Packet Size: = %d\n", read_size);

        if ((send_status = sendto(multi_sockfd, buffer, sizeof(buffer), 0,
                                  (struct sockaddr *)&servaddr,
                                  sizeof(servaddr))) == -1) {

            perror("sender: sendto");
            exit(1);
        }
        printf("%d : Packet Number: %i\n", multiport, packet_index);
        packet_index++;
        if (packet_index % num_pac_sec == 0) {
            mid = clock();
            execTime = ((double)(mid - start)) / CLOCKS_PER_SEC;
            execTime = (0.9 - execTime);
            usleep((int)(execTime * 1000000));
            // sleep(1);
            printf("CURRENT SONG NAME : AVENGERS CLIP\n");
            printf("CURRENT TIME : 00: 02 sec\n");
            printf("NEXT SONG NAME : NATURALS \n");
            delay(400);
            start = clock();
        }
        delay(5);
        // usleep(13000);
    }
    memset(buffer, 0, sizeof(buffer));
    close(multi_sockfd);
    return NULL;
}

int main() {
    pthread_t thread_id[5];
    pthread_t threadTCP;
    pthread_create(&threadTCP, NULL, threadfunctionTCP, NULL);

    struct args *stations1 = (struct args *)malloc(sizeof(struct args));
    struct args *stations2 = (struct args *)malloc(sizeof(struct args));
    struct args *stations3 = (struct args *)malloc(sizeof(struct args));
    /*struct args *stations4 = (struct args *)malloc(sizeof(struct args));
    struct args *stations5 = (struct args *)malloc(sizeof(struct args));
    */

    char sys_call[] = "ffmpeg -i station1.mp4  2>&1 | grep Duration | cut -d ' "
                      "' -f 4 | sed s/,// > duration_1.txt";
    system(sys_call);
    strcpy(sys_call, "ffmpeg -i station2.mp4  2>&1 | grep Duration | cut -d ' "
                     "' -f 4 | sed s/,// > duration_2.txt");
    system(sys_call);
    strcpy(sys_call, "ffmpeg -i station3.mp4  2>&1 | grep Duration | cut -d ' "
                     "' -f 4 | sed s/,// > duration_3.txt");
    system(sys_call);

    strcpy(stations1->RADIO, RADIO_1);
    stations1->MULTI_PORT = MULTI_PORT1;
    stations1->INFO_PORT = INFO_PORT1;
    stations1->BUF_SIZE = BUF_SIZE5;
    strcpy(stations1->video_name, "station1.mp4");
    strcpy(stations1->duration_file_name, "duration_1.txt");
    pthread_create(&thread_id[0], NULL, threadfunctionUDP, (void *)stations1);

    strcpy(stations2->RADIO, RADIO_2);
    stations2->MULTI_PORT = MULTI_PORT2;
    stations2->INFO_PORT = INFO_PORT2;
    stations2->BUF_SIZE = BUF_SIZE5;
    strcpy(stations2->video_name, "station2.mp4");
    strcpy(stations2->duration_file_name, "duration_2.txt");
    pthread_create(&thread_id[1], NULL, threadfunctionUDP, (void *)stations2);

    strcpy(stations3->RADIO, RADIO_3);
    stations3->MULTI_PORT = MULTI_PORT3;
    stations3->INFO_PORT = INFO_PORT3;
    stations3->BUF_SIZE = BUF_SIZE5;
    strcpy(stations3->video_name, "station3.mp4");
    strcpy(stations3->duration_file_name, "duration_3.txt");
    pthread_create(&thread_id[2], NULL, threadfunctionUDP, (void *)stations3);

    /*strcpy(stations4->RADIO, RADIO_4);
    stations4->MULTI_PORT = MULTI_PORT4;
    stations4->BUF_SIZE = BUF_SIZE2;
    pthread_create(&thread_id[3], NULL, threadfunctionUDP,(void*) stations4);

    strcpy(stations5->RADIO, RADIO_5);
    stations5->MULTI_PORT = MULTI_PORT5;
    stations5->BUF_SIZE = BUF_SIZE1;
    pthread_create(&thread_id[4], NULL, threadfunctionUDP,(void*) stations5);
    */

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    // pthread_join(thread_id[2], NULL);
    // pthread_join(thread_id[3], NULL);

    pthread_join(threadTCP, NULL);

    return 0;
}
