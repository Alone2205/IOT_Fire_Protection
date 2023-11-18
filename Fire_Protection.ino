#include <WiFi.h> //ทำการ Import Liberry เข้ามาเพื่อใช้ในการเชื่อม WiFi  
#include <TridentTD_LineNotify.h>//ทำการ Import Liberry ที่เชื่อมต่อหรือส่งการแจ้งเตือนผ่าน LINE Notify  
#include <HTTPClient.h> //ทำการ Import Liberry ที่เป็นการส่งข้อมูลไปยัง GoogleSheet ผ่าน HTTP
#define WIFI_STA_NAME "Yam" //กำหนด ID WiFi
#define WIFI_STA_PASS "96991617" // กำหนด PASSWORD WiFi
#define LINE_TOKEN "AP4zO4QmTiJUN4iAW2LIkSIBriUVm270Ne9NZPpmZDW" // กำหนด TOKEN เพื่อแชื่อมกับแชท Line ที่จะทำการแจ้งเตือน

String urlSensor = "https://script.google.com/macros/s/AKfycbzczHun4oPuokXJdSBJQ4d9Mmf0iKiUjVdUnWUsLkHrREFCkwkEjSzkS26tsNPuLw1R_g/exec"; 
//กำหนด Url เพื่อสำหรับการเก็บข้อมูลไปยัง Google Sheet ผ่าน Appsheet



#define BLYNK_TEMPLATE_ID "TMPL6wc4xf86B" //กำหนด Blynk ID
#define BLYNK_TEMPLATE_NAME "Tmp" //กำหนด Blank Name



#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG


#include "DHT.h" 
#define DHTPIN 4     //กำหนดให้ Sensor DHT22 อยู่ที่ ขา 4
#define DHTTYPE DHT22  //กำหนดว่า DHT เป็น DHT22 

DHT dht(DHTPIN, DHTTYPE); 
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);
int temperature ;//กำหนดตัวแปร temperature
int humidity ; // กำหนดตัวแปร humidity


#define gas  34 //กำหนด Sensor Gas อยู่ที่ ขา 34
int Smoke_pm; //กำหนดตัวแปร Smoke_pm
int ppm ;

#define LEDR 5 //กำหนด LED  อยู่ที่ ขา 5
#define SW 35 //กำหนด Button อยู่ที่ ขา 35
#define BUZ 19 //กำหนด  Buzzer อยู่ที่ ขา 34
int  Button; // กำหนดตัวแปร Button

int statusButton; // กำหนดตัวแปร statusButton
int swState ; // กำหนดตัวแปร swState
#include "BlynkEdgent.h" //ทำการ Import Liberry Blynk 

int i=0, j=0, k=0; //กำหนดตัวแปรต่างที่

#define Pump 21 //กำหนด ปั๊มน้ำ  อยู่ที่ ขา 18
int PumpWater;


void setup()
{
  wifi_connected(); //เป็นการ Function ในการเชื่อต่อ WiFi 
  Serial.begin(115200); //กำหนดอัตรา 115,200 สำหรับการสื่อสารซีเรียล
  BlynkEdgent.begin(); //ใช้ในการเริ่มต้นการใช้งาน Blynk Edgent library
  dht.begin();// ใช้ในการเริ่มต้นการใช้งาน dht library


  pinMode(gas, INPUT); //กำหนด pinMode gas เป็น INPUT
  pinMode(LEDR, OUTPUT);//กำหนด pinMode LEDR เป็น OUTPUT
  pinMode(SW,INPUT);//กำหนด pinMode SW เป็น INPUT
  pinMode(BUZ, OUTPUT);//กำหนด pinMode BUZ เป็น OUTPUT
  pinMode(Pump, OUTPUT);//กำหนด pinMode Pump เป็น OUTPUT

}

