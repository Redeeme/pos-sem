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
    int *buffer;
    int sockfd;
} DATA;

void *reader(void *args) {
    DATA *data = (DATA *) args;
    int buffer[WRITE_BUFFER_LENGTH];
    int sockfd = data->sockfd;
    int n;
    bzero(buffer, WRITE_BUFFER_LENGTH * sizeof(int));
    n = read(sockfd, buffer, WRITE_BUFFER_LENGTH * sizeof(int));
    memcpy(data->buffer, buffer, WRITE_BUFFER_LENGTH * sizeof(int));
    if (n < 0) {
        perror("Error reading from socket");
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

void printSnake(position *snake, int length) {
    for (int i = 0; i < length; i++) {
        if (i == 0) {
            wattron(main_Window, COLOR_PAIR(1));
            printText(snake[i].x, snake[i].y, '0');
            wattroff(main_Window, COLOR_PAIR(1));
            refresh();
        } else {
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
    int startx = (80 - AREA_SIZE_WIDTH) / 2;
    int starty = (24 - AREA_SIZE_HEIGHT) / 2;
    main_Window = newwin(AREA_SIZE_HEIGHT, AREA_SIZE_WIDTH, starty, startx);
    box(main_Window, 0, 0);
}


int snek(int socket) {
    initialize();
    int snakeLengthP1 = 3;
    int snakeLengthP2 = 3;
    int keyP1 = 'd';
    int keyP2 = 'd';
    position fruit;

    position *snakeP1 = malloc((snakeLengthP1 * 2) * sizeof(int));//allocation for snake
    position *snakeP2 = malloc((snakeLengthP2 * 2) * sizeof(int));//allocation for snake


    int buffer_w[READ_BUFFER_LENGTH];
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
    printSnake(snakeP1, snakeLengthP1);
    printSnake(snakeP2, snakeLengthP2);
    wrefresh(main_Window);
    while (1) {
        int in = wgetch(main_Window);
        box(main_Window, 0, 0);
        if (in != ERR)
            keyP1 = in;
        bzero(buffer, WRITE_BUFFER_LENGTH * sizeof(int));
        n = read(socket, buffer, WRITE_BUFFER_LENGTH * sizeof(int));
        keyP2 = buffer[0];
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

        directionOfSnake(keyP2, snakeP1);
        directionOfSnake(keyP1, snakeP2);


        printSnake(snakeP1, snakeLengthP1);
        printSnake(snakeP2, snakeLengthP2);


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
            snakeP1[0].y > AREA_SIZE_HEIGHT - 2 || selfColision(snakeP1, snakeLengthP1) == ERR) {
            endwin();
            free(snakeP1);
            exit(0);
        }
        if (snakeP2[0].x < 1 || snakeP2[0].x > AREA_SIZE_WIDTH - 2 || snakeP2[0].y < 1 ||
            snakeP2[0].y > AREA_SIZE_HEIGHT - 2 || selfColision(snakeP2, snakeLengthP2) == ERR) {
            endwin();
            free(snakeP2);
            exit(0);
        }
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
                    //buffer_w[i] = -1;
                    //buffer_w[i + 1] = -1;
                }
            }else{
                buffer_w[i] = fruit.x;
                buffer_w[i + 1] = fruit.y;
                buffer_w[i + 2] = -1;
                buffer_w[i + 3] = -1;
                buffer_w[i + 4] = 1;
                buffer_w[i + 5] = 0;
                buffer_w[i + 6] = -1;
                buffer_w[i + 7] = -1;
            }
        }
        int n = write(socket, buffer_w, READ_BUFFER_LENGTH);
        if (n < 0) {
            perror("Error writing to socket");
        }
        wrefresh(main_Window);
        }
        endwin();
    }

    int main(int argc, char *argv[]) {
        printf("omg");
        int sockfd, newsockfd;
        socklen_t cli_len;
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        char buffer[256];
        printf("omg");
        if (argc < 2)
        {
            fprintf(stderr,"usage %s port\n", argv[0]);
            return 1;
        }

        bzero((char*)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(atoi(argv[1]));
        printf("omg");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("Error creating socket");
            return 1;
        }
        printf("omg");
        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("Error binding socket address");
            return 2;
        }
        printf("omg");
        listen(sockfd, 5);
        cli_len = sizeof(cli_addr);

        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return 3;
        }
        /*printf("omg");
        bzero(buffer,256);
        n = read(newsockfd, buffer, 255);*/
        /*if (n < 0)
        {
            perror("Error reading from socket");
            return 4;
        }
        printf("Here is the message: %s\n", buffer);

        const char* msg = "I got your message";
        n = write(newsockfd, msg, strlen(msg)+1);
        if (n < 0)
        {
            perror("Error writing to socket");
            return 5;
        }*/


        snek(newsockfd);

        endwin();
        close(newsockfd);
        close(sockfd);

        return 0;
    }