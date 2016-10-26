#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

struct payload_t
{
    uint8_t axis_lh;
    uint8_t axis_lv;
    uint8_t axis_rh;
    uint8_t axis_rv;
    uint8_t button_r;
    uint8_t button_g;
    uint8_t button_b;
    uint8_t button_p;
    uint8_t button_rs;
    uint8_t button_ls;
    uint8_t button_ru;
    uint8_t button_rd;
    uint8_t button_lu;
    uint8_t button_ld;
    uint8_t mode;
};

RF24 radio(10,9);
RF24Network network(radio);

const uint16_t this_node = 01;
const uint16_t other_node = 00;
const uint16_t interval = 100;

uint16_t last_sent;

payload_t payload;


void setup(void)
{
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);

    Serial.begin(57600);
    SPI.begin();

    radio.begin();
    network.begin(90, this_node);

    Serial.println("Controller firmware v1.0");
}


void loop()
{
    network.update();

    uint16_t now = millis();
    if (now - last_sent >= interval)
    {
        last_sent = now;
        Serial.print("Sending... ");
        payload.axis_lh = analogRead(A0) >> 2;
        payload.axis_lv = 255 - (analogRead(A2) >> 2);
        payload.axis_rh = analogRead(A1) >> 2;
        payload.axis_rv = 255 - (analogRead(A3) >> 2);
        payload.button_r = (analogRead(A7) < 512) ? 1 : 0;
        payload.button_g = !(digitalRead(A4));
        payload.button_b = !(digitalRead(A5));
        payload.button_p = (analogRead(A6) < 512) ? 1 : 0;
        payload.button_rs = !(digitalRead(8));
        payload.button_ls = !(digitalRead(7));
        payload.button_ru = !(digitalRead(3));
        payload.button_rd = !(digitalRead(4));
        payload.button_lu = !(digitalRead(5));
        payload.button_ld = !(digitalRead(6));
        payload.mode = 0;
        RF24NetworkHeader header(other_node);
        bool ok = network.write(header,&payload,sizeof(payload));
        delay(50);
        if (ok)
        {
            Serial.println("ok.");
        }
        else
        {
            Serial.println("failed.");
        }
    }
}
