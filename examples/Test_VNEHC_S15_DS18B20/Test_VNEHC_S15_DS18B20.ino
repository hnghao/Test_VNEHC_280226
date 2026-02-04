/*
    Y260204 KXN Build xong:
      - Test hở Trở kéo lên OK
      - Test chập nguồn OK
      - Người test để ý giá trị của các cảm biến, nếu có giá trị bất thường là lỗi.

    Y260130 kxn đang build, biên dịch chưa lỗi
*/
#include "DS18B20_Test_Lib.h"

Task_VNEHC_Test Task_VNEHC_Test1;
DS18B20_Test_Lib DS18B20_Test_Lib1;

void setup() {
  // // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("Start test S14 DS18B20"));
  Serial.println(F("De y nhiet do & do am giua cac cam bien neu lech nhau nhieu la loi"));
  Serial.println();
  
  Task_VNEHC_Test1.setup();

  if(Task_VNEHC_Test1.isPullUp_Port3_OK(1) != VNEHC_List_Error_None) 
  {
    Task_VNEHC_Test1.showInfoWithErrorCode(VNEHC_List_Error_PORT3_R_PULLUP_FAIL);
    // while(1);
    // Task_VNEHC_Test1.stop();
  }
  

  // Set mA MAX 
  if(Task_VNEHC_Test1.checkCurrent_mA(20) != VNEHC_List_Error_None)
  {
    Serial.println(Task_VNEHC_Test1.getPort3_mA());
    Serial.println(F("May be ERROR!!!"));
  }

  // DS18B20_Test_Lib1.setup(&Task_VNEHC_Test1, &Serial, PIN_PORT4_RX, 9);
  DS18B20_Test_Lib1.setup(&Task_VNEHC_Test1, &Serial);
  DS18B20_Test_Lib1.setEnDebug(1);
  DS18B20_Test_Lib1.start();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  kxnTaskManager.run(millis());
}
