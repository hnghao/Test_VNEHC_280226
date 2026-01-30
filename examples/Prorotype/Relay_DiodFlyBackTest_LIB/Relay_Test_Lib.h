#ifndef Relay_Test_Lib_H
#define Relay_Test_Lib_H

#include "kxnTask.h"
#include "Task_VNEHC_Test.h"

CREATE_TASK_STATE(Relay_Test_Lib)
  Relay_Test_Lib_Setup,
  Relay_Test_Lib_GetVoltTranBeforeOn,
  Relay_Test_Lib_GetVoltTranAfterOn,
  Relay_Test_Lib_GetVoltTranAfterOff,
  Relay_Test_Lib_Test_Stop
END

CREATE_TASK(Relay_Test_Lib)

Task_VNEHC_Test *p_Task_VNEHC_Test = nullptr;
Stream *debugPort;
uint8_t pinA_Diod;
uint8_t pin_Relay;

int adcValueBeforeOn = 0;

#define RELAY_ACTIVE  (1)

void setup(Task_VNEHC_Test *pa_p_Task_VNEHC_Test, Stream *pa_debugPort = &Serial, uint8_t pa_pinA_Diod = PIN_PORT4_RX, uint8_t pa_pin_Relay = PIN_PORT3_SIG) {
  // put your setup code here, to run once:
  kxnTaskManager.add(this);
  this->p_Task_VNEHC_Test = pa_p_Task_VNEHC_Test;
  this->debugPort = pa_debugPort;
  this->pinA_Diod = pa_pinA_Diod;
  this->pin_Relay = pa_pin_Relay;

  // pinMode(pin_Relay, OUTPUT);

  // Serial.begin(115200);
  // Serial.println(F("Start Test Relay"));

  // Serial.print(F("ADC Start: "));
  // Serial.println(analogRead(pinA_Diod));

  // digitalWrite(pin_Relay, RELAY_ACTIVE);
  // delay(200);

  // Serial.print(F("ADC on: "));
  // Serial.println(analogRead(pinA_Diod));
  
  // Serial.print(F("ADC off: "));
  // digitalWrite(pin_Relay, !RELAY_ACTIVE);

  // int tempArr[10];
  // for(int cf = 0; cf < 10; cf++)
  // {
  //   tempArr[cf] = analogRead(pinA_Diod);
  //   delayMicroseconds(10);
  // }

  // Serial.println("====");
  // for(int cf = 0; cf < 10; cf++)
  // {
  //   Serial.println(tempArr[cf]);
  // }

}

void start()
{
  setState(Relay_Test_Lib_Setup);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (getState())
  {
    case Relay_Test_Lib_Setup:
        pinMode(pin_Relay, OUTPUT);

        Serial.begin(115200);
        Serial.println(F("Start Test Relay"));

        Serial.print(F("ADC Start: "));
        setState(Relay_Test_Lib_GetVoltTranBeforeOn);
        kDelay(0);
      break;

    case Relay_Test_Lib_GetVoltTranBeforeOn:
        this->adcValueBeforeOn = analogRead(this->pinA_Diod); 
        Serial.println(adcValueBeforeOn);
        digitalWrite(pin_Relay, RELAY_ACTIVE);
        Serial.print(F("ADC on: "));
        kDelay(200);
        setState(Relay_Test_Lib_GetVoltTranAfterOn);
      break;

    case Relay_Test_Lib_GetVoltTranAfterOn:
        Serial.println(analogRead(pinA_Diod));
        setState(Relay_Test_Lib_GetVoltTranAfterOff);
        kDelay(0);
      break;

    case Relay_Test_Lib_GetVoltTranAfterOff:
    {
          Serial.print(F("ADC off: "));
          digitalWrite(pin_Relay, !RELAY_ACTIVE);

          int tempArr[10];
          for(int cf = 0; cf < 10; cf++)
          {
            tempArr[cf] = analogRead(pinA_Diod);
            delayMicroseconds(10);
          }
          Serial.println("====");
          for(int cf = 0; cf < 10; cf++)
          {
            Serial.println(tempArr[cf]);
          }

          setState(Relay_Test_Lib_Test_Stop);
        break;
      }
    case Relay_Test_Lib_Test_Stop:
      break;


    default:
      break;
  }
  
}

END

#endif