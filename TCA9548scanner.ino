/**
 * TCA9548 and two ambient light sensors
 * by: Peter Mankowski
 */

#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_VEML7700.h"

#define TCAADDR 0x70

/* Assign a unique ID to this sensor at the same time */
Adafruit_VEML7700 vem0 = Adafruit_VEML7700();
Adafruit_VEML7700 vem1 = Adafruit_VEML7700();  

// Scant all eight chanels to see what is connected
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

/* Standard set-up fpr sevices */
void setup(void)
{ 
    uint8_t node_cnt = 0; //Couner of individual end nodes. Jsto to know how many are up there.
    uint8_t delay_local = 500; // How ofter to read values in variable
    uint8_t tcaselect_0 = 0; //Which sensor to pick
    uint8_t tcaselect_1 = 1; //Which sensor to pick
    
    while (!Serial);
    delay(delay_local);

    Wire.begin();
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready to probe 8-ch mux");
    
    for (uint8_t t=0; t<8; t++) {
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;

        Wire.beginTransmission(addr);
        if (!Wire.endTransmission()) {
          node_cnt = node_cnt + 1;
          Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
        }
      }
    }
    Serial.print("\nFound "); Serial.print(node_cnt); Serial.println(" nodes");
    Serial.println("VEML7700 configuration: begins"); 

    /* Initialise the 1st sensor */
    tcaselect(tcaselect_0);
    if(!vem0.begin())
    {
      /* There was a problem detecting the 1st ALS ... check your connections */
      Serial.println("Ch.0, no device detected ... Check your wiring!");
      while(1);
    }
  
    /* Initialise the 2nd sensor */
    tcaselect(tcaselect_1);
    if(!vem1.begin())
    {
      /* There was a problem detecting the 12nd ALS ... check your connections */
      Serial.println("Ch.1, no device detected ... Check your wiring!");
      while(1);
    }

    /* Pass sensor selection */
    else
    {
      /* PASS message confirmation */
      Serial.println("All ALS sensors intialized - PASS");
      delay(5000);         
    }
}

void loop()
{
    uint8_t delay_local = 500; // How ofter to read values in variable
    
    /* Get a new sensor event */ 
    float result_0 = 0; /* lux value for ALS.0 */
    float result_1 = 0; /* lux value for ALS.1 */
     
    /* Scan for all results. Need to pick each individually to read the proper chanel*/
    /* Ch.0 READ  */
    tcaselect(0);
    result_0 = vem0.readLux();

    /* Ch.1 READ  */
    tcaselect(1);
    result_1 = vem1.readLux();
    delay(10); /* Settle time */
    
    /* Display results to confirm each ALS works independently */
    Serial.print("Mux.0 & 1: "); Serial.print(result_0); Serial.print("  "); Serial.println(result_1);

    delay(delay_local); /* Control how often cmd line spitts values out */
}

/*
  
#include <Wire.h>
#include "Adafruit_VEML7700.h"

Adafruit_VEML7700 veml = Adafruit_VEML7700();

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit VEML7700 Test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  veml.setGain(VEML7700_GAIN_1_4);
  veml.setIntegrationTime(VEML7700_IT_800MS);

  delay(1000);

  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }

  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  //veml.powerSaveEnable(true);
  //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);

  veml.setLowThreshold(20);
  veml.setHighThreshold(270);
  veml.interruptEnable(true);
}

void loop()
{
  Serial.print("Lux: "); Serial.println(veml.readLux());
  //Serial.print("White: "); Serial.println(veml.readWhite());
  //Serial.print("Raw ALS: "); Serial.println(veml.readALS());

  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold"); 
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold"); 
  }
  delay(1000);
}
*/
