/*
  Microlab Example: RGB LED on Arduino Uno
  Repository: uc-Microlab

  Description:
  This example lights up the Red, Green, and Blue colors of an RGB LED connected to pins PB1, PB2, and PB3 of the Arduino Uno
  (corresponding to digital pins 9, 10, and 11) in 0.5 second intervals.

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
    For common anode LEDs, invert the logic levels (HIGH <-> LOW) in the commands.
*/

#define RED_PIN    9   // PB1
#define GREEN_PIN  10  // PB2
#define BLUE_PIN   11  // PB3

#define DELAY      500 // Delay in microseconds

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // Red
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  delay(DELAY);

  // Green
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
  delay(DELAY);

  // Blue
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  delay(DELAY);
}
