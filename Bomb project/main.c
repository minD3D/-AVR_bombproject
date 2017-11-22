/*
 * LCD_Joystick.c
 *
 * Created: 2017-11-15 오후 2:07:59
 * Author : gomdo
 */ 
 /*
 * TIMER PROJECT
 * Created: 2017-11-01 오후 12:53:17
 * Author : MINJI KIM
 */ 
 
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #define  F_CPU 14745600UL
 
 #include "_main.h"
 #include "_glcd.h"
 
 
 unsigned int Data_ADC3 = 0; //조이스틱(상하) 데이터 값 저장
 unsigned int Data_ADC4 = 0; //조이스틱(좌우) 데이터 값 저장
 #define ADC_VREF_TYPE 0x00      // A/D 컨버터 사용 기준 전압  REFS 설정
 #define ADC_AVCC_TYPE 0x40      // A/D 컨버터 사용 기준 전압  AVCC 설정
 #define ADC_RES_TYPE  0x80      // A/D 컨버터 사용 기준 전압  RES 설정
 #define ADC_2_56_TYPE 0xC0      // A/D 컨버터 사용 기준 전압  2.56 설정

 unsigned char led=0xfe;
 char togglemode,resetbtn,stopbtn;
 int togglecnt=0;
 int cnt;
 
 SIGNAL(INT0_vect)
{
	togglemode=~togglemode;
}


void Interrupt_init(void){
	EICRA = 0x02;
	EIMSK = 0x01;
	DDRD = 0x00;
}

void Port_init(void) { //포트 초기화 구문입니다.
	PORTA = 0x00; 	DDRA = 0xFF;			// PORTA 출력 LOW ,핀의 출력 설정
	PORTB = 0xFF; 	DDRB = 0b11111111;		// PORTB 출력 LOW ,핀의 출력 설정
	PORTC = 0x00; 	DDRC = 0xF0;			// PORTC 상위 4bit 입력, 하위 4bit 출력
	PORTD = 0x80;	DDRD = 0b10000000;		// 핀의 입출력 설정
	PORTE = 0x00; 	DDRE = 0xFF;			// PORTE 출력 LOW ,핀의 출력 설정
	PORTF = 0x00; 	DDRF = 0x00;			// PORTF 출력 LOW ,핀의 출력 설정
}


void init_devices(void)	{ // 초기화 함수를 여기에 넣습니다.
	cli(); //disable all interrupts

	Port_init();		// Port 초기화
	Adc_init();
	lcd_init();         // initialize GLCD
	
	sei(); //re-enable interrupts
}

 void screen_display(void){

	 lcd_string(0,0,"====================");
	 lcd_string(1,0,"*   Bouble P O P   *");
	 lcd_string(2,0,"====================");
	 
 }

void bouble_view(void){
		ScreenBuffer_clear();
		lcd_clear();
		Data_ADC3 = Read_Adc_Data(3) / 14; //아날로그 0번 포트 읽기
		Data_ADC4 = Read_Adc_Data(4) / 8; //아날로그 0번 포트 읽기
		
		GLCD_Circle(Data_ADC3, Data_ADC4, 5);
		for(int i=0;i<7;i++){
			GLCD_Dot(Data_ADC3,Data_ADC4+i);
			GLCD_Dot(Data_ADC3+i,Data_ADC4);
			GLCD_Dot(Data_ADC3,Data_ADC4-i);
			GLCD_Dot(Data_ADC3-i,Data_ADC4);
		}
}

void makeCircle(void){
	GLCD_Circle(8, 8, 8);
	GLCD_Circle(10, 1, 8);
	GLCD_Circle(4, 8, 8);

}

 int main(void)
 {
	 init_devices();
	 screen_display();
	 Interrupt_init();
	 DDRB=0xff;
	 PORTB=0x0f;
	 togglemode=0;
	 Data_ADC3=5;
	 Data_ADC4=5;
	 while (1)
	 {
		if(togglemode){
			bouble_view();
			makeCircle();

		}
		else screen_display();
	 }
 }
