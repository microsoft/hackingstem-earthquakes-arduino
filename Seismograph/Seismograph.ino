//===----------------__ Hacking STEM Seismograph Arduino __------------===//
// For use with the Using Computational Thinking to Understand Earthquakes 
// Seisomograph lesson plan available from Microsoft Education Workshop 
// at http://aka.ms/hackingSTEM
//
// Overview:
//
//   This projects uses an Arduino UNO microcontroller board. More 
//   information can be found by visiting the Arduino website: 
//   https://www.arduino.cc/en/main/arduinoBoardUno
//
//   The Seismograph reading is read from analog pin A0 and the results 
//   are sent out the serial port. Data can be visualized in Microsoft 
//   Excel using the Project Cordoba add-in.
//
// This project uses an Arduino UNO microcontroller board, information at:
// https://www.arduino.cc/en/main/arduinoBoardUno
//
// Comments, contributions, suggestions, bug reports, and feature requests
// are welcome! For source code and bug reports see:
// http://github.com/[TODO github path to Hacking STEM]
//
// Copyright Microsoft EDU Workshop - HackingSTEM
// MIT License terms detailed in LICENSE.txt 
//===----------------------------------------------------------------------===//

int sensorReading = 0;
int dataSpeed = 75; //Rate in milliseconds data is fed into Excel

// Setup function where we initialize serial communications.
void setup()
{
  // Enable serial communications.
  Serial.begin(9600);
  pinMode (A0, INPUT);
}

void loop()
{
  sensorReading = analogRead(A0) * 5 ; // 5 volt is our reference voltage

  sensorReading = sensorReading;
  sendSerialData(); // Print out the sensor reading.
// TODO why don't we pause between sends?

  sensorReading = -sensorReading;  // TODO why are we sending negative?
  sendSerialData();     // Print out the sensor reading.
  delay(dataSpeed);
}

void sendSerialData()
{
  Serial.print(sensorReading); // TODO why no commas
  Serial.println();
}


