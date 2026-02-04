#ifndef DHT11_Test_Lib_H
#define DHT11_Test_Lib_H

#include "kxnTask.h"
#include "Task_VNEHC_Test.h"
#include "MKL_DHT.h"

#define DHT11_TEST_LIB_SHOW_LOG(...)     if(this->debugPort && this->EnDebug) { this->debugPort->print(__VA_ARGS__); }
#define DHT11_TEST_LIB_SHOW_LOG_LN(...)     if(this->debugPort && this->EnDebug) { this->debugPort->println(__VA_ARGS__); }

MKL_DHT dht(PIN_PORT3_SIG, DHT11);

CREATE_TASK_STATE(DHT11_Test_Lib)
  DHT11_Test_Lib_Setup,
  DHT11_Test_Lib_GetVoltTranBeforeOn,
  DHT11_Test_Lib_GetVoltTranAfterOn,
  DHT11_Test_Lib_GetVoltTranAfterOff,
  DHT11_Test_Lib_ReportResult,
  DHT11_Test_Lib_Test_Stop
END

CREATE_TASK(DHT11_Test_Lib)

Task_VNEHC_Test *p_Task_VNEHC_Test = nullptr;
Stream *debugPort;
uint8_t pinA_Diod;
uint8_t pin_DHT11;
uint8_t EnDebug = 1;

int adcValueBeforeOn = 0;
int adcValueAfterOnMax = 0;
float current_mA = 0.0;

uint8_t flagTestFailed = 0;

#define DHT11_ACTIVE  (1)
float tem = 0.0, humi = 0.0; // temperature and humidity

void setEnDebug(uint8_t pa_en)
{
  EnDebug = pa_en;
}

void setup(Task_VNEHC_Test *pa_p_Task_VNEHC_Test, Stream *pa_debugPort = &Serial, uint8_t pa_pinA_Diod = PIN_PORT4_RX, uint8_t pa_pin_DHT11 = PIN_PORT3_SIG) {
  // put your setup code here, to run once:
  kxnTaskManager.add(this);
  this->p_Task_VNEHC_Test = pa_p_Task_VNEHC_Test;
  this->debugPort = pa_debugPort;
  this->pinA_Diod = pa_pinA_Diod;
  this->pin_DHT11 = pa_pin_DHT11;

  this->p_Task_VNEHC_Test->addOverCurrentCallback([](float current_mA, void *paClassInstance){
    DHT11_Test_Lib *pDHT11TestLib = (DHT11_Test_Lib*)paClassInstance;
    pDHT11TestLib->flagTestFailed = 1;
    if(pDHT11TestLib->debugPort && pDHT11TestLib->EnDebug)
    {
      pDHT11TestLib->debugPort->print(F("Over Current from DHT11 Test Lib: "));
      pDHT11TestLib->debugPort->println(String(current_mA));
    }
    // pDHT11TestLib->debugPort->print(F("Over Current from DHT11 Test Lib"));
    // pDHT11TestLib->debugPort->println(String(current_mA));
    pDHT11TestLib->current_mA = current_mA;
  }, this);

}

void start()
{
  setState(DHT11_Test_Lib_Setup);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (getState())
  {
    case DHT11_Test_Lib_Setup:
        // Serial.println(F("DHT11 check Volt Sig"));
        // pinMode(PIN_PORT3_SIG, INPUT_PULLUP);
        // delay(10);
        // if(this->p_Task_VNEHC_Test->isPullUp_Port3_OK(1) != VNEHC_List_Error_None) 
        if(this->p_Task_VNEHC_Test->checkVolSignal3P(0) != VNEHC_List_Error_None) 
        {
          // this->p_Task_VNEHC_Test->showInfoWithErrorCode(VNEHC_List_Error_PORT3_R_PULLUP_FAIL);
          // while(1);
          // this->p_Task_VNEHC_Test->stop();
          this->p_Task_VNEHC_Test->checkVolSignal3P(1);
          Serial.println(F("DHT11 Test Lib: Port3 Signal not pull-up to 3V3V"));
        }

        dht.begin();

        // tem = dht.readTemperature();
        // humi = dht.readHumidity();

        // this->debugPort->println(F("---- DHT11 Initial Read ----"));
        // this->debugPort->print(F("Temperature: "));
        // this->debugPort->println(String(tem));
        // this->debugPort->print(F("Humidity: "));
        // this->debugPort->println(String(humi));

        setState(DHT11_Test_Lib_ReportResult);
        kDelay(1000);
      break;

    case DHT11_Test_Lib_ReportResult:
    {
        tem = dht.readTemperature();
        humi = dht.readHumidity();

        this->debugPort->println(F("---- DHT11 Initial Read ----"));
        this->debugPort->print(F("Temperature: "));
        this->debugPort->println(String(tem));
        this->debugPort->print(F("Humidity: "));
        this->debugPort->println(String(humi));
        
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        this->debugPort->println();
        

        // kDelay(0);

      setState(DHT11_Test_Lib_Setup);
      break;
    }

    case DHT11_Test_Lib_Test_Stop:
      break;


    default:
      break;
  }
  
}

END

#endif