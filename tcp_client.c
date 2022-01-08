#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <curses.h>
#include <pthread.h>

#define AREA_SIZE_HEIGHT 22
#define AREA_SIZE_WIDTH 42
#define READ_BUFFER_LENGTH 1610
#define WRITE_BUFFER_LENGTH 2

typedef struct {
    int y;
    int x;
} position;

WINDOW *main_Window;

typedef struct data {
    int *buffer_r;
    int *buffer_w;
    int sockfd;
    pthread_mutex_t *mutex;
    pthread_cond_t *canRead;
    bool reading;
} DATA;

void *reader(void *args) {
    DATA *data = (DATA *) args;
    int buffer[READ_BUFFER_LENGTH];
    int n;
    do {
        bzero(buffer,READ_BUFFER_LENGTH*sizeof(int));
        n = read(data->sockfd, buffer, READ_BUFFER_LENGTH);
        pthread_mutex_lock(data->mutex);
        memcpy(data->buffer_r,buffer,READ_BUFFER_LENGTH);
        data->reading  = true;
        pthread_mutex_unlock(data->mutex);
        pthread_cond_signal(data->canRead);
        if (n < 0)
        {
            perror("Error reading from socket");
            break;
        }
    }while(n > 0);
}


void *writer(void *args) {
    DATA *data = (DATA *) args;
    int n = write(data->sockfd, data->buffer_w, WRITE_BUFFER_LENGTH);
    if (n < 0) {
        perror("Error writing to socket");
    }
}

void printText(int x,int y,char str){

    mvwprintw(main_Window,y,x,"%c",str);
}
void initialize(){
    initscr();
    srand( time( NULL ) );
    noecho( );
    curs_set( 0 );
    halfdelay( 1 );
    keypad( main_Window, TRUE );
    start_color();
    init_pair( 1,COLOR_RED,COLOR_BLACK);
    init_pair( 2,COLOR_YELLOW,COLOR_BLACK );
    init_pair( 3,COLOR_MAGENTA,COLOR_BLACK );
    init_pair( 4,COLOR_GREEN,COLOR_BLACK );
    int startx = (80 - AREA_SIZE_WIDTH) / 2;
    int starty = (24 - AREA_SIZE_HEIGHT) / 2;
    main_Window = newwin(AREA_SIZE_HEIGHT, AREA_SIZE_WIDTH, starty, startx);
    box(main_Window,0,0);
}

int snek(DATA *data) {
    initialize();
    DATA *dataa = (DATA *) data;
    int keyP2 = 's';

    int buffer[WRITE_BUFFER_LENGTH];

    int buffer_w[READ_BUFFER_LENGTH];

    while (1) {

        int in = wgetch(main_Window);
        box(main_Window,0,0);
        if( in != ERR )
            keyP2 = in;

        dataa->buffer_w[0] = keyP2;
        writer(dataa);

        for (int i = 1; i < AREA_SIZE_WIDTH - 1; ++i) {
            for (int j = 1; j < AREA_SIZE_HEIGHT - 1; ++j) {
                printText(i,j,' ');
            }
        }
        // citanie dat od servera
        pthread_mutex_lock(dataa->mutex);
        while(!dataa->reading) {
            pthread_cond_wait(dataa->canRead,dataa->mutex);
        }
        dataa->reading=false;
        memcpy(buffer_w,dataa->buffer_r,READ_BUFFER_LENGTH);
        pthread_mutex_unlock(dataa->mutex);
        int tmp = 0;
        int tmp1 = 0;
        for (int i = 0; i < READ_BUFFER_LENGTH; i+=2) {
            while(buffer_w[i] != ERR){
                if (i == 0) {
                    wattron(main_Window, COLOR_PAIR(1));
                    printText(buffer_w[i], buffer_w[i + 1], '0');
                    wattroff(main_Window, COLOR_PAIR(1));
                    tmp1++;
                }else if (tmp1==2){
                    wattron(main_Window, COLOR_PAIR(4));
                    printText(buffer_w[i], buffer_w[i + 1], '0');
                    wattroff(main_Window, COLOR_PAIR(4));
                    tmp1++;
                }else {
                    wattron(main_Window, COLOR_PAIR(2));
                    printText(buffer_w[i], buffer_w[i + 1], '0');
                    wattroff(main_Window, COLOR_PAIR(2));

                }
                i += 2;
            }
            tmp++;
            tmp1++;
            if (tmp == 3){
                wattron(main_Window, COLOR_PAIR(3));
                printText(buffer_w[i],buffer_w[i+1],'o');
                wattroff(main_Window, COLOR_PAIR(3));
                break;
            }
        }
        wrefresh(main_Window);
        }
}



int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }
    int *buffer_w =malloc((WRITE_BUFFER_LENGTH) * sizeof(int));
    int *buffer_r =malloc((READ_BUFFER_LENGTH) * sizeof(int));
    pthread_t t_read;
    pthread_mutex_t mutex;
    pthread_cond_t canRead;

    pthread_cond_init(&canRead,NULL);

    pthread_mutex_init(&mutex,NULL);

    DATA data = {buffer_r,buffer_w,sockfd,&mutex,&canRead,false};

    pthread_create(&t_read,NULL, &reader, &data);

    snek(&data);

    pthread_join(t_read,NULL);


    pthread_cond_destroy(&canRead);
    pthread_mutex_destroy(&mutex);
    free(buffer_w);
    free(buffer_r);
    endwin();
    close(sockfd);

    return 0;
}