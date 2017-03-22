/*
 * Turn_90.cpp
 *
 * Created: 2/1/2017 12:22:54 PM
 *  Author: Dung
 */ 


#include "function.h"

void case_10(void);
void case_11(void);
void case_12(void);
void case_20(void);
void case_21(void);
void case_31(void);
void case_32(void);
void case_41(void);
void case_42(void);
void case_51(void);
void case_52(void);
void case_61(void);
void case_62(void);
void case_100(void);

int check_line(void);
bool check_rightline(void);
bool check_leftline(void);

uint8_t  pattern, k = 0;
volatile uint16_t  cnt, cnt_temp, pulse_v, count = 0;
extern uint16_t pulse_ratio;

int8_t vitri = 0;
int8_t state = -1;

#define PERCENT_TURN_90		30	// percent of motor when turn 90 degree
#define PERCENT_CHANGE_LINE	85		// percent of motor when change line
#define PERCENT_11	90		//percent for state 11
#define PERCENT_12	90		//percent for state 12
#define TIME_DELAY_RIGHT 2
#define TIME_DELAY_LEFT 2

#define time_checkline		50		// delay (ms) waiting check line

int main(void)
{
	INIT();
	sel_mode();
	///////////////////////////////////////////////////////////////////////
	
	get_speed();

	///////////////////////////////////////////////////////////////////////
	
	pattern = 10;
	count = 0;
	
	while(1)
	{
		if (sensor_cmp(0xff) == 0xff) count++;
		if (count == 50) 
			{
				case_100();
				break;
			}
		switch(pattern)
		{
			case 10:							// Run STRANGE
			{
				state = check_line();		
				if (state == 0)
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 20;
					k = 0;
					break;
				}
				if (state == 1)
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 51;
					break;
				}
				if (state == 2)
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 61;
					break;
				}
 	
				led7(10);
				case_10();
				break;				
			}							
			
			case 11:							// Turn RIGHT
			{
				led7(11);
				case_11();
				break;
			}			
			
			case 12:							// Turn LEFT
			{
				led7(12);
				case_12();			
				break;
			}			
			
			/***** Turn 90 degree & No line ******/			
			
			case 20:							// CROSS line waiting
			{
				if ( check_rightline() )
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 31;
					break;
				}
				if ( check_leftline() )
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 41;
					break;
				}
				if (sensor_cmp(0xff) == 0x00)
				{
					cnt = 0;
					pulse_v = 0;
					pattern = 21;
					break;
				}
				
				led7(20);
				case_20();
				break;
			}
			
			case 21:
			{
				led7(21);
				case_21();
				break;
			}
			
			case 31:			// Turn 90 degree RIGHT
			{
				led7(31);
				case_31();
				break;
			}
			
			case 32:
				led7(32);
				case_32();
				break;
						
			case 41:			// Turn 90 degree LEFT
			{
				led7(41);
				case_41();
				break;
			}
			
			case 42:
				led7(42);
				case_42();
				break;
						
			/************ CHANGE LINE ************/			
			
			case 51:							// RIGHT line waiting
			{
				led7(51);
				case_51();
				break;
			}
						
			case 52:							// Change RIGHT line
			{
				led7(52);
				case_52();
				break;
			}
						
			case 61:							// LEFT line waiting
			{
				led7(61);
				case_61();
				break;
			}
						
			case 62:							// Change LEFT line
			{
				led7(62);
				case_62();
				break;
			}			
			
			 case 100:
				led7(100);
				case_100();
				break;
 		}
	}
}		

/************** INTERRUPT FUNTION *******************/
ISR(TIMER0_COMP_vect)
{
	cnt++;
	cnt_temp++;
	cal_ratio();
	print();			//Quét LED7 doan
}
ISR(INT0_vect) 
{
	pulse_v++;
	pulse_ratio++;
}

/*************** CHECK LINE FUNCTION ****************/
int check_line()
{
	/*************** CHECK RIGHT LINE ****************/
	uint8_t t = sensor_cmp(0b00111111);
	if( (t==0b00001111) || (t==0b00111111) || (t==0b00011111) || (t==0b00000111))
	{
		cnt_temp = 0;
		while(cnt_temp <= time_checkline)
		{
			switch (sensor_cmp(0xFF))
			{
				case 0b11111111:
				_delay_ms(time_checkline);
				return 0;
				break;
				
				case 0b01100000:
				case 0b00110000:
				case 0b00011000:
				case 0b00001100:
				case 0b00000110:
				return 1;
				break;
			}
		}
	}
	/*************** CHECK LEFT LINE ****************/
	t = sensor_cmp(0b11111100);
	if( (t==0b11110000) || (t==0b11111100) || (t==0b11111000) || (t==0b11100000))
	{
		cnt_temp = 0;
		while(cnt_temp <= time_checkline)
		{
			switch (sensor_cmp(0xFF))
			{
				case 0b11111111:
				_delay_ms(time_checkline);
				return 0;
				break;
				
				case 0b01100000:
				case 0b00110000:
				case 0b00011000:
				case 0b00001100:
				case 0b00000110:
				return 2;
				break;
			}			
		}
	}
	/*************** CHECK CROSS LINE ****************/
	if(sensor_cmp(0b01111110)==0b01111110)
	{
		_delay_ms(time_checkline);
		return 0;
	}
	
	return -1;
}

bool check_rightline()
{   
	uint8_t t = sensor_cmp(0b00111111);
	if( (t==0b00111111) || (t==0b00011111) || (t==0b00001111) || (t==0b00000111) )
		{ return true; }
	
	return false;
}

bool check_leftline()
{
	uint8_t t = sensor_cmp(0b11111100);
	if( (t==0b11100000) || (t==0b11110000) || (t==0b11111000) || (t==0b11111100) ) 
		{ return true; }
	
	return false;
}

/*************** CONTROL FUNCTION ******************/
			
void case_10()			// Run STRANGE
{
		
}

void case_11()			// Turn RIGHT
{
	
}

void case_12()			// Turn LEFT
{
	
}

void case_20()			// CROSS line waiting
{	
	
}

void case_21()			// NO line
{
	
}

void case_31()			// Turn 90 degree RIGHT
{
	
}

void case_32()
{
	
}

void case_41()			// Turn 90 degree LEFT
{
	
}

void case_42()
{
	
}

void case_51()			// RIGHT line waiting
{
	
}			

void case_52()			// Change RIGHT line
{
	
}

void case_61()			// LEFT line waiting
{
	
}

void case_62()			// Change LEFT line
{
	
}

 void case_100()
 {
	 handle(0);
	 speed(0,0,0);
 }