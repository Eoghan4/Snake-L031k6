/*
Eoghan and Konrad

22/10/24 - Added score, snake-like movement, sprites

23/10/24 - Added random movement for the apple, fixed apple sprite

TDODO: 
	1.
	Make snake a square rather then rectangke, will make 
	extending the snake easier.

	2.
	Make the snake able to grow. (array of coordinates?)
*/

#include <stm32f031x6.h>
#include <stdlib.h>
#include <time.h>
#include "display.h"
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

volatile uint32_t milliseconds;

// Sprites
const uint16_t snake1[]=
{
65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,
};
const uint16_t snake2[]= 
	{
65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,
	};
const uint16_t snake3[]= 
{
65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,
};
const uint16_t apple1[]=
{
	0,0,0,0,0,24576,24577,16385,57344,0,0,0,0,0,0,0,0,8193,40971,40971,8193,0,0,0,0,0,0,0,8192,40971,57355,8203,40961,0,0,0,0,0,256,26624,43264,16395,24587,522,19968,26624,0,0,0,256,62208,32256,65024,46081,19969,31488,40704,32512,36096,0,0,26880,32256,40704,40704,40704,40704,40704,40704,40704,62208,0,0,35840,32512,40704,32512,40704,40704,40704,40704,40704,5632,0,0,44800,32512,40704,40704,40704,32512,40704,40704,40704,32512,26624,0,5632,40704,32512,40704,40704,32512,32512,40704,40704,40704,62208,8960,23552,40704,32512,40704,40704,40704,32512,32512,40704,32512,5632,9216,32000,40704,40704,40704,40704,40704,40704,40704,32512,32512,53760,256,62208,32512,40704,40704,32512,32512,40704,40704,40704,32256,26880,0,17664,32000,40704,32512,53760,43776,23552,32512,32512,62464,256,0,0,35584,61952,26624,0,8192,17664,53248,44288,512,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int main()
{
	srand(time(NULL)); // Initialize "Random" numbers
	int score = 0; 
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	int direction = 0;
	uint16_t apple_x = (rand()%106)+10; // Screen width (128) - 10 (right border) - 12 (snake width) + 10 (left border)
	uint16_t apple_y = (rand()%103)+41; // Screen height (160) - 41 (bottom border) - 16 (sbake height) + 41 (top border)
	uint16_t x = 55; // Roughly Middle of screen
	uint16_t y = 70;
	uint16_t oldx = x;
	uint16_t oldy = y;
	initClock();
	initSysTick();
	setupIO();
	putImage(x,y,12,16,snake1,0,0);
	printText("Press to Start", 10, 20, RGBToWord(0xff,0xff,0), 0);
	while(1)
	{
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		// move right
		if(direction == 1){
			if (x < 110){
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}	
		}

		// move left
		if(direction == 2){
			if (x > 10){
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}	
		}

		// move down
		if(direction == 3){
			if (y < 140){
				y = y + 1;
				hmoved = 1;
				hinverted=0;
			}	
		}

		// move up
		if(direction == 4){
			if (y > 40){
				y = y - 1;
				hmoved = 1;
				hinverted=1;
			}	
		}

		// Buttons
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			direction = 1;				
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			direction = 2;
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			direction = 3;
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			direction = 4;
		}

		// Update when snake moves:
		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			putImage(apple_x,apple_y,12,16,apple1,0,0);
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle)
					putImage(x,y,12,16,snake1,hinverted,0);
				else
					putImage(x,y,12,16,snake2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else
			{
				putImage(x,y,12,16,snake3,0,vinverted);
			}
			// Now check for an overlap by checking to see if ANY of the 4 corners of snake are within the target area
			if (isInside(apple_x,apple_y,12,16,x,y) || isInside(apple_x,apple_y,12,16,x+12,y) || isInside(apple_x,apple_y,12,16,x,y+16) || isInside(apple_x,apple_y,12,16,x+12,y+16) )
			{
				score +=1;
				fillRectangle(10,20,100,20,0); // Used to stop score text overlapping
				printTextX2("Score:", 1, 20, RGBToWord(0xff,0xff,0), 0);
				printNumberX2(score, 70, 20, RGBToWord(0xff,0xff,0), 0);
				fillRectangle(apple_x,apple_y,12,16,0); // Erase apple
				apple_x = (rand()%106)+10;
				apple_y = (rand()%103)+41;
				putImage(apple_x,apple_y,12,16,apple1,0,0);
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