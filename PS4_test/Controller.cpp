#include "Controller.h"
#include "define.h"

Controller::Controller(HardwareSerial *_Ser)
{
    Ser = _Ser;

    ButtonState = 0;
    LJoyX = 128;
    LJoyY = 128;
    RJoyX = 128;
    RJoyY = 128;
}

void Controller::update()
{
    unsigned int checksum;
    //uint8_t tmp;
    char c;
    char recv_msgs[10];
    while (SERIAL_XBEE.available())
    {
        c = SERIAL_XBEE.read();
        if (c == '\n')
        {

            if (recv_num == 10)
            {
                checksum = 0;

                for (int i = 0; i < 9; i++)
                {
                    recv_msgs[i] = recv_msgs[i] - 0x20;
                    checksum += (unsigned int)recv_msgs[i];
                }

                if ((checksum & 0x3F) == (recv_msgs[9] - 0x20))
                {
                    digitalWrite(PIN_LED_1, !digitalRead(PIN_LED_1));
                    ButtonState = 0;//, LJoyX = 0, LJoyY = 0, RJoyX = 0, RJoyY = 0;

                    ButtonState |= recv_msgs[0] & 0x3F;
                    ButtonState |= (recv_msgs[1] & 0x3F) << 6;
                    ButtonState |= (recv_msgs[2] & 0x0F) << 12;

                    LJoyX = recv_msgs[3];
                    LJoyX |= (recv_msgs[4] & 0x03) << 6;

                    LJoyY = (recv_msgs[4] & 0x3C) >> 2;
                    LJoyY |= (recv_msgs[5] & 0x0F) << 4;

                    RJoyX = (recv_msgs[5] & 0x30) >> 4;
                    RJoyX |= (recv_msgs[6] & 0x3F) << 2;

                    RJoyY = recv_msgs[7];
                    RJoyY |= (recv_msgs[8] & 0x03) << 6;

                    //   buttonB = ButtonState & BUTTON_B;

                    //   Serial.print(ButtonState, HEX);
                    //   /*Serial.print(recv_msgs[0], HEX);
                    //   Serial.print(",");
                    //   Serial.print(recv_msgs[1], HEX);
                    //   Serial.print(",");
                    //   Serial.print(recv_msgs[2], HEX);*/
                    //   Serial.print(",");
                    // Serial.print(LJoyX);
                    // Serial.print(",");
                    // Serial.print(LJoyY);
                    // Serial.print(",");
                    // Serial.print(RJoyX);
                    // Serial.print(",");
                    // Serial.println(RJoyY);
                }
            }
            recv_num = 0;
        }
        else
        {
            recv_msgs[recv_num++] = c;
        }
    }
    // char c;
    // preButtonState = ButtonState;

    // while(Ser->available()){
    //     c = Ser->read();
    //     if(c == '\n'){
    //         if(recv_num == 9){

    //             ButtonState = 0, LJoyX = 0, LJoyY = 0, RJoyX = 0, RJoyY = 0;
    //             ButtonState |= recv_msgs[0] - 0x20;
    //             ButtonState |= (recv_msgs[1] - 0x20) << 6;
    //             ButtonState |= (recv_msgs[2] - 0x20) << 12;

    //             LJoyX |= (recv_msgs[3] - 0x20);
    //             LJoyX |= ((recv_msgs[4] - 0x20) & 0x03) << 6;

    //             LJoyY |= ((recv_msgs[4] - 0x20) & 0x3C) >> 2;
    //             LJoyY |= ((recv_msgs[5] - 0x20) & 0x0F) << 4;

    //             RJoyX |= ((recv_msgs[5] - 0x20) & 0x30) >> 4;
    //             RJoyX |= ((recv_msgs[6] - 0x20) & 0x3F) << 2;

    //             RJoyY |= (recv_msgs[7] - 0x20);
    //             RJoyY |= ((recv_msgs[8] - 0x20) & 0x03) << 6;
    //         }
    //         recv_num = 0;
    //     }
    //     else{
    //         recv_msgs[recv_num] = c;
    //         recv_num++;
    //     }
    // }

    // buttonChanged = (preButtonState == ButtonState);
    // return 1;
}

void controller_receive()
{
}

void Controller::begin()
{
    Ser->begin(BAUD_RATE_PROMICRO);
}

int8_t Controller::readButton(int buttonNum) const
{
    int8_t ret = 0;

    if ((ButtonState >> (buttonNum - 1)) & 0x01)
    {
        ret += 1;
    }
    if (preButtonState >> (buttonNum - 1) & 0x01)
    {
        ret -= 1;
    }

    return ret;
}

unsigned int Controller::getButtonState() const
{
    return ButtonState;
}

bool Controller::getButtonChanged() const
{
    return buttonChanged;
}

double Controller::readJoyLX() const
{
    return -((double)LJoyX - 128.0) / 128.0;
}
double Controller::readJoyLY() const
{
    return -((double)LJoyY - 128.0) / 128.0;
}
double Controller::readJoyRX() const
{
    return -((double)RJoyX - 128.0) / 128.0;
}
double Controller::readJoyRY() const
{
    return -((double)RJoyY - 128.0) / 128.0;
}
/*
coords Controller::getVel_xyz(){
    coords refV;
    refV.x = -((double)LJoyX - 128.0) / 128.0;
    refV.y = -((double)LJoyY - 128.0) / 128.0;
    refV.z = -((double)RJoyY - 128.0) / 128.0;
    return refV;
}
*/
unsigned int Controller::readJoyLXbyte() const
{
    return LJoyX;
}
unsigned int Controller::readJoyLYbyte() const
{
    return LJoyY;
}
unsigned int Controller::readJoyRXbyte() const
{
    return RJoyX;
}
unsigned int Controller::readJoyRYbyte() const
{
    return RJoyY;
}
/*
coords_4 Controller::getMDcmd_fourOmni(double refVelx, double refVely, double refVelz)
{
    double refOmega1, refOmega2, refOmega3, refOmega4;
    refOmega1 = (+refVelx*SINCOS_PI_4 + refVely*SINCOS_PI_4 + refVelz * DIST2WHEEL) / WHEEL_R * GEARRATIO;
    refOmega2 = (-refVelx*SINCOS_PI_4 + refVely*SINCOS_PI_4 + refVelz * DIST2WHEEL) / WHEEL_R * GEARRATIO;
    refOmega3 = (-refVelx*SINCOS_PI_4 - refVely*SINCOS_PI_4 + refVelz * DIST2WHEEL) / WHEEL_R * GEARRATIO;
    refOmega4 = (+refVelx*SINCOS_PI_4 - refVely*SINCOS_PI_4 + refVelz * DIST2WHEEL) / WHEEL_R * GEARRATIO;

    coords_4 MDcmd;
    MDcmd.a = refOmega1 * _2RES_PI;
    MDcmd.b = refOmega2 * _2RES_PI;
    MDcmd.c = refOmega3 * _2RES_PI;
    MDcmd.d = refOmega4 * _2RES_PI;

    return MDcmd;
}
*/