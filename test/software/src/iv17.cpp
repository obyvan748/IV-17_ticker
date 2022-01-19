#include "Arduino.h"
#include "iv17_font.h"

#define DEBUG                1           // debug on/off
#define DEBUG_LDR            0           // light sensor infos
#define SECOND               1000000     // 1000 * 1000 us

#define BRIGHTNESS_INITLEVEL 35
#define BRIGHTNESS_MINLEVEL  20    // 6V
#define BRIGHTNESS_MAXLEVEL  70    // 25V
#define BRIGHTNESS_STANDARD  50    // 15V
#define LDR_UPDATE_INTERVAL  1000
#define LDR_SAMPLES          10

const int LDR_PIN  = A0;
const int LATCHPIN = 13;
const int CLOCKPIN = 12;
const int DATAPIN  = 14;
const int BLANKPIN = 26;
const int PWMPIN   = 27; 


byte SERDEBUG = DEBUG;  // debug infos to serial

unsigned long timeStart = millis();
unsigned long lastTime = 0;

// setting PWM properties
const int frequency  = 35000;
const int ledChannel = 0;
const int resolution = 8;

// tubes grid connection
unsigned long grid = GRID;

// tubes brightness (via pwm)
unsigned char brightness        = BRIGHTNESS_INITLEVEL;
unsigned long refresh_cnt       = 0;

unsigned long time_stamps[]     = {0,0};
unsigned long second            = SECOND;
unsigned char digit             = 0;
unsigned char mode              = 0;

// LDR or phototransistor
unsigned int ldrRawData         = 0;
float sumldr                    = 0;
float avgldr                    = 0;
float save_avgldr               = 0;
long  ldrCnt                    = 0;
int   ldrSampleCnt              = 0;

volatile bool INTR_bit          = 0;

// prototypes
void shiftOutChar(int c);
void shiftOutRaw(unsigned long bits);
void serialOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val);
void setBrightness(void);
void whirl(char delaycnt, char mode); 
void test_pwm();
void seven(void);


// WiFi stuff
extern void WiFi_init();
extern void NTP_printLocalTime(int mode);


//
// initialize
//
void setup()
{
  // init PWM port
  pinMode(PWMPIN, OUTPUT);     // very first! To prevent high or float state on PWM (FET Gate)
  digitalWrite(PWMPIN, LOW);

  // shift-register port init
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  pinMode(BLANKPIN, OUTPUT);

  // re-init PWM 
  ledcSetup(ledChannel, frequency, resolution);
  ledcAttachPin(PWMPIN, ledChannel);

  // set initial HV Anode tube voltage (brightness)
  ledcWrite(ledChannel, BRIGHTNESS_INITLEVEL);

  Serial.begin(115200);
  delay(200);

  Serial.println("IV-17 initializing...");
  serialOut(DATAPIN, CLOCKPIN, MSBFIRST, segI | grid);

  // init WiFi
  WiFi_init();
  NTP_printLocalTime(1);
  delay(200);
  NTP_printLocalTime(0);

  // init timestamps
  time_stamps[0] = micros();
  second = SECOND;
}

//
// main()
//
void loop()
{
  if((micros() > time_stamps[0] + ulong(second))) {
    time_stamps[0] = micros();
    INTR_bit = 1;
    // Serial.print("0 time_stamps0: ");
    // Serial.print(time_stamps[0]);
    // Serial.print(" micros: ");
    // Serial.print(micros());
    // Serial.print("\n");
  }

  if(INTR_bit) {
    // Serial.print("INTR_bit - time_stamps0: ");
    // Serial.print(time_stamps[0]);
    // Serial.print(" micros: ");
    // Serial.print(micros());
    // Serial.print(" mode: ");
    // Serial.print(mode);
    // Serial.print("\n");

    INTR_bit=0;
    if(mode==0) {
      serialOut(DATAPIN, CLOCKPIN, MSBFIRST, segments_16_numerals[digit++] | grid);
      if(digit > 20) {
        digit=0;
        mode++;
      }
    }
    if(mode==1) {
      serialOut(DATAPIN, CLOCKPIN, MSBFIRST, segments_16_alpha_upper[digit++] | grid);
      if(digit > 26) {
        digit=0;
        mode=0;
      }
    }
    // if(mode==2) {
    //   second = SECOND/1000ul;
    //   for(char i=0; i!=20;i++)
    //     whirl(60-i*3, 1);
    //   for(char i=0; i!=20;i++)
    //     whirl(5, 1);
    //   second = SECOND;
    //   mode=0;
    // }
  }

  // update tubes brightness (PWM duty)
  setBrightness();  
}


