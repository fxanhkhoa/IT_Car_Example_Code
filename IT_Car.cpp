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
	switch(sensor_cmp(0b01111110))
	{
		case 0b00011000:
		handle(0);
		speed(100,100, 100);
		break;
		
		/******************************/			//lech trai 1
		case 0b00001000:
		handle(3);
		speed(100,100, 100);
		break;
				
		case 0b00010000:							//lech phai 1
		handle(-3);
		speed(100,100, 100);
		break;
		
		/******************************/			//lech trai 2
		case 0b00001100:
		handle(5);
		speed(100,100, 100);
		break;

		case 0b00110000:							//lech phai 2
		handle(-5);
		speed(100,100, 100);
		break;
		
		/******************************/			//lech trai 3
		case 0b00000100:
		handle(7);
		speed(100,90, 95);
		break;
		
		case 0b00100000:							//lech phai 3
		handle(-7);
		speed(90,100, 95);
		break;
		
		/******************************/			//lech trai goc lon
		case 0b00000110:
		case 0b00000010:
		handle(10);
		speed(100,80, 90);
		pattern = 11;
		break;
		
		/******************************/			//lech phai goc lon
		case 0b01100000:
		case 0b01000000:
		handle(-10);
		speed(80,100, 90);
		pattern = 12;
		break;
	}
}

void case_11()			// Turn RIGHT
{
	//switch (sensor_cmp(0b11111111))
	//{
		//case 0b11000000:
		//case 0b10000000:
		//handle(25);
		//speed(100,0, 100);
		//fast_brake_right();
		//break;
		//
		//case 0b00000001:
		//case 0b10000001:
		//handle(23);
		//speed(100,0, 100);
		//fast_brake_right();
		//break;
		//
		//case 0b00000011:
		//handle(20);
		//speed(100,0, 100);
		//break;
		//
		//case 0b00000010:
		//handle(17);
		//speed(100,20, 100);
		//break;
		//
		//case 0b00000110:
		//handle(15);
		//speed(100,50, 100);		
		//break;
		//
		//case 0b00000100:
		//handle(13);
		//speed(100,70, 100);
		//break;
		//
		//case 0b00001100:
		//handle(10);
		//speed(100,90, 100);
		//break;
		//
		//case 0b00001000:		
		//case 0b00011000:
		//case 0b00010000:
		//handle(7);
		//speed(100,100, 100);
		//pattern = 10;
		//break;
		//
		//default:
		//handle(30);
		//speed(100,0, 100);
		//break;
	//}
	//if (check_leftline())
	//{
		//pattern = 61;
		//return;
	//}
	//if (check_rightline())
	//{
		//pattern = 51;
		//return;
	//}
	if (check_line() == 0)
	{
		pattern = 20;
		k = 0;
		return;
	}
	switch(sensor_cmp(0b11001100))
	{
		case 0b11000000:
		speed(90,-10,PERCENT_11);
		handle(25);
		break;
		case 0b10000000:
		speed(90,0,PERCENT_11);
		handle(19);
		break;
		case 0b00000000:
		speed(90,5,PERCENT_11);
		handle(17);
		break;
		case 0b00000100:
		speed(90,10,PERCENT_11);
		handle(15);
		break;
		case 0b00001100:
		speed(90,15,PERCENT_11);
		handle(13);
		pattern=10;
		break;
		default: pattern=11;
		break;
	}
}

