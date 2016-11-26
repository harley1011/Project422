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
void generateID(char* data);
void usart_receiveString(char* data);

int main(void){
 char recieved_byte;
 char data[6];
  
usart_init();
/*test usart
while(1){
  usart_receive();
  usart_putstring("abcde");
}
end test*/
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
