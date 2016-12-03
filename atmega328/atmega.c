#include <avr/io.h>
#define F_CPU 1000000UL
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void usart_init(void);
unsigned char usart_receive(void);
void usart_send( unsigned char data);
void usart_putstring(char* StringPtr);
int write_to_master(char write);
void generateID(char* data);
void usart_receiveString(char* data);
int ADCsingleREAD(uint8_t adctouse);

char data[6];
int main(void){
 char recieved_byte;
 //char data[6];
  
usart_init();
/*test usart
while(1){
  usart_receive();
  usart_putstring("abcde");
}
end test*/

generateID(data);
timer1_init();
DDRD |= (1<<PD6);   //set pin 6 of Port D to output, debug
sei();

  
  while(1){}
  
    return 0;   /* never reached */
}

ISR(TIMER1_COMPA_vect) { // enable timer compare interrupt
PORTD ^= (1 << PD6); // set pin 6 of Port D as XOR to blink, debug
uint32_t door;
uint32_t light;
door = (uint32_t)ADCsingleREAD(1); //set ADC1
light = (uint32_t)ADCsingleREAD(2);

if(door>0){
  data[1]=0x01;
  data[5]=(door>>24) & 0xFF;
  data[4]=(door>>16) & 0xFF;
  data[3]=(door>>8) & 0xFF;
  data[2]=door & 0xFF;
}else if(light>0){
  data[1]=0x02;
  data[5]=(light>>24) & 0xFF;
  data[4]=(light>>16) & 0xFF;
  data[3]=(light>>8) & 0xFF;
  data[2]=light & 0xFF;
}

usart_putstring(data);
}

int write_to_master(char write) 
{
  SPDR = write;
  while(!(SPSR & (1 << SPIF)));
  return SPDR;
}

void usart_init(void){
    // usart initialization code
  UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
  UBRR0H  = (BAUD_PRESCALE >> 8);
  UBRR0L  = BAUD_PRESCALE;
}

unsigned char usart_receive(void){
    while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
      return UDR0;
}

void usart_send( unsigned char data){
    //wait for register to be ready
    while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
      UDR0 = data;
}

void usart_putstring(char* StringPtr){
    while(*StringPtr != 0x00){
          usart_send(*StringPtr);
              StringPtr++;}
}

void usart_receiveString(char* data){
    int i;
    for(i=0;i<6;i++){
      data[i]=usart_receive();
    }
}

void timer1_init(void){
  OCR1A = 976; //set compare register to 1 seconds count,1000000/1024
  TCCR1B |= (1<<WGM12);  //set  The timer mode to CTC
  TCCR1B |= (1<<CS12) | (1<<CS10);  //select the clock prescaler 1024
  TIMSK1 |= (1 <<OCIE1A); //Set interrupt on compare match 
}

void generateID(char* data){
  char idRequest[6];
  char idReceive[6];



idRequest[0]='0';
idRequest[1]='0';
//idRequest[1]='a';
//idRequest[2]='b';
//idRequest[3]='c';
//idRequest[4]='d';
srand(time(NULL));
uint32_t temp = rand();
idRequest[5]=(temp>>24) & 0xFF;
idRequest[4]=(temp>>16) & 0xFF;
idRequest[3]=(temp>>8) & 0xFF;
idRequest[2]=temp & 0xFF;

usart_receive();
usart_putstring(idRequest);

while(1){
  usart_receiveString(idReceive);
  if(idReceive[2]==idRequest[2]&&idReceive[3]==idRequest[3]
    &&idReceive[4]==idRequest[4]&&idReceive[5]==idRequest[5]){
    data[0]=idReceive[0];
  //test received string
  usart_putstring(idReceive);
  usart_putstring(data);

  //test end
    break;
  }
}

}

int ADCsingleREAD(uint8_t adctouse){
    int ADCval;

    ADMUX = adctouse;
    ADMUX |=(1 << REFS0);
    ADMUX &= ~(1 << ADLAR);

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);

    ADCSRA |= (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    //ADCval=ADC;
    ADCval = ADCL;
    ADCval = (ADCH << 8) + ADCval;

    return ADCval;
}
