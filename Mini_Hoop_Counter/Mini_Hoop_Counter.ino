#include <LCD.h>
#include <avr/io.h>
#include <util/delay.h>
#define checkbit(x, y) ((x) & (y))
#define bitn(p) (0x01 << (p))

void trigger();

int main(void)
{
  LCD lcd;
  lcd.init();
  int count;
  double distance;
  DDRB = 0x01;
  int pinstate;
  int objectCount = 0;

  while (1)
  {
    trigger();

    while (checkbit(PINB, bitn(1)))
    {
      TCCR1A = 0x00;
      TCCR1B = 0x03;
      pinstate = 1;
    }

    while (!checkbit(PINB, bitn(1)) && pinstate == 1)
    {
      TCCR1B = 0x00;
      count = TCNT1;
      distance = (count * 0.008575);

      lcd.cmd(0x01);
      lcd.cmd(0x0C);

      if (distance < 4.0) //ensures the counting only occurs when ball goes through the hoop 
      {
        // Increment the object count
        objectCount++;

        // Display the object count on LCD
        lcd.showvalue(objectCount);

        _delay_ms(750);
      }

      pinstate = 0;
      TCNT1 = 0;
    }
  }
}

void trigger()
{
  PORTB = 0x00;
  _delay_us(10);
  PORTB = 0x01;
  _delay_us(10);
  PORTB = 0x00;
}
