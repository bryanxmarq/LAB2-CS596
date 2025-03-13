#include <Arduino.h>

// led definitions
const int redPin = 27;
const int yellowPin = 26;
const int greenPin = 25;
const int buttonPin = 2;
const int buzzerPin = 33;

enum TrafficLightState { RED, RED_YELLOW, GREEN, YELLOW };
TrafficLightState currentState = RED; //initialize state of trffic light is red

// timing variables
unsigned long transitionStartTime = 0; //stores previous state change time
unsigned long buttonPressTime = 0; //stores when button was pressed
bool buttonPressed = false; //boolean to track when button is pressed

const unsigned long redDuration = 10000; // 10 seconds
const unsigned long yellowDuration = 2000; // 2 seconds
const unsigned long redYellowDuration = 2000; // 2 seconds
const unsigned long minGreenDuration = 5000; // 5 seconds

// buzzer variables
const unsigned long greenBuzzerOn = 500; //green buzzer on for 500ms
const unsigned long greenBuzzerOff = 1500; //green buzzer off for 1500ms
const unsigned long redBuzzerOn = 250; //red buzzer on for 250ms
const unsigned long redBuzzerOff = 250; //red buzzer off for 250ms
unsigned long buzzerStartTime = 0; //stores last buzzer toggle time
bool buzzerOn = false; //buzzer tracks when button is pressed

void setup() {
  //set up led, buzzer, and button  
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT);

  //starts in red
  digitalWrite(redPin, HIGH);
  transitionStartTime = millis();
}

void loop() {
  // checks if button has been pressed
  if (digitalRead(buttonPin) == LOW && !buttonPressed) { 
    buttonPressed = true;
    buttonPressTime = millis();
  }

  //gets current time
  unsigned long currentTime = millis();

  switch (currentState) {
    //stay in red for 10secs, then switch to red-yellow  
    case RED:
      if (currentTime - transitionStartTime >= redDuration) {
        currentState = RED_YELLOW;
        transitionStartTime = currentTime;
        digitalWrite(redPin, LOW);
        digitalWrite(yellowPin, HIGH);
      }
      break;
    //stay in red-yellow for 2secs, then switch to green
    case RED_YELLOW:
      if (currentTime - transitionStartTime >= redYellowDuration) {
        currentState = GREEN;
        transitionStartTime = currentTime;
        digitalWrite(yellowPin, LOW);
        digitalWrite(greenPin, HIGH);
      }
      break;

    case GREEN:
        //stay in green until the button is pressed and 5secs until transitio to yellow
        if (buttonPressed && (currentTime - buttonPressTime >= minGreenDuration)) {
            currentState = YELLOW;
            transitionStartTime = currentTime;
            digitalWrite(greenPin, LOW);
            digitalWrite(yellowPin, HIGH);
            buttonPressed = false; // Reset button flag
        }

      break;
    //stay yello for 2secs, then swithc to red
    case YELLOW:  
      if (currentTime - transitionStartTime >= yellowDuration) {
        currentState = RED;
        transitionStartTime = currentTime;
        digitalWrite(yellowPin, LOW);
        digitalWrite(redPin, HIGH);
      }
      break;
  }

  //buzzer settings
  if (currentState == GREEN) {
    //green should be on for 250ms and off for 1500ms  
    if (currentTime - buzzerStartTime >= (buzzerOn ? greenBuzzerOn : greenBuzzerOff)) {
      buzzerOn = !buzzerOn;
      digitalWrite(buzzerPin, buzzerOn);
      buzzerStartTime = currentTime;
    }
  } else if (currentState == RED) {
    //red should be on and off for 250ms  
    if (currentTime - buzzerStartTime >= (buzzerOn ? redBuzzerOn : redBuzzerOff)) {
      buzzerOn = !buzzerOn;
      digitalWrite(buzzerPin, buzzerOn);
      buzzerStartTime = currentTime;
    }
  } else {
    //buzzer is off during red-yellow and yellow transition  
    digitalWrite(buzzerPin, LOW); // Turn off buzzer during transitions
  }
}