//
// test PWM (be aware of high voltages, first measure VOUT without powered tube and VFD shift-register)
//
void test_pwm()
{
  char MINPWM = 10;
  char MAXPWM = 70;

  // increase brightness
  for(int dutyCycle = MINPWM; dutyCycle <= MAXPWM; dutyCycle++) {
    Serial.print("dutyCycle: ");
    Serial.println(dutyCycle);
    ledcWrite(ledChannel, dutyCycle);
    delay(500);
  }

  // decrease brightness
  for(int dutyCycle = MAXPWM; dutyCycle >= MINPWM; dutyCycle--) {
    Serial.print("dutyCycle: ");
    Serial.println(dutyCycle);
    ledcWrite(ledChannel, dutyCycle);   
    delay(500);
  }
}

//
// spin baby spin
//
void whirl(char delaycnt, char mode) 
{
  if(mode) {
    shiftOutRaw(segJ);
    delay(delaycnt);
    shiftOutRaw(segH);
    delay(delaycnt);
    shiftOutRaw(segK);
    delay(delaycnt);
    shiftOutRaw(segG2);
    delay(delaycnt);
    shiftOutRaw(segL);
    delay(delaycnt);
    shiftOutRaw(segI);
    delay(delaycnt);
    shiftOutRaw(segM);
    delay(delaycnt);
    shiftOutRaw(segG1);
    delay(delaycnt);
  } else {
    shiftOutRaw(segG1);
    delay(delaycnt);
    shiftOutRaw(segM);
    delay(delaycnt);
    shiftOutRaw(segI);
    delay(delaycnt);
    shiftOutRaw(segL);
    delay(delaycnt);
    shiftOutRaw(segG2);
    delay(delaycnt);
    shiftOutRaw(segK);
    delay(delaycnt);
    shiftOutRaw(segH);
    delay(delaycnt);
    shiftOutRaw(segJ);
    delay(delaycnt);
  }
}

void shiftOutRaw(unsigned long bits)
{
    serialOut(DATAPIN, CLOCKPIN, MSBFIRST, bits | grid);
}


void shiftOutChar(int c) 
{
    serialOut(DATAPIN, CLOCKPIN, MSBFIRST, segments_16[c] | grid);
}


void serialOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val)
{
  digitalWrite(LATCHPIN, LOW);
  for (char i=0; i<20; i++)  {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1ul << i)));   // 1ul tells it to use an unisgned long. If we use just 1, its only 16 bits
    else    
      digitalWrite(dataPin, !!(val & (1ul << (19 - i))));

    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);        
  }
  digitalWrite(LATCHPIN, HIGH);
}


//
// set brightness upon light sensors value
//
void setBrightness(void)
{
  // tube brightness upon LDR or phototransistor measure
  if(ldrCnt++ >= LDR_UPDATE_INTERVAL) {
    analogReadResolution(10);
    ldrRawData = analogRead(LDR_PIN);
    // Serial.print("analogRead LDR: "); 
    // Serial.println(ldrRawData);
    // Serial.println("");
    sumldr += ldrRawData;
    ldrCnt = 0;
    ldrSampleCnt++;
  }
  if(ldrSampleCnt >= LDR_SAMPLES) {
    avgldr = sumldr/LDR_SAMPLES;
    sumldr = 0;
    ldrSampleCnt = 0;
  }

  if(avgldr < save_avgldr-5 || avgldr > save_avgldr+5) {
    brightness = map(avgldr, 0, 1024, BRIGHTNESS_INITLEVEL-20, BRIGHTNESS_MAXLEVEL);

    if(brightness >= BRIGHTNESS_MAXLEVEL)
      brightness = BRIGHTNESS_MAXLEVEL;

    ledcWrite(ledChannel, brightness);
    save_avgldr = avgldr;

    if(SERDEBUG && DEBUG_LDR) {
      Serial.print("Average Illuminance: "); 
      Serial.print(avgldr);
      Serial.println(""); 
      Serial.print("Brightness: "); 
      Serial.print(brightness); 
      Serial.print("\n\n"); 
    }
  } 
}