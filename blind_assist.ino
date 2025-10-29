/*
 * Blind Assist Arduino
 * Smart Obstacle Detector for Blind and Visually Impaired
 * Uses Ultrasonic Sensor for obstacle detection with vibration and buzzer alerts
 * 
 * Components:
 * - Arduino (Uno/Nano)
 * - HC-SR04 Ultrasonic Sensor
 * - Vibration Motor
 * - Buzzer
 */

// Pin definitions
const int trigPin = 9;          // HC-SR04 Trigger Pin
const int echoPin = 10;         // HC-SR04 Echo Pin
const int vibrationPin = 3;     // Vibration Motor Pin
const int buzzerPin = 4;        // Buzzer Pin

// Distance thresholds (in centimeters)
const int dangerDistance = 50;   // Alert when obstacle is closer than 50cm
const int criticalDistance = 20; // Critical alert when very close

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibrationPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initial state - all outputs off
  digitalWrite(vibrationPin, LOW);
  digitalWrite(buzzerPin, LOW);
  
  Serial.println("Blind Assist System Initialized");
  Serial.println("Ready to detect obstacles...");
}

void loop() {
  // Get distance measurement
  long distance = measureDistance();
  
  // Print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check distance and provide appropriate alerts
  if (distance > 0 && distance < criticalDistance) {
    // Critical zone - strong continuous alert
    digitalWrite(vibrationPin, HIGH);
    tone(buzzerPin, 2000);  // High frequency tone
    delay(100);
  }
  else if (distance > 0 && distance < dangerDistance) {
    // Warning zone - pulsing alert (frequency increases as obstacle gets closer)
    int delayTime = map(distance, criticalDistance, dangerDistance, 100, 500);
    
    digitalWrite(vibrationPin, HIGH);
    tone(buzzerPin, 1500);
    delay(100);
    
    digitalWrite(vibrationPin, LOW);
    noTone(buzzerPin);
    delay(delayTime);
  }
  else {
    // Safe zone - no alerts
    digitalWrite(vibrationPin, LOW);
    noTone(buzzerPin);
    delay(100);
  }
}

// Function to measure distance using HC-SR04
long measureDistance() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 30000);  // Timeout after 30ms
  
  // Calculate distance (speed of sound is 343 m/s or 0.0343 cm/microsecond)
  // Distance = (Time × Speed of Sound) / 2
  long distance = duration * 0.0343 / 2;
  
  // Return 0 if no valid reading
  if (duration == 0) {
    return 0;
  }
  
  return distance;
}
