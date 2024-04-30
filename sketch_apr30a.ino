#include "HUSKYLENS.h"
#include "Wire.h"

const int LED_PINS[] = {5, 6, 7};
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

HUSKYLENS huskylens;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    for (int i = 0; i < NUM_LEDS; i++)
        pinMode(LED_PINS[i], OUTPUT);
    
    
    while (!huskylens.begin(Wire)) 
    {
        Serial.println(F("Begin failed"));
        delay(100);
    }
}

void loop() {
    if (data())
        delay(100);

    else
        Serial.println(F("request failed"));
        delay(500); 
    
}

bool data() {
    if (huskylens.request()) 
    {
        while (huskylens.available()) 
        {
            HUSKYLENSResult result = huskylens.read();
            if (result.command == COMMAND_RETURN_BLOCK && result.ID != 0) 
            {
                Serial.println(result.ID);
                controlLEDs(result.ID);    
            } else 
                Serial.println("Object unknown");
            
        }
        return true;
    }
    return false;
}

void controlLEDs(int id) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i == id - 1) 
            digitalWrite(LED_PINS[i], HIGH);
        
        else 
            digitalWrite(LED_PINS[i], LOW);
        
    }
}
