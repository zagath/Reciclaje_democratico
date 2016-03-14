/**
 * @file servidor.ino
 * @brief Proyecto Punto Reciclaje Democratico - Nodo Servidor
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
 * Reception Buffer
 */
unsigned char buffer_rx [32] = {0};

/**
 * Pin Buzzer
 */
int LED_BUZZER = P2_6;

/**
 * WiFi SSID Network
 */
#define SSID "UnCuartoTech"

/**
 * WiFi Password Network
 */
#define PASSWORD "1/4T3ch2015"

/**
 * Ubidots URL
 */
#define IP "translate.ubidots.com"

/**
 * Ubidots Port
 */
#define PORT 80

/**
 * Microcontroller Setup
 */
void setup()
{  
  // Configure Led and Buzzer
  pinMode(LED_BUZZER, OUTPUT);
  digitalWrite(LED_BUZZER, LOW);
  
  // Init NRF905 Module
  nrf905 = NRF905();
  nrf905.init();
  nrf905.write_config(write_config_buf);
  nrf905.read_config(read_config_buf);
  
  // Setup ESP8266 WiFi Module
  setupWiFi();

  // Loop Buzzer/Led four times
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
    int option = 0;
  
    // Wait Reception NRF905 Module
    nrf905.RX(buffer_rx);

    // Validate Option Selected
    if ((buffer_rx[0] == 'A') || (buffer_rx[0] == 'B') || (buffer_rx[0] == 'C')) {

      if(buffer_rx[0] == 'A') {

          option = 1;
      }
      else if(buffer_rx[0] == 'B') {

          option = 2;
      }
      else if(buffer_rx[0] == 'C') {

          option = 3;
      }
      
      // Turn On Led / Buzzer
      digitalWrite(LED_BUZZER, HIGH);
      
      // Envia primer dato
      sendWiFi(option);

      // Turn Off Led / Buzzer
      digitalWrite(LED_BUZZER, LOW);
      
      // Clear RX Buffer
      buffer_rx[0] = ' ';
      buffer_rx[1] = ' ';
      buffer_rx[2] = ' ';
    }
}

/**
 * Setup WiFi Network
 */
void setupWiFi(void) {

  String command;
  
  // Init UART for ESP8266 WiFi Module
  Serial.begin(115200);
  
  // Send AT Command Ready
  Serial.println("AT");
  delay(500);
  
  // Validate AT Command Ready
  if(Serial.find("OK")){

    // Set WiFi Mode 1
    Serial.println("AT+CWMODE=1");
    delay(500);
    
    // Connect to WiFi Network
    command = "AT+CWJAP=\"";
    command += SSID;
    command += "\",\"";
    command += PASSWORD;
    command += "\"";
    Serial.println(command);
    delay(8000);
  }
}

/**
 * Send WiFi Data
 */
void sendWiFi(int option) {

  String command;
  
  // Connect to Ubidots Server
  command = "AT+CIPSTART=\"TCP\",\"";
  command += IP;
  command += "\",";
  command += PORT;
  Serial.println(command);
  delay(1000);

  // Send Quantity Bytes to Send
  Serial.println("AT+CIPSEND=131");

  // Wait Ready To Send > ESP8266 Module
  if(Serial.find(">")){

    Serial.print("GET /api/postvalue/?");
    Serial.print("token=wGtaUR1BxWJCPUJLHygPYorWlLl");
    Serial.print("7bAaNuaW6JcVbA4HhCZkpc4RBO7HeZ042");
    Serial.print("&variable=561ea2f9762542449e5eb9ed");
    Serial.print("&value=");
    Serial.print(option);
    Serial.print("\r\n");
    Serial.println("AT+CIPCLOSE");
  }    
}
