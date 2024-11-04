/**
 * @file main.cpp
 * @brief Demo of speech recognition using Maix board - K210 MCU
 * @copyright 2019 - Andri Yadi, DycodeX
 */

 //Use this as templete to train my model for notifying me to understand if there are birds or cats in my balcony

#include <Arduino.h>
#include "Maix_Speech_Recognition.h"

#include "voice_model.h"


#define RECORD_MODE         0 // Set this 1 to start collecting your Voice Model

SpeechRecognizer rec;

#if RECORD_MODE
void recordModel() {
  Serial.println("Start recording...");
  if( rec.record(0, 0) == 0) //keyword_num, model_num 
  {    
    rec.print_model(0, 0);
  }
  else 
      Serial.println("Recording failed! Reset the board.");
}
#endif

void setup()
{ 
  rec.begin();
  delay(1000);
  Serial.begin(115200);
  
#if RECORD_MODE
  recordModel();
  while(1);
#endif

#define LED_RED 10
#define LED_BLUE 11

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  Serial.println("Initializing model...");

  rec.addVoiceModel(0, 0, cat_0, fram_num_cat_0); 
  rec.addVoiceModel(0, 1, cat_1, fram_num_cat_1); 
  rec.addVoiceModel(0, 2, cat_2, fram_num_cat_2); 
  rec.addVoiceModel(0, 3, cat_3, fram_num_cat_3); 
  rec.addVoiceModel(1, 0, bird_0, fram_num_bird_0);     
  rec.addVoiceModel(1, 1, bird_1, fram_num_bird_1);     
  rec.addVoiceModel(1, 2, bird_2, fram_num_bird_2);     
  rec.addVoiceModel(1, 3, bird_3, fram_num_bird_3); 

  Serial.println("Model init OK!");
  Serial.println("Everything is ready!!");
  delay(1000);
}

void loop()
{
  int res;

  Serial.println("Waiting for Wake Word..");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  res = rec.recognize();
  Serial.printf("Result: %d --> ", res);

  if (res > 0){
    switch (res)
    {
    case 1:
        digitalWrite(LED_RED, LOW); //power on red led
        digitalWrite(LED_BLUE, HIGH);
        Serial.println("means red LED");
        Serial.println("Cat Detected");
        break;
    case 2:
        digitalWrite(LED_BLUE, LOW); //power on blue led
        digitalWrite(LED_RED, HIGH);
        Serial.println("means blue LED");
        Serial.println("Bird Detected");
        break;
    case 3:
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_RED, LOW);
        Serial.println("-> failed!");
        Serial.println("Sorry Tony, I don't know");
    default:
        break;
    }
  }else
  {
      Serial.println("Recognizing failed.");
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, LOW);
      Serial.println("-> failed!");
      //printCenterOnLCD(lcd, "Sorry Tony, I don't know");
      Serial.println("No Animals Detected");
  }
  delay(10000);
}