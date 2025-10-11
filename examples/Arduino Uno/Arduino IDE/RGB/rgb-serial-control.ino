/*
  Microlab Example: Serial RGB Color Control on Arduino Uno by Serial Monitor
  Repository: uc-Microlab

  Description:
  This example receives three values (0 to 255) via the serial monitor, each representing the intensity of Red, Green, and Blue
  to control an RGB LED connected to pins PB1, PB2, and PB3 of the Arduino Uno (digital pins 9, 10, 11).

  Hardware: uc-Microlab - version r1

  Author: otavioacb
  Date: 2025-10-11

  License:
    This example is licensed under the MIT License.
    See the LICENSE file in the repository for more information.

  Connection details:
    - PB1 (Digital 9)   -> Red (R)
    - PB2 (Digital 10)  -> Green (G)
    - PB3 (Digital 11)  -> Blue (B)

  Note:
    For common anode LEDs, invert the logic levels (write 255-value) in analogWrite commands.
*/

#define RED_PIN    9   // PB1
#define GREEN_PIN  10  // PB2
#define BLUE_PIN   11  // PB3

uint8_t r, g, b;

void setup() {
  // Start serial comm and print the guide message
  Serial.begin(9600);
  Serial.println("Enter three values (0-255) for R G B separated by spaces:");

  // Defining each RGB pin as OUTPUT
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Set RGB as 0
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void loop() {
  if(Serial.available() > 1)
  {
    r = Serial.parseInt();
    g = Serial.parseInt();
    b = Serial.parseInt();

    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
  }
}
