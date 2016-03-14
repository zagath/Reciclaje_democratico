/**
 * @file cliente.ino
 * @brief Cliente Proyecto Recolector de Botellas Democratico
 * @date 24/10/2015
 * @author Leandro Perez Guatibonza / leandropg AT gmail DOT com
 * @author Willmar Romero / willmar DOT romero AT gmail DOT com
 * @version 1.0
 *
 * Fundacion Materializacion 3D Madrid
 * Copyright (C) 2015 - 2016
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <NRF905.h>
#include <SPI.h>

/**
 * Read Configuration Buffer
 */
unsigned char read_config_buf [10];

/**
 * Write Configuration Buffer
 * Frequency 433MHz / Receiving Address 0xCC,0xCC,0xCC,0xCC
 */
unsigned char write_config_buf [10] = {0x6A, 0x0C, 0x44, 0x20, 0x20, 0xCC, 0xCC, 0xCC, 0xCC, 0x58};

/**
 * Transmission Buffer
 */
unsigned char buffer_tx [32] = {0};

/**
 * Pin Led and Buzzer
 */
int LED_BUZZER = P2_6;

/**
 * PushButton Option A
 */
int PUSH_BUTTON_A = P1_0;

/**
 * PushButton Option B
 */
int PUSH_BUTTON_B = P1_1;

/**
 * PushButton Option C
 */
int PUSH_BUTTON_C = P1_2;

/**
 * Bottle Sensor
 */
int BOTTLE_SENSOR = P1_3;

/**
 * Microcontroller Setup
 */
void setup()
{
  // Configure Led and Buzzer
  pinMode(LED_BUZZER, OUTPUT);
  digitalWrite(LED_BUZZER, LOW);
  
  // Configure Push Button
  pinMode(PUSH_BUTTON_A, INPUT);
  pinMode(PUSH_BUTTON_B, INPUT);
  pinMode(PUSH_BUTTON_C, INPUT);
  
  // Configure Bottle Sensor
  pinMode(BOTTLE_SENSOR, INPUT);
  
  // Init NRF905 Module
  nrf905 = NRF905();
  nrf905.init();
  nrf905.write_config(write_config_buf);
  nrf905.read_config(read_config_buf);
  
  // Loop Led and Buzzer four times
  for(int i = 0; i < 4; i ++) {

    digitalWrite(LED_BUZZER, HIGH);
    delay(50);
    
    digitalWrite(LED_BUZZER, LOW);
    delay(50);
  }
}

/**
 * Main Program
 */
void loop()
{    
    unsigned char optionSelected = 'N';
  
    // Validate Bottle Sensor
    if(digitalRead(BOTTLE_SENSOR) == HIGH) {

        // Indicate Wait
        for(int i = 0; i < 4; i++) {

          digitalWrite(LED_BUZZER, HIGH);
          delay(100);
          digitalWrite(LED_BUZZER, LOW);
          delay(100);
        }

        // Turn On Led and Buzzer        
        digitalWrite(LED_BUZZER, HIGH);
        
        // Wait Selection Option
        do {
            optionSelected = verifySensors();
        }
        while(optionSelected == 'N');
        
        // Configure Information to Send
        buffer_tx[0] = optionSelected;
        buffer_tx[1] = '\r';
        buffer_tx[2] = '\n';
        
        // Transmit Option Selected
        txOption();
    }
}

/**
 * Verify Sensor
 */
int verifySensors() {
  
    unsigned char optionSelected = 'N';

    // Validate Push Button A
    if(digitalRead(PUSH_BUTTON_A) == LOW) {

        // Delay 50 ms      
        delay(50);
        
        // Validate Push Button A
        if(digitalRead(PUSH_BUTTON_A) == LOW) {
    
            // Wait release Push Button A
            while(digitalRead(PUSH_BUTTON_A) == LOW);

            // Select Led Option A
            pinMode(PUSH_BUTTON_A, OUTPUT);

            // Indicate Option A Selected
            for(int i = 0; i < 4; i++) {

              digitalWrite(PUSH_BUTTON_A, LOW);
              digitalWrite(LED_BUZZER, HIGH);
              delay(100);
              digitalWrite(LED_BUZZER, LOW);
              digitalWrite(PUSH_BUTTON_A, HIGH);
              delay(100);
            }            
            
            // Select Push Button A
            pinMode(PUSH_BUTTON_A, INPUT);
            
            // Indicate Option A Selected
            optionSelected = 'A';
        }
    }
    
    // Validate Push Button B
    if(digitalRead(PUSH_BUTTON_B) == LOW) {

        // Delay 50 ms      
        delay(50);
        
        // Validate Push Button B
        if(digitalRead(PUSH_BUTTON_B) == LOW) {
    
            // Wait release Push Button B
            while(digitalRead(PUSH_BUTTON_B) == LOW);

            // Select Led Option B
            pinMode(PUSH_BUTTON_B, OUTPUT);

            // Indicate Option B Selected
            for(int i = 0; i < 4; i++) {

              digitalWrite(PUSH_BUTTON_B, LOW);
              digitalWrite(LED_BUZZER, HIGH);
              delay(100);
              digitalWrite(LED_BUZZER, LOW);
              digitalWrite(PUSH_BUTTON_B, HIGH);
              delay(100);
            }            
            
            // Select Push Button B
            pinMode(PUSH_BUTTON_B, INPUT);

            // Indicate Option B Selected
            optionSelected = 'B';
        }
    }
    
    // Validate Push Button C
    if(digitalRead(PUSH_BUTTON_C) == LOW) {

        // Delay 50 ms      
        delay(50);
        
        // Validate Push Button C
        if(digitalRead(PUSH_BUTTON_C) == LOW) {
    
            // Wait release Push Button C
            while(digitalRead(PUSH_BUTTON_C) == LOW);

            // Select Led Option C
            pinMode(PUSH_BUTTON_C, OUTPUT);

            // Indicate Option C Selected
            for(int i = 0; i < 4; i++) {

              digitalWrite(PUSH_BUTTON_C, LOW);
              digitalWrite(LED_BUZZER, HIGH);
              delay(100);
              digitalWrite(LED_BUZZER, LOW);
              digitalWrite(PUSH_BUTTON_C, HIGH);
              delay(100);
            }            
            
            // Select Push Button C
            pinMode(PUSH_BUTTON_C, INPUT);

            // Indicate Option C Selected
            optionSelected = 'C';
        }
    }
    return optionSelected;
}

/**
 * Transmit Option Selected
 */
void txOption() {
 
    // Turn On Led and Buzzer
    digitalWrite(LED_BUZZER, HIGH);
    
    // Send Info to Server Node
    nrf905.TX(buffer_tx);
    
    // Delay 100 ms
    delay(100);
    
    // Turn Off Led and Buzzer
    digitalWrite(LED_BUZZER, LOW);
}