void case_12()			// Turn LEFT
{
	//switch(sensor_cmp(0b11111111))
	//{
		//case 0b00000011:
		//case 0b00000001:
		//handle(-45);
		//speed(0,100, 100);
		//fast_brake_left();
		//break;
		//
		//case 0b10000000:
		//case 0b10000001:
		//handle(-23);
		//speed(0,100, 100);
		//fast_brake_left();
		//break;
		//
		//case 0b11000000:
		//handle(-20);
		//speed(0,100, 100);
		//break;
		//
		//case 0b01000000:
		//handle(-17);
		//speed(20,100, 100);
		//break;
		//
		//case 0b01100000:
		//handle(-15);
		//speed(50,100, 100);
		//break;
		//
		//case 0b00100000:
		//handle(-13);
		//speed(70,100, 100);
		//break;
		//
		//case 0b00110000:
		//handle(-10);
		//speed(90,100, 100);
		//break;
		//
		//case 0b00010000:
		//case 0b00011000:
		//case 0b00001000:
		//handle(-7);
		//speed(100,100, 100);
		//pattern = 10;
		//break;
		//
		//default:
		//handle(-30);
		//speed(0,100, 100);
		//break;
		//
	//}
	//if (check_leftline())
	//{
		//pattern = 61;
		//return;
	//}
	//if (check_rightline())
	//{
		//pattern = 51;
		//return;
	//}
	if (check_line() == 0)
	{
		pattern = 20;
		k = 0;
		return;
	}
	switch(sensor_cmp(0b00110011))
	{
		case 0b00000011:
		speed(-10,90,PERCENT_12);
		handle(-25);
		break;
		case 0b00000001:
		speed(0,90,PERCENT_12);
		handle(-19);
		break;
		case 0b00000000:
		speed(5,90,PERCENT_12);
		handle(-17);
		break;
		case 0b00100000:
		speed(10,90,PERCENT_12);
		handle(-15);
		break;
		case 0b00110000:
		speed(15,90,PERCENT_12);
		handle(-13);
		pattern=10;
		break;
		
		default: pattern=12;
		break;
	}
}

void case_20()			// CROSS line waiting
{	
	led7(20);
	if (k == 0)
	{
		speed(100,100,-70);
		cnt = 0;
		while (cnt < 100);
		k = 1;
	}
	//if (cnt > 300) case_100();
	switch(sensor_cmp(0b01111110))
	{
		case 0b00011000:
		handle(-2);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		/******************************/			//lech trai 1
		case 0b00001000:
		handle(0);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		case 0b00010000:							//lech phai 1
		handle(-4);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		/******************************/			//lech trai 2
		case 0b00001100:
		handle(1);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		case 0b00110000:							//lech phai 2
		handle(-5);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		/******************************/			//lech trai 3
		case 0b00000100:
		handle(3);
		speed(90,90, PERCENT_TURN_90);
		break;
		
		case 0b00100000:							//lech phai 3
		handle(-7);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		/******************************/
		
		case 0b00000110:
		handle(4);
		speed(90, 90, PERCENT_TURN_90);
		break;
		
		case 0b01100000:
		handle(-8);
		speed(80,100, PERCENT_TURN_90);
		break;
		
		
	}
}

void case_21()			// NO line
{
	switch (sensor_cmp(0b01111110))
	{
		case 0b00000000:
			handle(0);
			speed(100,100,100);
		break;
		case 0b01111110:
		speed(100,100,100);
		break;
		case 0b00011000:
		case 0b00110000:
		case 0b00111000:
		case 0b00001100:
		case 0b00011100:
		pattern=10;
		break;
		//case 0b01100000:
		//case 0b01110000:
			//handle(2);
			//speed(100,80,100);
			//pattern = 10;
			//break;
		default:
		break;
	}
	switch (sensor_cmp(0b11100000))
	{
		case 0b10000000:
		handle(18);
		speed(100,100,100);
		break;
		case 0b11000000:
		handle(25);
		speed(100,100,100);
		break;
		case 0b11100000:
		handle(27);
		speed(100,100,100);
		break;
		default:
		break;
	}
	
	switch (sensor_cmp(0b00000111))
	{
		case 0b00000001:
		handle(-18);
		speed(100,100,100);
		break;
		case 0b00000011:
		handle(-25);
		speed(100,100,100);
		break;
		case 0b00000111:
		handle(-27);
		speed(100,100,100);
		break;
		default:
		break;
	}
}

