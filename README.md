# Accident-Detection-and-Alert-System

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>CodSoft Internship - Python Programming</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 20px;
      color: #333;
      line-height: 1.6;
    }
    h1, h2, h3 {
      color: #007bff;
    }
    p, ol {
      color: #666;
    }
    code {
      background-color: #f8f9fa;
      padding: 2px 5px;
      border-radius: 4px;
      font-family: Consolas, monospace;
    }
    pre {
      background-color: #f8f9fa;
      padding: 10px;
      border-radius: 4px;
      overflow-x: auto;
      margin: 10px 0;
    }
  </style>
</head>
<body>
  <h1>CodSoft Internship - Python Programming</h1>
  <p>Welcome to my Internship Repo! This repository contains a collection of tasks completed during my internship at CodSoft in Python Programming.</p>

  <h2>Tasks</h2>
  <ol>
    <li>
      <h3>Calculator</h3>
      <ol>
        <li>The calculator application allows users to perform basic arithmetic operations.</li>
        <li>Users can input two numbers and an operator to get the result of the operation.</li>
        <li>Features include resizing, where buttons and fonts dynamically adjust when the screen is resized.</li>
      </ol>
    </li>
    <li>
      <h3>ToDo List</h3>
      <ol>
        <li>Add Tasks: Users can easily add tasks to the list by entering task descriptions in the input field and clicking the "Add Task" button.</li>
        <li>View Tasks: The application displays a list of tasks added by the user. Tasks are listed with checkboxes indicating their completion status. Users can easily view their pending tasks at a glance.</li>
        <li>Mark Tasks as Complete: Users can mark tasks as complete by selecting them from the list and clicking the "Mark Complete" button. This feature helps users track their progress and stay organized.</li>
        <li>Delete Tasks: Users can delete tasks from the list by selecting them and clicking the "Delete Task" button. This allows users to remove unnecessary or completed tasks and keep their list clean and manageable.</li>
      </ol>
    </li>
    <li>
      <h3>Rock Paper Scissors Game</h3>
      <ol>
        <li>User Input: Allow users to select between rock, paper, or scissors.</li>
        <li>Computer Selection: Generate a random choice (rock, paper, or scissors) for the computer.</li>
        <li>Game Logic: Determine the winner based on the user's choice and the computer's choice. Rock crushes scissors, scissors cuts paper, and paper covers rock.</li>
        <li>Score Tracking: Keep track of the user's and computer's scores for multiple rounds.</li>
        <li>Visual Feedback: Provide visual feedback to indicate the result of each round, such as highlighting the winning option or displaying a message.</li>
        <li>Play Again: Allow users to play multiple rounds of the game without restarting the application.</li>
      </ol>
    </li>
  </ol>

  <h2>Arduino Code</h2>
  <pre><code>
#include &lt;Wire.h&gt;
#include &lt;Adafruit_MPU6050.h&gt;
#include &lt;Adafruit_Sensor.h&gt;
#include &lt;ESP8266WiFi.h&gt;
#include &lt;WiFiClientSecure.h&gt;
#include &lt;ThingSpeak.h&gt;
#include &lt;base64.h&gt;

const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* account_sid = "ACCOUNT SID";
const char* auth_token = "Token";
String from_number = ""; // Your Twilio phone number
String to_number = ""; // Receiver's phone number

// Twilio server fingerprint
const char fingerprint[] = "Fingerprint";

Adafruit_MPU6050 mpu;

const int vibrationPin = D0; // Connect vibration sensor OUT pin to D0 pin
const int vibrationThreshold = 800; // Adjust the vibration threshold value as needed
const float rotationThreshold = -0.5; // Adjust the rotation threshold value as needed
const int gasSensorPin = A0;

unsigned long channelID = Channel ID;
const char *writeAPIKey = "API KEY";

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
  
  String auth_header = get_auth_header(account_sid, auth_token
