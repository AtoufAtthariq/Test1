#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHT_TYPE DHT22
#define DHT_PIN A0
#define SOIL_MOISTURE_PIN A1
#define FAN_PIN 9
#define FOGGER_PIN 10  // Menggunakan fogger untuk meningkatkan kelembaban
#define HEATER_PIN 11  // Menggunakan heater untuk menjaga suhu kumbung
#define HUMIDITY_THRESHOLD 90  // Ambang batas kelembaban untuk mengaktifkan fogger
#define TEMPERATURE_THRESHOLD 20  // Ambang batas suhu untuk mengaktifkan heater

DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(FOGGER_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Sensor Reading");
  dht.begin();
}

void loop() {
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int soilMoisturePercentage = map(soilMoistureValue, 0, 1023, 0, 100);

  Serial.print("Kelembaban Tanah: ");
  Serial.println(soilMoisturePercentage);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca suhu atau kelembaban dari sensor DHT!");
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setBacklight(1);

  // Menghidupkan kipas jika suhu melebihi 25 derajat Celsius
  if (temperature > 25) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }

  // Menghidupkan fogger jika kelembaban di bawah ambang batas
  if (humidity < HUMIDITY_THRESHOLD) {
    digitalWrite(FOGGER_PIN, HIGH);
  } else {
    digitalWrite(FOGGER_PIN, LOW);
  }

  // Menghidupkan heater jika suhu di bawah ambang batas
  if (temperature < TEMPERATURE_THRESHOLD) {
    digitalWrite(HEATER_PIN, HIGH);
  } else {
    digitalWrite(HEATER_PIN, LOW);
  }

  delay(2000);
}
