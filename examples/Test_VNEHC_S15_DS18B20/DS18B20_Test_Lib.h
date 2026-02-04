#ifndef DS18B20_Test_Lib_H
#define DS18B20_Test_Lib_H

#include "kxnTask.h"
#include "Task_VNEHC_Test.h"
// #include "MKL_DHT.h"
#include "MKL_DS18B20.h"

#define DS18B20_TEST_LIB_SHOW_LOG(...)     if(this->debugPort && this->EnDebug) { this->debugPort->print(__VA_ARGS__); }
#define DS18B20_TEST_LIB_SHOW_LOG_LN(...)     if(this->debugPort && this->EnDebug) { this->debugPort->println(__VA_ARGS__); }

// MKL_DHT dht(PIN_PORT3_SIG, DHT11);

OneWire oneWire_10(PIN_PORT3_SIG);
MKL_DS18B20 sensor(&oneWire_10);


CREATE_TASK_STATE(DS18B20_Test_Lib)
  DS18B20_Test_Lib_Setup,
  DS18B20_Test_Lib_GetVoltTranBeforeOn,
  DS18B20_Test_Lib_GetVoltTranAfterOn,
  DS18B20_Test_Lib_GetVoltTranAfterOff,
  DS18B20_Test_Lib_ReportResult,
  DS18B20_Test_Lib_Test_Stop
END

CREATE_TASK(DS18B20_Test_Lib)

Task_VNEHC_Test *p_Task_VNEHC_Test = nullptr;
Stream *debugPort;
uint8_t pinA_Diod;
uint8_t pin_DS18B20;
uint8_t EnDebug = 1;

int adcValueBeforeOn = 0;
int adcValueAfterOnMax = 0;
float current_mA = 0.0;

uint8_t flagTestFailed = 0;

float tem = 0.0, humi = 0.0; // temperature and humidity

void setEnDebug(uint8_t pa_en)
{
  EnDebug = pa_en;
}

void setup(Task_VNEHC_Test *pa_p_Task_VNEHC_Test, Stream *pa_debugPort = &Serial, uint8_t pa_pinA_Diod = PIN_PORT4_RX, uint8_t pa_pin_DS18B20 = PIN_PORT3_SIG) {
  // put your setup code here, to run once:
  kxnTaskManager.add(this);
  this->p_Task_VNEHC_Test = pa_p_Task_VNEHC_Test;
  this->debugPort = pa_debugPort;
  this->pinA_Diod = pa_pinA_Diod;
  this->pin_DS18B20 = pa_pin_DS18B20;

  this->p_Task_VNEHC_Test->addOverCurrentCallback([](float current_mA, void *paClassInstance){
    DS18B20_Test_Lib *pDS18B20TestLib = (DS18B20_Test_Lib*)paClassInstance;
    pDS18B20TestLib->flagTestFailed = 1;
    if(pDS18B20TestLib->debugPort && pDS18B20TestLib->EnDebug)
    {
      pDS18B20TestLib->debugPort->print(F("Over Current from DS18B20 Test Lib: "));
      pDS18B20TestLib->debugPort->println(String(current_mA));
    }
    // pDS18B20TestLib->debugPort->print(F("Over Current from DS18B20 Test Lib"));
    // pDS18B20TestLib->debugPort->println(String(current_mA));
    pDS18B20TestLib->current_mA = current_mA;

    pDS18B20TestLib->debugPort->println(F("\t\tRESET MCU FOR NEXT"));
    while(1);
  }, this);

}

void start()
{
  setState(DS18B20_Test_Lib_Setup);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (getState())
  {
    case DS18B20_Test_Lib_Setup:
        // Serial.println(F("DS18B20 check Volt Sig"));
        // pinMode(PIN_PORT3_SIG, INPUT_PULLUP);
        // delay(10);
        // if(this->p_Task_VNEHC_Test->isPullUp_Port3_OK(1) != VNEHC_List_Error_None) 
        if(this->p_Task_VNEHC_Test->checkVolSignal3P(0) != VNEHC_List_Error_None) 
        {
          // this->p_Task_VNEHC_Test->showInfoWithErrorCode(VNEHC_List_Error_PORT3_R_PULLUP_FAIL);
          // while(1);
          // this->p_Task_VNEHC_Test->stop();
          this->p_Task_VNEHC_Test->checkVolSignal3P(1);
          Serial.println(F("DS18B20 Test Lib: Port3 Signal not pull-up to 3V3V"));
        }

        sensor.begin();

        setState(DS18B20_Test_Lib_ReportResult);
        kDelay(1000);
      break;

    case DS18B20_Test_Lib_ReportResult:
    {
        sensor.requestTemperatures();
        tem = sensor.getTempC();

        this->debugPort->println(F("---- DS18B20 Initial Read ----"));
        this->debugPort->print(F("Temperature: "));
        this->debugPort->println(String(tem));
        
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        

        // kDelay(0);

      setState(DS18B20_Test_Lib_Setup);
      break;
    }

    case DS18B20_Test_Lib_Test_Stop:
      break;


    default:
      break;
  }
  
}

END

#endif