void loop() {
  h = dht.readHumidity(); // ให้ตัวแปร h อ่านค่าHumidity 
  t = dht.readTemperature(); // ให้ตัวแปร t อ่านค่า temperature
  SenserTemp(); //เป็นการใช้ Function SensorTemp มาใช้
  Smoke(); //เป็นการใช้ Function Smoke มาใช้

 if(Button == 1){ // ถ้าค่า Button == 1 ให้ทำงานในเงื่อนไข
     LINE.notify("Smoke = " + String(Smoke_pm)); //ทำการแจ้งเตือน ข้อความ และค่าควัน ไปยัง Line
     LINE.notify("Humidity = " + String(h)); //ทำการแจ้งเตือน ข้อความ และค่า Humidity ไปยัง Line
     LINE.notify("Temperature = " + String(t)); //ทำการแจ้งเตือน ข้อความ และค่า Temperatureไปยัง Line
    
   if(Smoke_pm > 250){ //แล้วในเมื่อค่าควัน มีค่ามากว่า 150 จะทำในเงื่อนไข
     LINE.notifySticker(4, 264);//ทำการแจ้งเตือนสติ๊ก
    while(Smoke_pm > 250){ //และในเมื่อควัน มีค่ามากว่า 150  จะทำในเงื่อนไข While จนกว่า ค่าควัน จะลดลง
       SenserTemp(); //เป็นการใช้ Function Sensortemp มาใช้
       Smoke(); //เป็นการใช้ Function Smoke มาใช้
       //digitalWrite(BUZ, LOW); //ให้ BUZ เป็นค่า LOW เพราะว่าค่า LOW คือเปิด และ HIGH คือปิด เป็นแค่เฉพาะกับ Buzzder
       digitalWrite(LEDR, HIGH);//ให้LEDR เป็นค่า HIGH
       digitalWrite(Pump,HIGH);//ให้ Pump เป็นค่า HIGH

       Blynk.virtualWrite(V4, HIGH); //ให้ Datastream V4 เป็น HIGH
       Blynk.virtualWrite(V3,"   Warnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
       Blynk.virtualWrite(V5,"    !!Alert!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
       if(Smoke_pm > 550){//แล้วในเมื่อค่าควัน มีค่ามากว่า 200 จะทำในเงื่อนไข
         LINE.notify("Smoke = " + String(Smoke_pm)); //ทำการแจ้งเตือน ข้อความ และค่าควัน ไปยัง Line
         LINE.notify("Humidity = " + String(h)); //ทำการแจ้งเตือน ข้อความ และค่า Humidity ไปยัง Line
         LINE.notify("Temperature = " + String(t));//ทำการแจ้งเตือน ข้อความ และค่า Temperatureไปยัง Line
         LINE.notifySticker(4, 274); //ทำการแจ้งเตือนสติ๊ก

         while(Smoke_pm > 550){//และในเมื่อควัน มีค่ามากว่า 200  จะทำในเงื่อนไข While จนกว่า ค่าควัน จะลดลง
            SenserTemp(); //เป็นการใช้ Function Sensortemp มาใช้
            Smoke();//เป็นการใช้ Function Smoke มาใช้ 
            digitalWrite(BUZ, LOW);  //ให้ BUZ เป็นค่า LOW
            digitalWrite(LEDR, HIGH); //ให้LEDR เป็นค่า HIGH
            digitalWrite(Pump, HIGH); //ให้ Pump เป็นค่า HIGH

            Blynk.virtualWrite(V4, HIGH);//ให้ Datastream V4 เป็น HIGH 
            Blynk.virtualWrite(V3,"   Burnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
            Blynk.virtualWrite(V5,"     !!!Run!!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
         }
   
     }

   }

   }
       digitalWrite(Pump,LOW);  //ให้ Pump เป็นค่า LOW
   
  }else if( swState == HIGH ){ // ถ้า SW == 1 ให้ทำงานในเงื่อนไข
    LINE.notify("Smoke = " + String(Smoke_pm)); //ทำการแจ้งเตือน ข้อความ และค่าควัน ไปยัง Line
    LINE.notify("Humidity = " + String(h)); //ทำการแจ้งเตือน ข้อความ และค่า Humidity ไปยัง Line
    LINE.notify("Temperature = " + String(t)); //ทำการแจ้งเตือน ข้อความ และค่า Temperatureไปยัง Line
   if(Smoke_pm > 450){ //แล้วในเมื่อค่าควัน มีค่ามากว่า 150 จะทำในเงื่อนไข
    LINE.notifySticker(4, 264);//ทำการแจ้งเตือนสติ๊ก
   while(Smoke_pm > 450){ //และในเมื่อควัน มีค่ามากว่า 150  จะทำในเงื่อนไข While จนกว่า ค่าควัน จะลดลง
      SenserTemp();//เป็นการใช้ Function Sensortemp มาใช้
      Smoke();//เป็นการใช้ Function Smoke มาใช้
      digitalWrite(BUZ, LOW);//ให้ BUZ เป็นค่า LOW
      digitalWrite(LEDR, HIGH);//ให้LEDR เป็นค่า HIGH
      digitalWrite(Pump, HIGH);//ให้ Pump เป็นค่า HIGH

      Blynk.virtualWrite(V4, HIGH);//ให้ Datastream V4 เป็น HIGH 
      Blynk.virtualWrite(V3,"     Warnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
      Blynk.virtualWrite(V5,"    !!Alert!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
      if(Smoke_pm > 550){//แล้วในเมื่อค่าควัน มีค่ามากว่า 150 จะทำในเงื่อนไข
        LINE.notify("Smoke = " + String(Smoke_pm));//ทำการแจ้งเตือน ข้อความ และค่าควัน ไปยัง Line
        LINE.notify("Humidity = " + String(h));//ทำการแจ้งเตือน ข้อความ และค่า Humidity ไปยัง Line
        LINE.notify("Temperature = " + String(t)); //ทำการแจ้งเตือน ข้อความ และค่า Temperatureไปยัง Line
        LINE.notifySticker(4, 274);//ทำการแจ้งเตือนสติ๊ก

        while(Smoke_pm >550){//และในเมื่อควัน มีค่ามากว่า 200  จะทำในเงื่อนไข While จนกว่า ค่าควัน จะลดลง
           SenserTemp(); //เป็นการใช้ Function Sensortemp มาใช้    
           Smoke();//เป็นการใช้ Function Smoke มาใช้
           digitalWrite(BUZ, LOW); //ให้ BUZ เป็นค่า LOW
           digitalWrite(LEDR, HIGH);//ให้LEDR เป็นค่า HIGH
           digitalWrite(Pump, HIGH);//ให้ Pump เป็นค่า HIGH

           Blynk.virtualWrite(V4, HIGH);//ให้ Datastream V4 เป็น HIGH 
           Blynk.virtualWrite(V3,"   Burnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
           Blynk.virtualWrite(V5,"    !!!Run!!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
        }
    }
   }


   }
       digitalWrite(Pump, LOW); //ให้ Pump เป็นค่า LOW

  }if(Smoke_pm > 650){ //แล้วในเมื่อค่าควัน มีค่ามากว่า 150 จะทำในเงื่อนไข

    digitalWrite(BUZ, LOW); //ให้ BUZ เป็นค่า LOW
    digitalWrite(LEDR, HIGH);  //ให้ LEDR เป็นค่า HIGH
    digitalWrite(Pump, HIGH);  //ให้ Pump เป็นค่า HIGH
    int i = 0;

   if( i % 20 == 0){
      LINE.notify("Smoke = " + String(Smoke_pm));//ทำการแจ้งเตือน ข้อความ และค่าควัน ไปยัง Line
      LINE.notify("Humidity = " + String(h));//ทำการแจ้งเตือน ข้อความ และค่า Humidity ไปยัง Line
      LINE.notify("Temperature = " + String(t));//ทำการแจ้งเตือน ข้อความ และค่า Temperatureไปยัง Line
    }
    i++;
    Blynk.virtualWrite(V4, HIGH);//ให้ Datastream V4 เป็น HIGH 
    Blynk.virtualWrite(V3,"   Warnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
    Blynk.virtualWrite(V5,"    !!Alert!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
    LINE.notifySticker(4, 264);//ทำการแจ้งเตือนสติ๊ก
  }
  if(Smoke_pm > 750 ){//แล้วในเมื่อค่าควัน มีค่ามากว่า 200 จะทำในเงื่อนไข
  
    LINE.notifySticker(4, 274);//ทำการแจ้งเตือนสติ๊ก
    digitalWrite(BUZ, LOW); //ให้ BUZ เป็นค่า LOW
    digitalWrite(LEDR, HIGH); //ให้ LEDR เป็นค่า HIGH
    digitalWrite(Pump, HIGH); //ให้ Pump เป็นค่า HIGH

    Blynk.virtualWrite(V4, HIGH); //ให้ Datastream V4 เป็น HIGH
    Blynk.virtualWrite(V3,"   Burnning!!!");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
    Blynk.virtualWrite(V5,"     !!!Run!!!");//ให้ Datastream V5 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
        }
    digitalWrite(Pump, LOW); //ให้ Pump เป็นค่า HIGH
    digitalWrite(LEDR, LOW); //ให้ Pump เป็นค่า HIGH
    digitalWrite(BUZ, LOW); //ให้ BUZ เป็นค่า LOW

    Blynk.virtualWrite(V4, LOW);//ให้ Datastream V4 เป็น LOW
    Blynk.virtualWrite(V3,"    Peaceful");//ให้ Datastream V3 เป็น เป็นข้อความดังกล่าวที่ใช้กับ LCD ในBlynk
    Blynk.virtualWrite(V5," ");//ให้ Datastream V5 เป็น เป็นค่าว่าง

    BlynkEdgent.run();

    
    SendButtonTOSheet(); //ดึง Function SendButtonTOSheet มาใช้
    SendTempTOSheet(); //ดึง Function SendTempTOSheet มาใช้
    SendGasTOSheet(); //ดึง Function SendGasTOSheet มาใช้
    SendPumpWaterTOSheet(); //ดึง Function SendPumpWaterTOSheet มาใช้
    delay(2000);

}

void SenserTemp(){ //เป็น Function ของ Sensor DHT 


  if (isnan(h) || isnan(t)) { //ถ้าค่าไม่มีค่า 

    Serial.println("Failed to read from DHT sensor!"); //จะแสดงข้อมความดังกล่าว 
    return; //ทำงาน Return

  }
  
  Serial.print("Humidity: "); //จะแสดงข้อมความดังกล่าว 
  Serial.print(h); //จะแสดงค่า h
  Serial.print(" %\t"); 
  Serial.print("\nTemperature: ");//จะแสดงข้อมความดังกล่าว 
  Serial.print(t); //จะแสดงค่า t
  Serial.println(" *C "); //จะแสดงข้อมความดังกล่าว 
  Blynk.virtualWrite(V1, h); //ให้ Datastream V1 มาค่าเท่ากับ ค่าของตัวแปร h
  Blynk.virtualWrite(V2, t);//ให้ Datastream V2 มาค่าเท่ากับ ค่าของตัวแปร t
  delay(1000);//ให้ มีDelay 1วิ

}

void Smoke(){ //เป็น Function ของ Sensor Gas

    Smoke_pm = analogRead(gas); //ให้ ตัวแปร Smoke_pm อ่านค่าแบบ analog จาก gas
    Blynk.virtualWrite(V9,Smoke_pm);  //ให้ Datastream V1 มาค่าเท่ากับ ค่าของตัวแปร Smoke_pm
    Serial.print("\nSmoke_Pm = "); //จะแสดงข้อมความดังกล่าว 
    Serial.println(Smoke_pm);// พร้อมกับค่า Smoke_pm

}

void wifi_connected(){ //เป็น Function ในการเชื่อม WiFi

  WiFi.mode(WIFI_STA); //
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);// เริ่มต้นการเชื่อมต่อ Wi-Fi
  Serial.println(LINE.getVersion()); //เรียกฟังก์ชัน getVersion() ของไลบรารี LINE
  Serial.println("WiFi connected"); //แสดงข้อความดังกล่าว
  Serial.print("IP address: "); //แสดงข้อความดังกล่าว

  while (WiFi.status() != WL_CONNECTED) { // รอการเชื่อมต่อ Wi-Fi
  
    delay(500); // ให้ Delay 0.5 วิ
    Serial.print("."); //แสดงข้อความดังกล่าว
    LINE.setToken(LINE_TOKEN); //ทำการSetToken ที่เรากำหนดไว้

  }
}

BLYNK_WRITE(V0){ //เป็น Method ที่เอาไว้รับค่าจาการกดปุ่ม จาก แอปพิเคชั่น Blynk

  Button = param.asInt(); //ให้ตัวแปร Button เก็บค่า param จาก Datastream V0

}
void SendTempTOSheet(){// เป็น Fonction SendSTempTOSheet
  temperature = t; //ให้ตัวแปร temperature  เท่ากับค่า t
  humidity = h; //ให้ตัวแปร humidity เท่ากับค่า h


  j++; //ให้ค่า j+1
  if(j==60){ //ถ้า j == 60ให้ทำในเงื่อนไข
    String urlSensors = urlSensor + "?temp=" + temperature + "&humidity=" + humidity; //ให้ตัวแปรhttpCodetemp และตัวแปร humidity ส่งคำร้องข้อมูล GET

    HTTPClient http; //ใช้เพื่อส่งคำร้องข้อมูล HTTP ไปยัง URL ของเว็บเซิร์ฟเวอร์
    http.begin(urlSensors.c_str()); //ใช้สำหรับเริ่มต้นคำร้องข้อมูล HTTP
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); //ใช้ในการกำหนดว่าคำร้องข้อมูล HTTP ควรทำการติดตามการเปลี่ยนเส้นทาง (redirects) อย่างไร เซ็ตอัตโนมัติ
    int httpCodetemp = http.GET(); //ให้ตัวแปรhttpCodetemp ส่งคำร้องข้อมูล GET


    if (httpCodetemp == 200 || httpCodetemp == 201) { //ถ้าhttpCodetemp มีค่า 200 หรือ 201 แสดงว่าส่งค่าเข้าชีทได้
      Serial.println("Temp Ok"); //ให้แสดงขึ้นmonitor ว่า Temp ok
    } else { //แต่ถ้าไม่ใช่
      Serial.println("TempFail: " + String(httpCodetemp)); //ให้บอกว่า TempFail: ที่ค่าhttpCodetempเท่าไหร่
    }
    j=0; //เมื่อเข้าเงื่อนไขแล้ว ก่อนออกให้เซ็ตค่า j เป็น 0
  } 
  }

void SendGasTOSheet(){ // เป็น Fonction SendGasTOSheet
  ppm = Smoke_pm; //ให้ตัวแปร ppm เท่ากับค่า Smoke_pm
  k++; //k+1
  if(k==60){ //ถ้าค่าk=60แสดงว่าวนครบ60รอบ หรือ1นาทีแล้ว ให้เข้าเงื่อนไข
     String urlSensors = urlSensor + "?ppm=" + ppm; //ประกาศตัวแปรurlSensors เพื่อรับค่าppm ไปเก็บที่urlSensor

    HTTPClient http; //ใช้เพื่อส่งคำร้องข้อมูล HTTP ไปยัง URL ของเว็บเซิร์ฟเวอร์
    http.begin(urlSensors.c_str());  //ใช้สำหรับเริ่มต้นคำร้องข้อมูล HTTP
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); //ใช้ในการกำหนดว่าคำร้องข้อมูล HTTP ควรทำการติดตามการเปลี่ยนเส้นทาง (redirects) อย่างไร เซ็ตอัตโนมัติ
    int httpCodeGas = http.GET(); //ให้ตัวแปรhttpCodeGas ส่งคำร้องข้อมูล GET


    if (httpCodeGas == 200 || httpCodeGas == 201) { //ถ้าhttpCodeGas มีค่า 200 หรือ 201 แสดงว่าส่งค่าเข้าชีทได้
      Serial.println("Ok Gas"); //ให้แสดงขึ้นmonitor ว่า Ok Gas
    } else { //แต่ถ้าไม่ใช่
      Serial.println("Fail: " + String(httpCodeGas)); //ให้บอกว่า Fail: ที่ค่าhttpCodeGasเท่าไหร่
    }
    k=0; //เมื่อเข้าเงื่อนไขแล้ว ก่อนออกให้เซ็ตค่า k เป็น 0

  }
  }
  


 
  void SendButtonTOSheet(){ // เป็น Fonction SendButtonTOSheet
  statusButton = SW; // ให้ตัวแปร statusButton เท่ากับค่า SW
  k++; //ให้k+1
  if(k==60){ //ถ้าค่าk=60แสดงว่าวนครบ60รอบ หรือ1นาทีแล้ว ให้เข้าเงื่อนไข
    String urlSensors = urlSensor + "?Button=" + statusButton; //ประกาศตัวแปรurlSensors เพื่อรับค่าButton ไปเก็บที่urlSensor

    HTTPClient http; //ใช้เพื่อส่งคำร้องข้อมูล HTTP ไปยัง URL ของเว็บเซิร์ฟเวอร์
    http.begin(urlSensors.c_str());  //ใช้สำหรับเริ่มต้นคำร้องข้อมูล HTTP
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); //ใช้ในการกำหนดว่าคำร้องข้อมูล HTTP ควรทำการติดตามการเปลี่ยนเส้นทาง (redirects) อย่างไร เซ็ตอัตโนมัติ
    int httpCodeBut = http.GET(); //ให้ตัวแปรhttpCodeBut ส่งคำร้องข้อมูล GET

    if (httpCodeBut == 200 || httpCodeBut == 201) { //ถ้าhttpCodeBut มีค่า 200 หรือ 201 แสดงว่าส่งค่าเข้าชีทได้
      Serial.println("Ok But"); //ให้แสดงขึ้นmonitor ว่า Ok But
    } else { //แต่ถ้าไม่ใช่
      Serial.println("Fail: " + String(httpCodeBut)); //ให้บอกว่า Fail: ที่ค่าhttpCodeButเท่าไหร่
    }
    k=0; //เมื่อเข้าเงื่อนไขแล้ว ก่อนออกให้เซ็ตค่า k เป็น 0
  }
}
  void SendPumpWaterTOSheet(){// เป็น Fonction SendPumpWaterTOSheet
  PumpWater = Pump; //ให้ตัวแปร PumpWater เท่ากับค่า Pump
  i++; //ให้ i เพิ่มค่า1ค่า เมื่อมาถึงส่วนนี้
  if(i==60){//ถ้าค่าi=60แสดงว่าวนครบ60รอบ หรือ1นาทีแล้ว ให้เข้าเงื่อนไข
    String urlSensors = urlSensor + "?pump=" +  PumpWater;//ประกาศตัวแปรurlSensors เพื่อรับค่าPumpWater ไปเก็บที่urlSensor

    HTTPClient http;//ใช้เพื่อส่งคำร้องข้อมูล HTTP ไปยัง URL ของเว็บเซิร์ฟเวอร์
    http.begin(urlSensors.c_str());  //ใช้สำหรับเริ่มต้นคำร้องข้อมูล HTTP
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);//ใช้ในการกำหนดว่าคำร้องข้อมูล HTTP ควรทำการติดตามการเปลี่ยนเส้นทาง (redirects) อย่างไร เซ็ตอัตโนมัติ
    int httpCodepump = http.GET();//ให้ตัวแปรhttpCodepump ส่งคำร้องข้อมูล GET

    if (httpCodepump == 200 || httpCodepump == 201) { //ถ้าhttpCodepump มีค่า 200 หรือ 201 แสดงว่าส่งค่าเข้าชีทได้
      Serial.println("Ok But"); //ให้แสดงขึ้นmonitor ว่า Ok But
    } else {//แต่ถ้าไม่ใช่
      Serial.println("Fail: " + String(httpCodepump));//ให้บอกว่า Fail: ที่ค่าhttpCodepumpเท่าไหร่
    }
    i=0;//เมื่อเข้าเงื่อนไขแล้ว ก่อนออกให้เซ็ตค่า i เป็น 0
  }
}


