#include <Arduino.h>

#define GREEN_LED 25 //define green light and pin they use
#define YELLOW_LED 26 //define yellow light and pin they use 
#define RED_LED 27 //define red light and pin they use
#define BUZZER 33 //define buzzer and pin they use
#define TOUCH_BUTTON 2 //define button and pin they use

unsigned long previousMillis = 0; //previous time in order to tell how long a light was on
bool pedestrianRequest = false; //boolean to tell when the button is pressed (false hasn't been pressed)

enum TLState {GREEN, YELLOW, RED, RED_YELLOW}; //default order
TLState state = GREEN; //default state when turned on

void setup() {
    //pin mode for led, buzzer, & button
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(TOUCH_BUTTON, INPUT);
}

void loop() {
    static unsigned long stateStartTime = millis(); //keep time with millis() function
    static bool buzzerState = false; //boolean to tell when the buzzer should be on
    unsigned long currentMillis = millis();
    
    //check if pedestrian button is pressed
    if (digitalRead(TOUCH_BUTTON) == HIGH) {
        pedestrianRequest = true;
    }
    
    //order of the traffic light
    switch (state) {
        case GREEN:
            //turns green and turns off the rest
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(YELLOW_LED, LOW);
            digitalWrite(RED_LED, LOW);
            
            //buzzer should sound every 500ms
            if ((currentMillis - previousMillis) >= 500) {
                previousMillis = currentMillis;
                buzzerState = !buzzerState;
                digitalWrite(BUZZER, buzzerState);
            }
            
            //change state to yellow is button was pressed and green light has been on for 5secs
            if (pedestrianRequest && (currentMillis - stateStartTime >= 5000)) {
                state = YELLOW;
                stateStartTime = currentMillis;
            }
            break;
        
        case YELLOW:
            //turn on yellow and keep others off
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(YELLOW_LED, HIGH);
            
            //after 2secs,  swithc to red
            if (currentMillis - stateStartTime >= 2000) {
                state = RED;
                stateStartTime = currentMillis;
                pedestrianRequest = false;
            }
            break;
        
        case RED:
            //turn on red and keep others off
            digitalWrite(YELLOW_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            
            //buzeer should sound every 250ms
            if ((currentMillis - previousMillis) >= 250) {
                previousMillis = currentMillis;
                buzzerState = !buzzerState;
                digitalWrite(BUZZER, buzzerState);
            }
            
            //stay red for 10 secs and swithc to yellow
            if (currentMillis - stateStartTime >= 10000) {
                state = RED_YELLOW;
                stateStartTime = currentMillis;
            }
            break;
        
        case RED_YELLOW:
            //turn yellow and red light
            digitalWrite(YELLOW_LED, HIGH);
            
            //after 2secs switch to green
            if (currentMillis - stateStartTime >= 2000) {
                state = GREEN;
                stateStartTime = currentMillis;
            }
            break;
    }
} 
