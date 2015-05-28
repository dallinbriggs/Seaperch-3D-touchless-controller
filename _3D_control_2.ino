//
// By Kyle McDonald
// Modified by Dallin Briggs
// From the instructables project at:
// http://www.instructables.com/id/DIY-3D-Controller/

#define resolution 8
#define mains 60 // 60: north america, japan; 50: most other places
#include <AFMotor.h>

AF_DCMotor motor4(4);
AF_DCMotor motor3(3);
AF_DCMotor motor2(2);

#define refresh 2 * 1000000 / mains

void setup() 
{
  Serial.begin(9600);

  for(int i = 9; i < 12; i++)
    pinMode(i, INPUT);

  startTimer();
  
  motor4.setSpeed(200);
  
}

int i;
double median, upper, lower;
long starttime, t, minimum = 100000, maximum;

void loop() 
{  
  starttime = millis();
  // Prints time since program started.
  Serial.print(starttime);
  Serial.print(" ");
//  Serial.print(time(9, B00000010), DEC);
//  Serial.print(" ");
  Serial.print(time(10, B00000100), DEC);
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(minimum);
  Serial.print(" ");
  Serial.print(lower);
  Serial.print(" ");
  Serial.print(median);
  Serial.print(" ");
  Serial.print(upper);
  Serial.print(" ");
  Serial.println(maximum);
//  Serial.print(" ");
//  Serial.println(time(11, B00001000), DEC);
  
  t = time(10, B00000100);
  
  if(t > maximum)
    maximum = t;
  
  if(minimum > t)
    minimum = t;
  
  median = (maximum + minimum)/2;
  upper = median + .25*(maximum - minimum);
  lower = median - .25*(maximum - minimum);
  
  if (t > upper)
    motor4.run(FORWARD);
  else
    motor4.run(RELEASE);
    
  if (t < lower)
    motor4.run(BACKWARD);
  
}
  
long time(int pin, byte mask) 
{
  unsigned long count = 0, total = 0;
  while(checkTimer() < refresh) 
  {
    // pinMode is about 6 times slower than assigning
    // DDRB directly, but that pause is important
    pinMode(pin, OUTPUT);
    PORTB = 0;
    pinMode(pin, INPUT);
    while((PINB & mask) == 0)
      count++;
    total++;
  }
  startTimer();
  return (count << resolution) / total;
}

extern volatile unsigned long timer0_overflow_count;

void startTimer() {
  timer0_overflow_count = 0;
  TCNT0 = 0;
}

unsigned long checkTimer() {
  return ((timer0_overflow_count << 8) + TCNT0) << 2;
}


