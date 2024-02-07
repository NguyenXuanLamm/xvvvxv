#include <16F877A.h>
#device ADC=10
#use delay(clock = 20000000)  //Su dung thach anh 20Mhz  
#fuses HS,NOWDT,PROTECT,BROWNOUT,PUT,NOLVP,NOCPD,NOWRT,NOCPD   //Fuse bituse fast_io(C)
#use fast_io(D)
#define LCD_DB4   PIN_B3
#define LCD_DB5   PIN_B2
#define LCD_DB6   PIN_B1
#define LCD_DB7   PIN_B0

#define LCD_E     PIN_B4
#define LCD_RS    PIN_B5
#include <lcd16x2.c>
//hx711
#define ADDO      PIN_D0 
#define ADSK      PIN_C3

//read weigh loadcell
unsigned int32 ReadCount(){ 
  unsigned int32 Count = 0; 
  unsigned int8 i,  convert_1,  convert_2,  convert_3; 
  output_high(ADDO);            
  output_low(ADSK); 
  Count=0; 
  while(input(ADDO)); 
  for (i=0;i<24;i++){ 
    output_high(ADSK); 
    Count=Count<<1; 
    output_low(ADSK);
    if(input(ADDO)) Count++; 
  } 
  output_high(ADSK); 
  Count=Count|0x80;  
  output_low(ADSK); 
  convert_1=MAKE8(Count, 0);
  convert_2=MAKE8(Count, 1); 
  convert_3=MAKE8(Count, 2);
  convert_2=(convert_2 & 0b11111000);
  Count= MAKE16(convert_3, convert_2);
  return(Count); 
}
unsigned int donvi ,chuc ,tram ,nghin;
void main(){
   lcd_init();
   unsigned int16 weighs, weigh;
   lcd_gotoxy(1,1);
   printf(lcd_putc, " Load Cell HX711 ");
   delay_ms(1000);

   weigh = ReadCount();  
   while(true){      
         lcd_gotoxy(1,1);
         //printf(lcd_putc, " -------------- ");
         printf(lcd_putc, " Load Cell HX711 ");
         lcd_gotoxy(1,2);
         weighs = ReadCount()-weigh;
         if(weighs > 10000)
         {
            weighs = 0;  
            printf(lcd_putc, " Weigh %lu g        ", weighs);
         }
         else
            if(weighs < 1000){
               printf(lcd_putc, " Weigh %lu g        ", weighs);
            }
            else{
               donvi =  weighs % 10; 
               chuc  = (weighs/10) % 10; 
               tram  = (weighs/100) % 10; 
               nghin = (weighs/1000) % 10; 
               printf(lcd_putc, " Weigh %d.%d%d%d Kg", nghin, tram, chuc, donvi);
            }
            

   }
}

