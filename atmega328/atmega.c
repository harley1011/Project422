#include <avr/io.h>
#define F_CPU 1000000UL
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
#include <string.h>
#include <time.h>
#include <stdlib.h>

void usart_init(void);
unsigned char usart_receive(void);
void usart_send( unsigned char data);
void usart_putstring(char* StringPtr);
int write_to_master(char write);

int main(void){
 char recieved_byte;
 char data[6];
  
usart_init();

generateID(data);





  
  while(1){}
  
    return 0;   /* never reached */
}

int write_to_master(char write) 
{
  SPDR = write;
  while(!(SPSR & (1 << SPIF)));
  return SPDR;
}
void usart_init(void){
    // usart initialization code
    UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
      UBRR0L = (uint8_t)(BAUD_PRESCALLER);
        UCSR0B = (1<<RXEN0)|(1<<TXEN0);
          UCSR0C = (3<<UCSZ00);
}

unsigned char usart_receive(void){
    while(!(UCSR0A & (1<<RXC0)));
      return UDR0;
}

void usart_send( unsigned char data){
    //wait for register to be ready
    while(!(UCSR0A & (1<<UDRE0)));
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

void generateID(char* data){
  char idRequest[5];
  char idReceive[5];

idRequest[0]=0;
srand(time(NULL));
int temp = rand();
idRequest[4]=(temp>>24) & 0xFF;
idRequest[3]=(temp>>16) & 0xFF;
idRequest[2]=(temp>>8) & 0xFF;
idRequest[1]=temp & 0xFF;

usart_putstring(idRequest);

while(1){
  usart_receiveString(idReceive);
  if(idReceive[1]==idRequest[1]&&idReceive[2]==idRequest[2]
    &&idReceive[3]==idRequest[3]&&idReceive[4]==idRequest[4]){
    data[0]=idReceive[0];
    break;
  }
}

}
