// Define PINS
#define photo_Pin A2
#define alert_Pin 8   // LED pin

// Threshold (converted to % since we are using mapping)
const int light_Threshold_Percent = 21;  // 220 raw ≈ 21%

bool shouldBlink = false;   // stays true even after brightness drops
bool stopBlink = false;     // becomes true when user types "stop"

void setup() {
  pinMode(alert_Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read sensor raw value
  int brightnessRaw = analogRead(photo_Pin);

  // MAPPING: convert 0–1023 to 0–100% 
  int brightnessPercent = map(brightnessRaw, 0, 1023, 0, 100);

  Serial.print("Brightness: ");
  Serial.print(brightnessRaw);
  Serial.print(" (");
  Serial.print(brightnessPercent);
  Serial.println("%)");

  // If brightness is above threshold (now in percent), start blinking mode
  if (brightnessPercent >= light_Threshold_Percent) {
    shouldBlink = true; 
  }

  // Check Serial input for STOP command
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.toLowerCase();  // make it case insensitive
    if (input == "stop") {
      shouldBlink = false;
      digitalWrite(alert_Pin, LOW); // turn LED off immediately
    }
  }

  // Blinking behavior
  if (shouldBlink) {
    digitalWrite(alert_Pin, HIGH);
    delay(100);
    digitalWrite(alert_Pin, LOW);
    delay(100);
  } else {
    digitalWrite(alert_Pin, LOW);
    delay(200);
  }
}
