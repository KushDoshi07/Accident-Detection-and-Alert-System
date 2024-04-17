<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IoT Project - Accident Detection and Alert System</title>
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
  <h1>IoT Project - Accident Detection and Alert System</h1>

  <h2>Project Overview</h2>
  <p>The IoT project involves building a system for monitoring environmental conditions and detecting accidents using sensors and cloud communication.</p>

  <h2>Features</h2>
  <ol>
    <li>
      <h3>Sensor Integration</h3>
      <ol>
        <li>Integration of MPU6050 accelerometer and gyroscope sensor for motion detection.</li>
        <li>Utilization of gas sensor for measuring environmental gas concentration.</li>
        <li>Connection of vibration sensor for detecting physical impacts or movements.</li>
      </ol>
    </li>
    <li>
      <h3>Data Transmission</h3>
      <ol>
        <li>Establishment of Wi-Fi connection for data transmission to cloud services.</li>
        <li>Integration with ThingSpeak for real-time data logging and visualization.</li>
        <li>Secure communication using TLS 1.2 protocol for data privacy.</li>
      </ol>
    </li>
    <li>
      <h3>Accident Detection</h3>
      <ol>
        <li>Implementation of accident detection logic based on sensor readings.</li>
        <li>Recognition of hazardous conditions such as excessive gas concentration or physical impacts.</li>
        <li>Automatic triggering of alerts and notifications in case of accidents.</li>
      </ol>
    </li>
    <li>
      <h3>Alert System</h3>
      <ol>
        <li>Integration with Twilio API for sending SMS alerts to predefined phone numbers.</li>
        <li>Customizable message content and recipient phone numbers for emergency notifications.</li>
        <li>Secure communication with Twilio server using server fingerprint verification.</li>
      </ol>
    </li>
  </ol>
</body>
</html>
