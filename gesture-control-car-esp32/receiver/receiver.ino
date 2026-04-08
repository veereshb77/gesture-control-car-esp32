// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

// Set Motor PINs
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21

// Initialize global variables for movement commands
bool front = false;
bool back = false;
bool left = false;
bool right = false;

// Struct to receive data (must match the transmitter's struct)
typedef struct {
  bool f; // Front
  bool b; // Back
  bool l; // Left
  bool r; // Right
} message;

// Create an instance of the message struct to hold incoming data
message data;

// Callback function to handle incoming data
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  // Check if the received data length matches the expected size
  if (len == sizeof(message)) {
    memcpy(&data, incomingData, sizeof(data));

    // Map the received data to movement directions
    front = data.f;
    back  = data.b;
    left  = data.l;
    right = data.r;

       // Optional: Print received data for debugging
       // Serial.print("Received Data - Front: ");
       // Serial.print(front);
       // Serial.print(", Back: ");
       // Serial.print(back);
       // Serial.print(", Left: ");
       // Serial.print(left);
       // Serial.print(", Right: ");
       // Serial.println(right);
  } else {
    Serial.println("Received unexpected data length");
  }
}

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(115200);
  Serial.println("ESP-NOW Receiver Initialized");

  // Initialize motor pins as OUTPUT
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set all motors to stop initially
  carStop();

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi Mode set to STA");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
  // Retry initialization after a delay
  
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized Successfully");
  }

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receive Callback Registered");
}

void loop() {
  // Control the car based on received commands
  if (front) {
    carforward();
  } 
  else if (back) {
    carbackward();
  } 
  else if (left) {
    carturnleft();
  } 
  else if (right) {
    carturnright();
  } 
  else {
    carStop();
  }

  // Small delay to stabilize loop execution
  delay(100);
}

// Functions for specific movements

// Move the car forward
void carforward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Moving Forward");
}

// Move the car backward
void carbackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Car Moving Backward");
}

// Turn the car to the left
void carturnleft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Turning Left");
}

// Turn the car to the right
void carturnright() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Car Turning Right");
}

// Stop the car
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Car Stopped");
}
