#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3);

String input = "";

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  Serial.println("UNO READY");
  
}


void loop() {
  while (espSerial.available()) {
    char c = espSerial.read();
    
    if (c == '\n') {

      input.trim();

      Serial.print("COMMAND: ");
      Serial.println(input);

      handleCommand(input);

      input = "";

    } else {

      input += c;

    }
  }
}

void handleCommand(String command) {

  if (command.startsWith("WATER:")) {

    int firstColon = command.indexOf(':');
    int secondColon = command.indexOf(':', firstColon + 1);

    int plantId =
      command.substring(firstColon + 1, secondColon).toInt();

    int amount =
      command.substring(secondColon + 1).toInt();

    Serial.print("Plant ID: ");
    Serial.println(plantId);

    Serial.print("Amount: ");
    Serial.println(amount);

    // Later:
    // activate pump
  }
}

