/*
    Y260130 kxn đang build, biên dịch chưa lỗi
*/
#include "Relay_Test_Lib.h"

// uint8_t pinA_Diod = A1;
// uint8_t pinD_Diod = 7;
// uint8_t pin_Relay = 9;

#define RELAY_ACTIVE  (1)

Task_VNEHC_Test Task_VNEHC_Test1;
Relay_Test_Lib Relay_Test_Lib1;

void setup() {
  // // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("Start test M05 Relay"));
  Task_VNEHC_Test1.setup();
  Task_VNEHC_Test1.addHelp(&help);

  // // check R Pulldown Port 3

  if(Task_VNEHC_Test1.isPullDown_Port3_OK() != VNEHC_List_Error_None) 
  {
    Task_VNEHC_Test1.showInfoWithErrorCode(VNEHC_List_Error_PORT3_R_PULLDOWN_FAIL);
    // while(1);
    Task_VNEHC_Test1.stop();
  }
  

  // Set mA MAX 
  if(Task_VNEHC_Test1.checkCurrent_mA(200) != VNEHC_List_Error_None)
  {
    // Serial.println(Task_VNEHC_Test1.getPort3_mA());
    Serial.println(F("May be ERROR!!!"));
  }

  // Relay_Test_Lib1.setup(&Task_VNEHC_Test1, &Serial, PIN_PORT4_RX, 9);
  Relay_Test_Lib1.setup(&Task_VNEHC_Test1, &Serial);
  Relay_Test_Lib1.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  kxnTaskManager.run(millis());
}

void help()
{
  Serial.print(F("ERROR Code: "));
  Serial.println(VNEHC_List_Error_PORT3_R_PULLDOWN_FAIL);
  Serial.print(F("\t kiem tra ho chan R ngoai cung"));
  Serial.println();
  Serial.println();

  Serial.print(F("ERROR Code: "));
  Serial.println(VNEHC_List_Error_PORT3_CURRENT_LOWER);
  Serial.println(F("\t 1) kiem tra ho chan Tran và R ke ben"));
  Serial.println(F("\t 2) Kiem tra Tran, lay nhip nôi tat chan 2-3 cua tran lai, Relay keu la loi Tran => thay Tran"));
  Serial.println(F("\t \t - Relay keu la loi Tran => thay Tran"));
  Serial.println(F("\t \t - Relay KO keu la loi Relay => Kiem tra chieu Relay hoac thay moi"));
  Serial.println();
  Serial.println();

  Serial.print(F("ERROR Code: "));
  Serial.println(VNEHC_List_Error_PORT3_CURRENT_HIGHER);
  Serial.print(F("\t Ghi nhan lai so luong SP bi loi roi bao cao len cap tren duyet"));
  Serial.println();
  Serial.println();
}