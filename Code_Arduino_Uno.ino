#include <dht.h>
#include <SoftwareSerial.h>

dht DHT;
#define temperaturePin 12
#define ldrPin A1
#define fanPin 5
#define choPin 6
#define LED_AUTO 7
#define buzzerPin 11
#define led1Pin 2
#define led2Pin 3
#define led3Pin 4
#define bluetoothTxPin 8
#define bluetoothRxPin 9

SoftwareSerial mySerial(bluetoothRxPin, bluetoothTxPin);

float set_value = 0;
float tempF = 0;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(LED_AUTO, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(choPin, OUTPUT);

  pinMode(ldrPin, INPUT);
  pinMode(temperaturePin, INPUT);
}

void loop() {
  int chk = DHT.read11(temperaturePin);
  tempF = (DHT.temperature * 1.8) + 32;

  if ((int)DHT.temperature != -999.00) {
    if ((int)DHT.temperature != (int)set_value) {
      mySerial.print("S;");
      mySerial.print(DHT.temperature, 0);
      mySerial.print(";");
      mySerial.print(tempF, 0);
      mySerial.println(";");
      set_value = DHT.temperature;
    }
  }

  int ldrValue = analogRead(ldrPin);
  digitalWrite(LED_AUTO, ldrValue > 500 ? LOW : HIGH);

  int gasSensorValue = analogRead(A0);
  if (gasSensorValue >= 155) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      delay(500);
    }
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  if (mySerial.available()) {
    String data = mySerial.readString();
    handleBluetoothCommands(data);
  }
}

void handleBluetoothCommands(String data) {
  if (data == "ON ROOM1") {
    digitalWrite(led1Pin, HIGH);
  } else if (data == "OFF ROOM1") {
    digitalWrite(led1Pin, LOW);
  }

  if (data == "ON ROOM2") {
    digitalWrite(led2Pin, HIGH);
  } else if (data == "OFF ROOM2") {
    digitalWrite(led2Pin, LOW);
  }

  if (data == "ON ROOM3") {
    digitalWrite(led3Pin, HIGH);
  } else if (data == "OFF ROOM3") {
    digitalWrite(led3Pin, LOW);
  }

  if (data == "ON ALL ROOMS") {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, HIGH);
  } else if (data == "OFF ALL ROOMS") {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
  }

  if (data == "ON FAN") {
    digitalWrite(fanPin, HIGH);
  } else if (data == "OFF FAN") {
    digitalWrite(fanPin, LOW);
  }

  if (data == "ON CHO") {
    digitalWrite(choPin, HIGH);
  } else if (data == "OFF CHO") {
    digitalWrite(choPin, LOW);
  }
}