void case_31()			// Turn 90 degree RIGHT
{
	led7(31);
	/*while(cnt <= 40)
	{
		handle(35);
		speed(-100,100, 100);
	}*/
	switch (sensor_cmp(0b11111111))
	{
		case 0x00:
		//speed(100, 100, 50);
		pattern = 32;
		break;
		
		default:
		//handle(30);
		//speed(100, -10, 100);
		break;
	}
}

void case_32()
{
	switch (sensor_cmp(0b11111000))
	{
		case 0b00110000:
		case 0b01100000:
		case 0b11100000:
		case 0b01110000:
		case 0b00011000:
		//cnt_straight = 0;
		handle(35);
		speed(100,-10,100);
		cnt = 0;
		while (cnt < 5);
		k = 0;
		while ( k == 0)
		{
			switch (sensor_cmp(0b11111000))
			{
				case 0b00110000:
				case 0b01100000:
				case 0b11100000:
				case 0b01110000:
				case 0b00011000:
				pattern = 10;
				k = 1;
				break;
				default:
				handle(35);
				speed(100,-10,100);
				break;
			}
		}
		break;
		default:
		handle(35);
		speed(100,-10,100);
		cnt = 0;
		while (cnt < TIME_DELAY_RIGHT);
		break;
	}
}

void case_41()			// Turn 90 degree LEFT
{
	led7(41);
	/*while(cnt <= 40)
	{
		handle(-35);
		speed(100,-100, 100);
	}*/
	switch (sensor_cmp(0b11111111))
	{
		case 0x00:
		pattern = 42;
		break;
		
		default:
		//handle(-30);
		//speed(-20,100, 100);
		//pattern = 42;
		break;
	}
}

void case_42()
{
	switch (sensor_cmp(0b00011111))
	{
		case 0b00001100:
		case 0b00011000:
		case 0b00001110:
		case 0b00000110:
		case 0b00000111:
		handle(-35);
		speed(-10,100,100);
		cnt = 0;
		while (cnt < 5);
		k = 0;
		while (k == 0)
		{
			switch (sensor_cmp(0b00011111))
			{
				case 0b00001100:
				case 0b00011000:
				case 0b00001110:
				case 0b00000110:
				case 0b00000111:
				pattern = 10;
				k = 1;
				break;
				default:
				handle(-35);
				speed(-10,100,100);
				break;
			}
		}
		//cnt_straight = 0;
		break;
		default:
		handle(-35);
		speed(-10,100,100);
		cnt = 0;
		while (cnt < TIME_DELAY_LEFT);
		//_delay_ms(100);
		break;
	}
}

void case_51()			// RIGHT line waiting
{
	led7(51);
	if (sensor_cmp(0b11111111) == 0x00)
	{
		cnt = 0;
		pattern = 52;
	}		
	else
	{
		handle(10);
		speed(100,100, PERCENT_CHANGE_LINE);
	}		
}			

void case_52()			// Change RIGHT line
{
	led7(52);
	switch(sensor_cmp(0b11110000))
	{
		case 0b00110000:
		case 0b01110000:
		case 0b01100000:
		case 0b00010000:
		case 0b00100000:
		//handle(-10);
		cnt = 0;
		pattern = 10;
		break;
		
		default:
		handle(10);
		speed(100,100, PERCENT_CHANGE_LINE);
		break;
		
	}
}

void case_61()			// LEFT line waiting
{
	if (sensor_cmp(0b11111111) == 0x00)
	{
		cnt = 0;
		pattern = 62;
	}
	else 
	{
		handle(-10);
		speed(100,100, PERCENT_CHANGE_LINE);
	}		
}

void case_62()			// Change LEFT line
{
	switch(sensor_cmp(0b00001111))
	{
		case 0b00001100:
		case 0b00001110:
		case 0b00001000:
		case 0b00000100:
		//handle(10);
		cnt = 0;
		pattern = 10;
		break;
		
		default:
		handle(-10);
		speed(100,100, PERCENT_CHANGE_LINE);
		break;
		
	}
}

 void case_100()
 {
	 handle(0);
	 speed(0,0,0);
 }