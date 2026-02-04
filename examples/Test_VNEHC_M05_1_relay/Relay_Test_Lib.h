#ifndef Relay_Test_Lib_H
#define Relay_Test_Lib_H

#include "kxnTask.h"
#include "Task_VNEHC_Test.h"

#define RELAY_TEST_LIB_SHOW_LOG(...)     if(this->debugPort && this->EnDebug) { this->debugPort->print(__VA_ARGS__); }
#define RELAY_TEST_LIB_SHOW_LOG_LN(...)     if(this->debugPort && this->EnDebug) { this->debugPort->println(__VA_ARGS__); }

CREATE_TASK_STATE(Relay_Test_Lib)
  Relay_Test_Lib_Setup,
  Relay_Test_Lib_GetVoltTranBeforeOn,
  Relay_Test_Lib_GetVoltTranAfterOn,
  Relay_Test_Lib_GetVoltTranAfterOff,
  Relay_Test_Lib_ReportResult,
  Relay_Test_Lib_Test_Stop
END

CREATE_TASK(Relay_Test_Lib)

Task_VNEHC_Test *p_Task_VNEHC_Test = nullptr;
Stream *debugPort;
uint8_t pinA_Diod;
uint8_t pin_Relay;
uint8_t EnDebug = 1;

int adcValueBeforeOn = 0;
int adcValueAfterOnMax = 0;
float current_mA = 0.0;

uint8_t flagTestFailed = 0;

#define RELAY_ACTIVE  (1)

void setEnDebug(uint8_t pa_en)
{
  EnDebug = pa_en;
}

void setup(Task_VNEHC_Test *pa_p_Task_VNEHC_Test, Stream *pa_debugPort = &Serial, uint8_t pa_pinA_Diod = PIN_PORT4_RX, uint8_t pa_pin_Relay = PIN_PORT3_SIG) {
  // put your setup code here, to run once:
  kxnTaskManager.add(this);
  this->p_Task_VNEHC_Test = pa_p_Task_VNEHC_Test;
  this->debugPort = pa_debugPort;
  this->pinA_Diod = pa_pinA_Diod;
  this->pin_Relay = pa_pin_Relay;

  this->p_Task_VNEHC_Test->addOverCurrentCallback([](float current_mA, void *paClassInstance){
    Relay_Test_Lib *pRelayTestLib = (Relay_Test_Lib*)paClassInstance;
    pRelayTestLib->flagTestFailed = 1;
    if(pRelayTestLib->debugPort && pRelayTestLib->EnDebug)
    {
      pRelayTestLib->debugPort->print(F("Over Current from Relay Test Lib: "));
      pRelayTestLib->debugPort->println(String(current_mA));
    }
    // pRelayTestLib->debugPort->print(F("Over Current from Relay Test Lib"));
    // pRelayTestLib->debugPort->println(String(current_mA));
    pRelayTestLib->current_mA = current_mA;
  }, this);

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

        // Serial.begin(115200);
        // Serial.println(F("Start Test Relay"));

        // Serial.print(F("ADC Start: "));
        setState(Relay_Test_Lib_GetVoltTranBeforeOn);
        kDelay(0);
      break;

    case Relay_Test_Lib_GetVoltTranBeforeOn:
        this->adcValueBeforeOn = analogRead(this->pinA_Diod); 
        // Serial.println(adcValueBeforeOn);
        digitalWrite(pin_Relay, RELAY_ACTIVE);
        // Serial.print(F("ADC on: "));
        kDelay(200);
        setState(Relay_Test_Lib_GetVoltTranAfterOn);
      break;

    case Relay_Test_Lib_GetVoltTranAfterOn:
        // Serial.println(analogRead(pinA_Diod));
        if(this->current_mA == 0)
        {
          this->current_mA = this->p_Task_VNEHC_Test->getPort3_mA();
        }
        setState(Relay_Test_Lib_GetVoltTranAfterOff);
        kDelay(0);
      break;

    case Relay_Test_Lib_GetVoltTranAfterOff:
    {
          // Serial.print(F("ADC off: "));
          digitalWrite(pin_Relay, !RELAY_ACTIVE);

          int tempArr[10];
          for(int cf = 0; cf < 10; cf++)
          {
            tempArr[cf] = analogRead(pinA_Diod);
            delayMicroseconds(10);
          }
          // Serial.println("====");
          for(int cf = 0; cf < 10; cf++)
          {
            // Serial.println(tempArr[cf]);
            adcValueAfterOnMax = max(adcValueAfterOnMax, tempArr[cf]);
          }

          setState(Relay_Test_Lib_ReportResult);
        break;
      }

    case Relay_Test_Lib_ReportResult:
    {
      RELAY_TEST_LIB_SHOW_LOG_LN(F("---- Relay Test Result ----"));
      RELAY_TEST_LIB_SHOW_LOG(F("Volt Tran Before On: "));
      RELAY_TEST_LIB_SHOW_LOG_LN(String(this->adcValueBeforeOn));
      RELAY_TEST_LIB_SHOW_LOG(F("Volt Tran After On: "));
      RELAY_TEST_LIB_SHOW_LOG_LN(String(analogRead(this->pinA_Diod)));
      RELAY_TEST_LIB_SHOW_LOG(F("Volt Tran After Off: "));
      RELAY_TEST_LIB_SHOW_LOG_LN(String(this->adcValueAfterOnMax));
      RELAY_TEST_LIB_SHOW_LOG(F("Current mA through Relay: "));
      RELAY_TEST_LIB_SHOW_LOG_LN(String(this->current_mA));
      RELAY_TEST_LIB_SHOW_LOG_LN(F("---- End Relay Test ----"));

      if((this->adcValueBeforeOn == 0) || (this->adcValueAfterOnMax == 0))
      {
        this->debugPort->println(F("Kim Test Tiep xuc khong tot voi chan 3 Transistor"));
        this->flagTestFailed = 1;
      }
      else if(this->adcValueAfterOnMax > (this->adcValueBeforeOn * 130/100))
      {
        this->debugPort->println(F("\t\tDiod Flyback ERROR!"));
        this->flagTestFailed = 1;
      }
      else
      {
        // this->debugPort->println(F("Result: Test Passed!"));
      }

      if(IS_INRANGE(this->current_mA,65,85))
      {
        // this->debugPort->println(F("\t\tRelay GOOD * * * * *"));
        // Serial.println(F("RESET Tool Test for next"));
      }
      else if(this->current_mA < 65)
      {
        this->debugPort->println(F("ERROR! Curent < 65mA"));
        this->debugPort->println(F("\t\t Kiem tra ho chan R ke ben Transistor, Transistor, Relay"));
        this->p_Task_VNEHC_Test->showInfoWithErrorCode(VNEHC_List_Error_PORT3_CURRENT_LOWER);
        this->flagTestFailed = 1;
        // Serial.println(F(""));
        // Serial.println(F("RESET Tool Test for next"));
      }
      else if(this->current_mA > 85)
      {
        this->debugPort->println(F("\t\tERROR! Curent > 85mA"));
        this->p_Task_VNEHC_Test->showInfoWithErrorCode(VNEHC_List_Error_PORT3_CURRENT_HIGHER);
        this->flagTestFailed = 1;
        // Serial.println(F(""));
        // Serial.println(F("RESET Tool Test for next"));
      }

      if(this->flagTestFailed == 1)
      {
        this->debugPort->println(F("\t\t\tTEST RELAY ERROR!"));
      }
      else
      {
        this->debugPort->println(F("\t\t\tTEST RELAY GOOD!"));
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