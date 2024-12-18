#include <RTClib.h>
#include <U8g2lib.h>
#include <Servo.h>

#define PIN_SERVO 8
#define PIN_SENSOR 7

// Inicialización del display con la librería U8g2
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0);

// Inicialización del RTC y Servo
RTC_DS3231 rtc;
Servo servo;

// Variables globales optimizadas
const byte hora = 18;
const byte minuto = 21;
const byte segundo = 0;

// Variables de control de tiempo no bloqueante
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  
  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println(F("Error: No se detectó el módulo RTC"));
    while (1);
  }
  
  // Inicializar Display
  display.begin();
  
  // Inicializar Servo y Sensor
  servo.attach(PIN_SERVO);
  pinMode(PIN_SENSOR, INPUT);
  servo.write(0);

  Serial.println(F("Sistema inicializado correctamente"));
}

void loop() {
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  // Actualización no bloqueante cada 500 ms
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    // Limpiar y preparar pantalla
    display.clearBuffer();
    display.setFont(u8g2_font_5x8_tf); // Fuente ligera para ahorrar memoria

    if ((hora == now.hour() && minuto == now.minute() && segundo == now.second()) || digitalRead(PIN_SENSOR) == HIGH) {
      // Mostrar mensaje "Sirviendo Comida"
      display.setCursor(20, 20);
      display.print(F("SIRVIENDO"));
      display.setCursor(30, 40);
      display.print(F("COMIDA"));
      display.sendBuffer();

      // Controlar el servo
      servo.write(90);
      delay(1300); // Control del tiempo del servo
      servo.write(0);
      delay(200);

    } else {
      // Mostrar hora actual
      display.setCursor(20, 10);
      display.print(F("HORA ACTUAL:"));
      display.setCursor(20, 25);
      display.print(now.hour());
      display.print(":");
      display.print(now.minute());
      display.print(":");
      display.print(now.second());

      // Mostrar hora programada
      display.setCursor(20, 45);
      display.print(F("HORA PROG: "));
      display.print(hora);
      display.print(":");
      display.print(minuto);
      display.print(":");
      display.print(segundo);

      display.sendBuffer();
    }
  }
}
