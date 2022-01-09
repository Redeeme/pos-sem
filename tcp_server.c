#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <pthread.h>

//2414
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
    int buffer[WRITE_BUFFER_LENGTH];
    int sockfd = data->sockfd;
    int n;
    do {
        bzero(buffer,WRITE_BUFFER_LENGTH*sizeof(int));
        n = read(sockfd, buffer, WRITE_BUFFER_LENGTH);
        pthread_mutex_lock(data->mutex);
        memcpy(data->buffer_r,buffer,WRITE_BUFFER_LENGTH);
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
    int n = write(data->sockfd, data->buffer_w, READ_BUFFER_LENGTH);
    if (n < 0) {
        perror("Error writing to socket");
    }
}

void printText(int x, int y, char str) {

    mvwprintw(main_Window, y, x, "%c", str);
}

void directionOfSnake(int key, position *snake) {
    switch (key) {
        case 's'://dole
            snake[0].y++;
            break;
        case 'd'://doprava
            snake[0].x++;
            break;
        case 'w'://hore
            snake[0].y--;
            break;
        case 'a'://dolava
            snake[0].x--;
            break;
        default:
            break;
    }
}

position printFood(position fruit) {
    fruit.x = rand() % (AREA_SIZE_WIDTH - 2) + 1;
    fruit.y = rand() % (AREA_SIZE_HEIGHT - 2) + 1;
    wattron(main_Window, COLOR_PAIR(3));
    printText(fruit.x, fruit.y, 'o');
    wattroff(main_Window, COLOR_PAIR(3));
    refresh();
    return fruit;
}

void printSnake(position *snake, int length,int type) {
    for (int i = 0; i < length; i++) {
        if (i == 0 && type == 1) {
            wattron(main_Window, COLOR_PAIR(1));
            printText(snake[i].x, snake[i].y, '0');
            wattroff(main_Window, COLOR_PAIR(1));
        } else if(i == 0 && type == 2){
            wattron(main_Window, COLOR_PAIR(4));
            printText(snake[i].x, snake[i].y, '0');
            wattroff(main_Window, COLOR_PAIR(4));
        }
        else {
            wattron(main_Window, COLOR_PAIR(2));
            printText(snake[i].x, snake[i].y, '0');
            wattroff(main_Window, COLOR_PAIR(2));
        }
    }
}

void eraseSnake(position *snake, int length) {
    for (int i = 0; i < length; i++) {
        printText(snake[i].x, snake[i].y, ' ');
    }
}

int selfColision(position *snake, int length) {
    for (int i = 1; i < length - 1; ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return -1;
        }
    }
    return 0;
}
void destruct(DATA* data,position* snake1, position* snake2){
    pthread_cond_destroy(data->canRead);
    pthread_mutex_destroy(data->mutex);
    close(data->sockfd);
    free(snake1);
    free(snake2);
    free(data->buffer_w);
    free(data->buffer_r);


    endwin();
    exit(0);
}

