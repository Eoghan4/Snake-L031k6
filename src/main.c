/*
Eoghan and Konrad

22/10/24 - Added score, snake-like movement, sprites

23/10/24 - Added random movement for the apple, fixed apple sprite

4/11/24 - Snake Grows, Snake is now a rectangle(of SNAKE_SIZE), tidyied up
		- Added Visual Boarders

TODO:
 - Add Sound
 - Add Game Over when hits wall or tail
 - Multiplayer (Player 1, player 2, compare scores)
 - Homescreen
 - Restarting
 - Serial Port
*/

#include <stm32f031x6.h>
#include <stdlib.h>
#include <time.h>
#include "display.h"
#define SNAKE_SIZE 10
#define SNAKE_MOVE 5
#define APPLE_WIDTH 12
#define APPLE_HEIGHT 16

struct snake{ // Struct for pixels in the snake, simply contains coordinates for each pixel
    int x, y;
};

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void snakeUpdate(int count, int newX, int newY, struct snake snakeArray[100]);

volatile uint32_t milliseconds;

// Sprites
const uint16_t apple1[]=
{
	0,0,0,0,0,24576,24577,16385,57344,0,0,0,0,0,0,0,0,8193,40971,40971,8193,0,0,0,0,0,0,0,8192,40971,57355,8203,40961,0,0,0,0,0,256,26624,43264,16395,24587,522,19968,26624,0,0,0,256,62208,32256,65024,46081,19969,31488,40704,32512,36096,0,0,26880,32256,40704,40704,40704,40704,40704,40704,40704,62208,0,0,35840,32512,40704,32512,40704,40704,40704,40704,40704,5632,0,0,44800,32512,40704,40704,40704,32512,40704,40704,40704,32512,26624,0,5632,40704,32512,40704,40704,32512,32512,40704,40704,40704,62208,8960,23552,40704,32512,40704,40704,40704,32512,32512,40704,32512,5632,9216,32000,40704,40704,40704,40704,40704,40704,40704,32512,32512,53760,256,62208,32512,40704,40704,32512,32512,40704,40704,40704,32256,26880,0,17664,32000,40704,32512,53760,43776,23552,32512,32512,62464,256,0,0,35584,61952,26624,0,8192,17664,53248,44288,512,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

// Letters For Homescreen
const uint16_t s[]=
{
	0,0,0,0,0,24576,8192,57344,0,49152,44816,0,0,0,0,8192,8192,40970,11,24587,24587,24586,44816,44816,0,0,32768,57346,49163,49163,0,49163,49163,44816,32768,0,0,0,40970,40971,57355,40971,16395,16395,49162,8193,24576,0,0,24577,32779,57355,49163,8194,24576,40960,0,0,0,0,0,1,16395,57355,16395,49153,16384,0,0,0,0,0,0,49152,16394,49163,49163,57355,16395,8202,16394,40961,8192,0,0,0,24576,16394,24587,57355,49163,49163,49163,40971,40970,49152,0,0,0,40960,1,11,32779,49163,16395,49163,16395,24577,0,0,0,40960,49152,0,0,16384,32769,32779,49163,8193,0,0,16384,49154,8202,32768,0,0,16385,32779,49163,40961,0,0,32769,24587,40971,8202,16384,32769,57355,49163,32779,32769,0,0,57344,24587,57355,40971,11,24587,49163,49163,57355,32768,0,0,8192,49153,11,49163,49163,49163,24587,8203,32768,0,0,0,0,8192,49152,24586,40970,10,16385,57344,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t n[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40968,8480,8480,32776,0,0,0,0,0,0,24848,33897,27083,19179,10987,42922,288,0,0,0,16384,57904,2243,11259,35835,35835,35835,27643,51667,49720,0,0,24864,51667,27643,27643,51955,51667,10987,27643,27643,59859,49432,16384,59026,27643,27643,27083,9025,32776,16944,43475,27643,11259,49993,41240,18907,27643,2795,8752,0,0,0,42073,19451,19451,1121,25417,3059,27643,10154,24584,0,0,0,25417,11259,19451,25705,9585,27643,11259,33881,0,0,0,0,25417,11259,27643,42089,18074,27643,51955,33336,0,0,0,0,17217,3067,27643,25977,42930,27643,27371,33064,0,0,0,0,577,3059,27643,1410,2226,27643,19179,8480,0,0,0,0,41528,60147,27643,18058,26538,27643,2787,288,0,0,0,8192,57920,11259,27643,26250,58465,35291,10154,49160,0,0,0,8192,16672,35291,3059,58465,24584,33064,16400,0,0,0,0,16384,8192,33336,49993,57352,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t a[]=
{
	0,0,0,0,32768,57856,8448,0,0,0,0,0,0,0,0,8448,44803,30981,5893,12035,25857,8192,0,0,0,0,0,44034,15878,7942,7942,57094,64517,43522,16384,0,0,0,57856,30981,65286,32518,48646,57094,65286,15366,50433,0,0,0,35586,24326,57094,13828,42753,13828,57094,57094,45059,0,0,24832,5893,65286,48646,59649,0,50433,7430,65286,30468,49408,0,42241,7430,65286,14597,24832,0,49920,39685,65286,39173,24832,0,35330,32518,48902,45059,0,0,16896,31237,7942,39685,49920,24576,61955,65286,24326,43778,8704,256,40960,46596,7942,48646,42497,17152,23301,7942,7430,31237,47621,22789,38148,46596,48902,32518,2562,43522,15878,65286,31750,65286,65286,7942,65286,40710,32518,57094,61699,45315,57094,24326,29187,5893,31237,64517,15622,48390,40454,65286,22020,5636,7942,7430,25857,24832,25088,58112,50433,51457,31750,65286,38917,46084,57094,22789,512,0,0,0,0,17665,7430,65286,30212,9985,37123,51457,16384,0,0,0,0,50176,39685,48902,37123,8192,49408,16384,0,0,0,0,0,40960,60162,53251,58368,
};

const uint16_t k[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,1024,1024,0,0,0,0,0,0,256,0,0,1280,6656,6912,1536,0,0,0,0,2560,5632,3328,256,2048,7680,7936,4096,0,0,0,2048,7168,7936,6400,512,1280,7424,7936,5632,0,0,2560,7168,7936,7936,5120,0,768,6656,7936,6656,768,2560,7424,7936,7936,6400,1280,0,0,5376,7936,7680,3840,7168,7936,7936,5120,768,0,0,0,3584,7936,7936,7424,7936,7936,5120,512,0,0,0,0,2048,7680,7936,7936,7936,7680,3328,0,0,0,0,0,1024,7424,7936,7168,7680,7936,7168,1536,0,0,0,0,512,6400,7936,7168,5120,7936,7936,5632,512,0,0,0,0,5376,7936,7680,2048,6656,7936,7680,3072,0,0,0,0,4864,7936,7680,1536,2816,7680,7936,7168,2304,0,0,0,5120,7936,7680,1536,256,4864,7936,7936,6400,768,0,256,5120,7936,7168,1024,0,512,5376,7936,7680,1792,0,0,2304,5632,3840,256,0,0,1024,4608,4352,512,
};

const uint16_t e[]=
{
	0,0,0,0,8192,57344,32768,32768,32768,24576,40960,0,57344,25090,33795,25860,50693,42501,34565,42757,9990,42757,17411,40960,1285,43271,35079,43271,51463,51463,51463,51463,51463,43271,34823,41217,59655,51463,26887,35079,2055,34823,2055,59399,34823,43014,9476,24576,10503,51463,9733,16897,257,16641,41217,33025,33025,16641,57344,0,34823,51463,9990,16384,0,0,0,0,0,0,0,0,1798,51463,18439,41217,8192,770,33796,513,24576,0,0,0,58628,43271,10503,17668,1798,51463,43271,34052,57344,0,0,0,49922,18695,43271,26887,51463,51463,10247,1027,57344,0,0,0,49665,2055,51463,43271,10247,58885,17154,0,0,0,0,0,49408,26630,51463,10246,49665,40960,0,0,49152,49152,0,0,8192,18182,51463,59398,57600,0,32768,257,17668,25604,32768,0,32768,59141,51463,2055,41217,32768,41987,26631,35079,59399,33281,0,40960,34309,51463,43271,25604,58885,43271,59655,35079,34565,8192,0,0,1284,51463,51463,35079,51463,51463,2055,9476,41216,8192,0,0,8705,18438,10503,35079,51463,50950,770,0,0,0,0,
};


int main()
{
	srand(time(NULL)); // Initialize "Random" numbers
	struct snake snakeArray[100];
	int score = 1;
	int hmoved = 0; // Has the snake moved horizontally
	int vmoved = 0; // Has the snake moved vertically
	int direction = 0;
	uint16_t apple_x = (rand()%106)+10; // Screen width (128) - 10 (right border) - 12 (apple width) + 10 (left border)
	uint16_t apple_y = (rand()%103)+41; // Screen height (160) - 41 (bottom border) - 16 (apple height) + 41 (top border)
	uint16_t x = 55;
	uint16_t y = 70; 
	initClock();
	initSysTick();
	setupIO();
	
	while(direction==0){ // Home screen, runs when game hasn't started.
		putImage(34,20,12,16,s,0,0);
		putImage(46,20,12,16,n,0,0);
		putImage(58,20,12,16,a,0,0);
		putImage(70,20,12,16,k,0,0);
		putImage(82,20,12,16,e,0,0);
		printText("Press to Start", 10, 50, RGBToWord(0xff,0xff,0), 0);
	}

	fillRectangle(0,0,160,128,0); // Erase Screen
	
	// Draw Borders
	drawLine(9,39,9,150,RGBToWord(200,20,200));
	drawLine(120,39,120,150,RGBToWord(200,20,200));
	drawLine(9,39,120,39,RGBToWord(200,20,200));
	drawLine(9,150,120,150,RGBToWord(200,20,200));

	while(1)
	{
		hmoved = vmoved = 0;

		// move right
		if(direction == 1){
			if (x < 110){
				x = x + SNAKE_MOVE;
				hmoved = 1;
			}	
		}

		// move left
		if(direction == 2){
			if (x > 10){
				x = x - SNAKE_MOVE;
				hmoved = 1;
			}	
		}

		// move down
		if(direction == 3){
			if (y < 140){
				y = y + SNAKE_MOVE;
				hmoved = 1;
			}	
		}

		// move up
		if(direction == 4){
			if (y > 40){
				y = y - SNAKE_MOVE;
				hmoved = 1;
			}	
		}

		// Buttons
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (direction!=2){
				direction = 1;
			}				
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			if (direction!=1){
				direction = 2;
			}	
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			if (direction!=4){
				direction = 3;
			}	
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			if (direction!=3){
				direction = 4;
			}	
		}

		// Update when snake moves:
		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			putImage(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,apple1,0,0);
			// Borders
			drawLine(9,39,9,150,RGBToWord(200,20,200));
			drawLine(120,39,120,150,RGBToWord(200,20,200));
			drawLine(9,39,120,39,RGBToWord(200,20,200));
			drawLine(9,150,120,150,RGBToWord(200,20,200));	
			delay(100);

			snakeUpdate(score, x, y, snakeArray);

			// Now check for an overlap by checking to see if ANY of the 4 corners of snake are within the apple
			if (isInside(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,x,y) || isInside(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,x+SNAKE_SIZE,y) || isInside(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,x,y+SNAKE_SIZE) || isInside(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,x+SNAKE_SIZE,y+SNAKE_SIZE) )
			{
				score +=1;
				fillRectangle(10,20,100,20,0); // Used to stop score text overlapping
				printTextX2("Score:", 1, 20, RGBToWord(0xff,0xff,0), 0);
				printNumberX2(score, 70, 20, RGBToWord(0xff,0xff,0), 0);
				fillRectangle(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,0); // Erase apple
				apple_x = (rand()%106)+10;
				apple_y = (rand()%103)+41;
				putImage(apple_x,apple_y,APPLE_WIDTH,APPLE_HEIGHT,apple1,0,0);
			}
		}		
		delay(50);
	}
	return 0;
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

// SNAKE LOGIC - Draws snake
void snakeUpdate(int count, int newX, int newY, struct snake snakeArray[100]){
    snakeArray[0].x = newX;
    snakeArray[0].y = newY;
	fillRectangle(snakeArray[count].x, snakeArray[count].y, 12, 16, 0);
    for(int i=count;i>0;i--){
        snakeArray[i].x = snakeArray[i-1].x;
        snakeArray[i].y = snakeArray[i-1].y;
        fillRectangle(snakeArray[i].x,snakeArray[i].y,SNAKE_SIZE,SNAKE_SIZE,65535);
    }
}