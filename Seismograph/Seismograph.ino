/*
   Seisomograph code for use with the Tuned Mass Damper lesson plan
   Available from the Microsoft Education Workshop at http://aka.ms/hackingSTEM

   This projects uses an Arduino UNO microcontroller board. More information can
   be found by visiting the Arduino website: https://www.arduino.cc/en/main/arduinoBoardUno

   The Seismograph reading is read from analog pin A0 and the results are sent out the serial port.
   Data can be visualized in Microsoft Excel using the Project Cordoba add-in.

   2017 Microsoft EDU Workshop

*/

int sensorReading = 0;
int dataSpeed = 75; //Change this variable to slow down or speed up the rate data is fed into Excel

// Setup function where we initialize serial communications.
void setup()
{
  // Enable serial communications.
  Serial.begin(9600);
  pinMode (A0, INPUT);
  //analogReference(INTERNAL);
}

void loop()
{
  sensorReading = analogRead(A0) * 5 ;

  sensorReading = sensorReading;
  sendSerialData(); // Print out the sensor reading.
  //delay(75);

  sensorReading = -sensorReading;
  sendSerialData();     // Print out the sensor reading.
  delay(dataSpeed);
}

void sendSerialData()
{
  Serial.print(sensorReading);
  Serial.println();
}