void initialize() {
    initscr();
    srand(time(NULL));
    noecho();
    curs_set(0);
    halfdelay(1);
    keypad(main_Window, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    int startx = (80 - AREA_SIZE_WIDTH) / 2;
    int starty = (24 - AREA_SIZE_HEIGHT) / 2;
    main_Window = newwin(AREA_SIZE_HEIGHT, AREA_SIZE_WIDTH, starty, startx);
    box(main_Window, 0, 0);
}
void writeToClient(DATA* data, int snakeLengthP1, int snakeLengthP2,
                   position* snakeP1, position* snakeP2, position fruit, int type){
    int buffer_w[READ_BUFFER_LENGTH];
    int tmp = 0;
    for (int i = 0; i <= ((snakeLengthP1 + snakeLengthP2) * 2) + 4; i += 2) {//snake100snake200f00xxxx
        if (i < snakeLengthP1 * 2) {
            buffer_w[i] = snakeP1[tmp].x;
            buffer_w[i + 1] = snakeP1[tmp].y;
            tmp++;
            if (i  == (snakeLengthP1 - 1) * 2) {
                i += 2;
                buffer_w[i] = -1;
                buffer_w[i + 1] = -1;
                tmp = 0;
            }
        } else if (i > snakeLengthP1 * 2 && i <= ((snakeLengthP1 + snakeLengthP2) * 2)){
            buffer_w[i] = snakeP2[tmp].x;
            buffer_w[i + 1] = snakeP2[tmp].y;
            tmp++;
            if (i == ((snakeLengthP1 + snakeLengthP2) * 2)){
                i += 2;
                buffer_w[i] = -1;
                buffer_w[i + 1] = -1;
            }
        }else{
            buffer_w[i] = fruit.x;
            buffer_w[i + 1] = fruit.y;
            buffer_w[i + 2] = snakeLengthP1;
            buffer_w[i + 3] = snakeLengthP2;
            buffer_w[i+4] = type;
            buffer_w[i + 5] = 0;
            buffer_w[i + 6] = -1;
            buffer_w[i + 7] = -1;
        }
    }
    memcpy(data->buffer_w,buffer_w,READ_BUFFER_LENGTH*sizeof(int));
}

int snek(DATA *data) {
    initialize();
    int type = 1;
    int snakeLengthP1 = 3;
    int snakeLengthP2 = 3;
    int keyP1 = 'd';
    int keyP2 = 'd';
    position fruit;

    position *snakeP1 = malloc((snakeLengthP1 * 2) * sizeof(int));//allocation for snake
    position *snakeP2 = malloc((snakeLengthP2 * 2) * sizeof(int));//allocation for snake
    DATA *dataa = (DATA *) data;
    //sleep(10);

    int buffer[WRITE_BUFFER_LENGTH];
    snakeP1[0].x = 3;
    snakeP1[0].y = 1;
    snakeP1[1].x = 2;
    snakeP1[1].y = 1;
    snakeP1[2].x = 1;
    snakeP1[2].y = 1;//snake starting pos
    snakeP2[0].x = 6;
    snakeP2[0].y = 2;
    snakeP2[1].x = 5;
    snakeP2[1].y = 2;
    snakeP2[2].x = 4;
    snakeP2[2].y = 2;//snake starting pos
    fruit = printFood(fruit);//spawn first food
    int n = 0;
    printSnake(snakeP1, snakeLengthP1,1);
    printSnake(snakeP2, snakeLengthP2,2);
    wrefresh(main_Window);
    while (1) {

        int in = wgetch(main_Window);
        box(main_Window, 0, 0);
        if (in != ERR)
            keyP1 = in;
        pthread_mutex_lock(dataa->mutex);
        while(!dataa->reading) {
            pthread_cond_wait(dataa->canRead,dataa->mutex);
        }
        dataa->reading = false;
        keyP2 = dataa->buffer_r[0];
        pthread_mutex_unlock(dataa->mutex);
        wrefresh(main_Window);
        eraseSnake(snakeP1, snakeLengthP1);
        eraseSnake(snakeP2, snakeLengthP2);
        for (int i = snakeLengthP1 - 1; i >= 1; i--) {
            snakeP1[i].x = snakeP1[i - 1].x;
            snakeP1[i].y = snakeP1[i - 1].y;
        }
        for (int i = snakeLengthP2 - 1; i >= 1; i--) {
            snakeP2[i].x = snakeP2[i - 1].x;
            snakeP2[i].y = snakeP2[i - 1].y;
        }

        directionOfSnake(keyP1, snakeP1);
        directionOfSnake(keyP2, snakeP2);


        printSnake(snakeP1, snakeLengthP1,1);
        printSnake(snakeP2, snakeLengthP2,2);


        if (fruit.x == snakeP1[0].x && fruit.y == snakeP1[0].y) {
            position *tmpSnake = malloc((snakeLengthP1 * 2) * sizeof(int));
            for (int i = 0; i < (snakeLengthP1); ++i) {
                tmpSnake[i] = snakeP1[i];
            }
            snakeLengthP1++;
            snakeP1 = realloc(snakeP1, (snakeLengthP1 * 2) * sizeof(int));
            for (int i = 0; i < (snakeLengthP1 - 1); ++i) {
                snakeP1[i] = tmpSnake[i];
            }
            free(tmpSnake);
            fruit = printFood(fruit);
        }
        if (fruit.x == snakeP2[0].x && fruit.y == snakeP2[0].y) {
            position *tmpSnake = malloc((snakeLengthP2 * 2) * sizeof(int));
            for (int i = 0; i < (snakeLengthP2); ++i) {
                tmpSnake[i] = snakeP2[i];
            }
            snakeLengthP2++;
            snakeP2 = realloc(snakeP2, (snakeLengthP2 * 2) * sizeof(int));
            for (int i = 0; i < (snakeLengthP2 - 1); ++i) {
                snakeP2[i] = tmpSnake[i];
            }
            free(tmpSnake);
            fruit = printFood(fruit);
        }
        if (snakeP1[0].x < 1 || snakeP1[0].x > AREA_SIZE_WIDTH - 2 || snakeP1[0].y < 1 ||
            snakeP1[0].y > AREA_SIZE_HEIGHT - 2) {
            printf("\nSKORE HRACA1: %d     SKORE HRACA2: %d\t\t\t\t\t",snakeLengthP1,snakeLengthP2);
            writeToClient(dataa, snakeLengthP1, snakeLengthP2, snakeP1, snakeP2, fruit, 0);
            destruct(dataa,snakeP1,snakeP2);
        }else{
            writeToClient(dataa, snakeLengthP1, snakeLengthP2, snakeP1, snakeP2, fruit, 1);
        }
        if (snakeP2[0].x < 1 || snakeP2[0].x > AREA_SIZE_WIDTH - 2 || snakeP2[0].y < 1 ||
            snakeP2[0].y > AREA_SIZE_HEIGHT - 2) {
            printf("\nSKORE HRACA1: %d     SKORE HRACA2: %d\t\t\t\t\t",snakeLengthP1,snakeLengthP2);
            writeToClient(dataa, snakeLengthP1, snakeLengthP2, snakeP1, snakeP2, fruit, 0);
            destruct(dataa,snakeP1,snakeP2);
        }else{
            writeToClient(dataa, snakeLengthP1, snakeLengthP2, snakeP1, snakeP2, fruit, 1);
        }

        writer(dataa);
        if (n < 0) {
            perror("Error writing to socket");
        }
        wrefresh(main_Window);
    }
    endwin();
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];
    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    printf("\n|--------------------------------------------------------|\n"
           "VITAJTE V HRE SNAKE, HRAC 2 SA UZ NAPOJIL HRA ZACNE ONEDLHO...\n"
           " OVLADNIE HRY -> w -> hore\n"
           "             -> a -> dolava\n"
           "             -> s -> dole\n"
           "             -> d -> doprava\n");
    sleep(1);

    int *buffer_w =malloc((WRITE_BUFFER_LENGTH) * sizeof(int));
    int *buffer_r =malloc((READ_BUFFER_LENGTH) * sizeof(int));
    pthread_t t_read;
    pthread_mutex_t mutex;
    pthread_cond_t canRead;

    pthread_cond_init(&canRead,NULL);

    pthread_mutex_init(&mutex,NULL);

    DATA data = {buffer_r,buffer_w,newsockfd,&mutex,&canRead,false};

    pthread_create(&t_read,NULL, &reader, &data);
    snek(&data);
    pthread_join(t_read,NULL);


    pthread_cond_destroy(&canRead);
    pthread_mutex_destroy(&mutex);
    endwin();
    close(newsockfd);
    close(sockfd);

    return 0;
}