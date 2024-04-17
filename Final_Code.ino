#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ThingSpeak.h>
#include <base64.h>

const char* ssid = "SSID";
const char* password = "Password";

const char* account_sid = "Account SID";
const char* auth_token = "Auth Token";
String from_number = ""; // Your Twilio phone number
String to_number = ""; // Receiver's phone number

// Twilio server fingerprint
const char fingerprint[] = "80:58:F5:0B:A9:DF:A0:B8:49:5B:87:7C:82:47:75:04:FB:D2:9D:FD ";

Adafruit_MPU6050 mpu;

const int vibrationPin = D0; // Connect vibration sensor OUT pin to D0 pin
const int vibrationThreshold = 800; // Adjust the vibration threshold value as needed
const float rotationThreshold = -0.5; // Adjust the rotation threshold value as needed
const int gasSensorPin = A0;

unsigned long channelID = 2490287;
const char *writeAPIKey = "XG7GBU3GZ5AEA57Y";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("");
  Serial.println(F("+++ Setup."));

  // Connect to Wi-Fi
  Serial.println("+ Connect to WiFi.");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("+ Connected to WiFi, IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  pinMode(vibrationPin, INPUT); // Set vibrationPin as input

  // Initialize ThingSpeak library
  ThingSpeak.begin(client);
}

void loop() {
  // Read the state of the vibration sensor
  int vibrationValue = analogRead(vibrationPin);

  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Read analog voltage from gas sensor
  int sensorValue = analogRead(gasSensorPin);
  
  // Convert analog value to gas concentration (example conversion, may vary for different sensors)
  float voltage = sensorValue * (3.3 / 1023.0); // NodeMCU ESP8266 operates at 3.3V
  float gasConcentration = voltage * 100; // Example conversion to percentage
  
  // Print gas concentration to serial monitor
  Serial.print("Gas Concentration: ");
  Serial.print(gasConcentration);
  Serial.println("%");

  // Check for accident conditions
  if ((g.gyro.x < 0) || ((g.gyro.y < 0 || g.gyro.z < 0) && vibrationValue > vibrationThreshold) || gasConcentration > 200) {
    Serial.println("Accident Detected!");
    sendSMS(); // Call the function to send an SMS
  }

  // Update data on ThingSpeak
  ThingSpeak.writeField(channelID, 1, gasConcentration, writeAPIKey); // Gas concentration to Field 1
  ThingSpeak.writeField(channelID, 2, vibrationValue, writeAPIKey);   // Vibration value to Field 2
  ThingSpeak.writeField(channelID, 3, g.gyro.x, writeAPIKey);          // X-axis rotation to Field 3
  
  // Check if gas concentration is more than 100% or vibration and negative X-axis rotation are detected
  if (gasConcentration > 100 || (vibrationValue > vibrationThreshold && g.gyro.x < rotationThreshold)) {
    Serial.println("Accident Detected");
    Serial.println();
    
    // Stop the program
    while(1); // This keeps the program in an infinite loop, effectively stopping further execution
  }

  // Print out other sensor values
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2 ");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  Serial.println();
  delay(500);
}

void sendSMS() {
  // Use WiFiClientSecure to create a TLS 1.2 connection
  WiFiClientSecure client;
  client.setFingerprint(fingerprint);
  
  const char* host = "api.twilio.com";
  const int httpsPort = 443;
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("- Connection failed.");
    return;
  }
  
  String post_data = "To=" + urlencode(to_number) +
                     "&From=" + urlencode(from_number) +
                     "&Body=" + urlencode("Accident detected! Help needed."); // Change the message body as needed
  
  String auth_header = get_auth_header(account_sid, auth_token);
  String http_request = "POST /2010-04-01/Accounts/" + String(account_sid) + "/Messages HTTP/1.1\r\n" +
                      auth_header + "\r\n" +
                      "Host: " + host + "\r\n" +
                      "Cache-control: no-cache\r\n" +
                      "User-Agent: ESP8266 Twilio Example\r\n" +
                      "Content-Type: application/x-www-form-urlencoded\r\n" +
                      "Content-Length: " + String(post_data.length()) + "\r\n" +
                      "Connection: close\r\n" +
                      "\r\n" +
                      post_data +
                      "\r\n";
  client.println(http_request);

  // Read the response (optional)
  String response = "";
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    response += line;
    response += "\r\n";
  }
  
  Serial.println("Connection is closed.");
  Serial.println("Response:");
  Serial.println(response);
}

// Function to URL encode a string
String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}

// Function to generate the Authorization header
String get_auth_header(const String& user, const String& password) {
  String toencode = user + ":" + password;
  String encoded = base64::encode((uint8_t*)toencode.c_str(), toencode.length());
  return "Authorization: Basic " + encoded;
}
