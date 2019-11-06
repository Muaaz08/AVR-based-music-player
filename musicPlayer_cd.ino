/*
  AVR Based Music Player

  This example shows how to play three songs from SD card by pressing a push button

  The circuit:
   1.Push Button on pin 2 and 3
   2.Audio Out - pin 9
   3.SD card attached to SPI bus as follows:
    MOSI - pin 11
    MISO - pin 12
    CLK  - pin 13
    CS   - pin 8
   4. LCD
    RS - A3
    EN - A2
    D4-D7 - D4-D7
*/

#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card
#include<LiquidCrystal.h> //lib for lcd 

#define SD_ChipSelectPin 8 //Chip select is pin number 8
TMRpcm music; //Lib object is named "music"
LiquidCrystal lcd(A3, A2, 4, 5, 6, 7);
int song_number = 0;
boolean debounce1 = true;
boolean debounce2 = true;
boolean play_pause;

void setup() {
  music.speakerPin = 9; //Auido out on pin 9
  Serial.begin(9600); //Serial Com for debugging
  Serial.println("AVR MUSIC PLAYER");
  lcd.begin(16, 2);
  lcd.print("AVR MUSIC PLAYER");

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    lcd.setCursor(0, 1);
    lcd.print("SD fail");
    while (1);
  }

  pinMode(2, INPUT_PULLUP); //Button 1 with internal pull up to chage track
  pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to play/pause
  pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to fast forward

  music.setVolume(3);    //   0 to 7. Set volume level
  music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal
  //music.volume(0);        //   1(up) or 0(down) to control volume
  //music.play("filename",30); plays a file starting at 30 seconds into the track
}

void loop()
{
  if (digitalRead(2) == LOW  && debounce1 == true) { //Button 1 Pressed
    delay(10);
    song_number++;
    if (song_number == 4) song_number = 1;

    debounce1 = false;

    Serial.println("BUTTON PRESSED");
    Serial.print("Song Number = ");
    Serial.println(song_number);
    lcd.clear();
    lcd.print("Song Number = ");
    lcd.print(song_number);

    if (song_number == 1) {
      music.play("1.wav"); //Play song 1 from beginning
    }
    if (song_number == 2) {
      music.play("2.wav", 30); //Play song 2 from 30th second
    }
    if (song_number == 3) {
      music.play("3.wav", 10); //Play song 3 from 10th second
    }
  }
  if (digitalRead(3) == LOW  && debounce2 == true) { //Button 2 Pressed
    music.pause();
    Serial.println("PLAY / PAUSE");
    lcd.clear();
    lcd.print("Song Number = ");
    lcd.print(song_number);
    lcd.setCursor(0, 1);
    lcd.print("PLAY / PAUSE");
    debounce2 = false;
  }

  if (digitalRead(2) == HIGH) //Avoid debounce
    debounce1 = true;

  if (digitalRead(3) == HIGH) //Avoid debounce
    debounce2 = true;
}
