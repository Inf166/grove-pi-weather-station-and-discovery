#include "../grovepi.h"

/**
*   LCD backlight displays text with background color
**/

int main(){
    char text[200];
    float value = 332.0;
    // connect to the i2c-line
    connectLCD();

    // set text and RGB color on the LCD
    setText("Dies ist ein neuer Text");
    setRGB(216, 76, 216);

    //switching throw several colors
    for(int value = 0; value < 256; value++)
    {
        setRGB(value, 255 - value, 0);
        pi_sleep(10);
    }
    // set final color
    setRGB(0, 255, 0);

    // save the text with a value in char array "text" with the function "sprintf()"
    sprintf(text, "Text mit Wert: %02f", value);
    //show the text
    setText(text);

    return 0;


}
