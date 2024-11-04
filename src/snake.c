/*
Testing that the snake growing function works

The main function shows a "snake" and its x and y postition
The score is updated every 10 seconds, adding another pixel
to the end of the snake, trailing behind.
*/

#include <stdio.h>
#include <time.h>

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

struct snake{
    int x, y;
};

void snakeUpdate(int count, int newX, int newY, struct snake snakeArray[100]){
    snakeArray[0].x = newX;
    snakeArray[0].y = newY;
    for(int i=count;i>0;i--){
        snakeArray[i].x = snakeArray[i-1].x;
        snakeArray[i].y = snakeArray[i-1].y;
        // PutImage();
    }
}

void main(){
    int score = 1;
    int x = 10;
    int y = 10;
    struct snake snake1[100] = {};
    snake1[0].x = x;
    snake1[0].y = y;
    while(1){
        x++;
        y++;
        if(!(x%10)){
            score++;
        }
        snakeUpdate(score,x,y,snake1);
        for(int i=0; i<score; i++){
            printf("Pixel %d: X = %d, Y = %d\n",i, snake1[i].x, snake1[i].y);
        }
        delay(1);
    }
}