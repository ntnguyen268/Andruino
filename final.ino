#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
//Defition DHT Sensor
#define DHTTYPE DHT11   
#define DHTPin  10
DHT dht(DHTPin, DHTTYPE);
BH1750 lightMeter;
#define pin_analog A0
#define pin_digital D0
bool mannual = 0;
float t = 0.0;
float h = 0.0;
int Dryness = analogRead(pin_analog);
uint16_t lux = lightMeter.readLightLevel();
const int pin_relay = 14;
int lcdColumns = 16;
int lcdRows = 2;
//Definiton for LCD I2C
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
// Set number of relays
#define NUM_RELAYS  1
// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = { 14 };
// define for wifi sever 
      //const char* ssid = "nguyen";
     //const char* password = "123456789";
 const char* ssid = "P 1022";
 const char* password = "matkhau1022";
const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";
// set username and password
const char* http_username = "gacontrolai";
const char* http_password = "123";
AsyncWebServer server(80);
unsigned long previousMillis = 0;
const long interval = 3000;
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="utf-8">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    @font-face {
    font-family: "BeautyMountainsPersonalUse";
    src: url(BeautyMountains.ttf);
    }
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    h3 { font-size: 2.0rem; }
    p {
    font-size: 3.0rem;
    margin-bottom: 1.0em;
    margin-top:0px;
    }
    .units { 
        font-size: 1.2rem; 

    }
    .dht-labels{
      font-size: 2.0rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    body { margin:0px auto; padding-bottom: 25px; background-image: url("https://i.ibb.co/0Kpc7N1/Sunrise-above-clouds-from-airplane-window.jpg"); background-repeat: no-repeat; background-attachment: fixed; background-size: cover;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #32CD32}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  
  .head {
    margin-top:20px;
  }

  .head h2 {
    color: skyblue;
    -webkit-text-stroke-width: 2px;
    -webkit-text-stroke-color: black;
    font-family: 'BeautyMountainsPersonalUse'; 
    margin-bottom:0px;
    font-size: 80px;
    margin-top: auto;
    text-align: center;
  }
  
  .sidenav {
  height: 100%;
  position: fixed;
  z-index: 1;
  top: 0;
  left: 0;
  background-color: #111;
  overflow-x: hidden;
  transition: 0.5s;
  padding-top: 40px;
}

.sidenav a {
  padding: 10px 10px 8px 20px;
  text-decoration: none;
  font-size: 20px;
  color: #818181;
  display: block;
  transition: 0.3s;
  text-align:left;
}

.sidenav a:hover {
  color: #f1f1f1;
}

.sidenav .closebtn {
  position: absolute;
  top: -10px;
  right: 0px;
  font-size: 35px;  
  margin-left: 50px;
}
.sidenav .

.data{
  padding-top: 0px;
}
.measure{
  text-align:left;
  display: inline-block;  
}
#hinhThay{
  transition:1s;  
}

  </style>
  <script>
    function openNav() {
    if(document.getElementById("mySidenav").style.width == "0px")
      document.getElementById("mySidenav").style.width = "270px";
    else 
      document.getElementById("mySidenav").style.width = "0px";
  }

  function closeNav() {
      document.getElementById("mySidenav").style.width = "0";
  }
  function hienHinh(){
    var  hinh = document.getElementById("hinhThay");
    var linked = document.getElementById("button")
    if (hinh.style.height == "0px") {
        hinh.style.height = "150px";
      } 
    else {
        hinh.style.height = "0px";
      }
  }
  </script>
</head>
<body>
  <div></div>
    <div id="mySidenav" class="sidenav" style="width: 0px;">
      <span style="font-size:25px;cursor:pointer; position:fixed;top: 0px;left:10px;color:#C00; margin-bottom:0px;" onclick="openNav()">&#9776;<b> Group Members </b></span>
      <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
      <a href="#">1. Dương Phi Long</a>
      <a href="#">2. Đinh Hoàng Long</a>
      <a href="#">3. Lưu Nguyễn</a>
      <a href="#">4. Nguyễn Minh Phúc</a>
      <a href="#">5. Nguyễn Thành Nguyên</a>
      <a href="#">6. Nguyễn Duy Tịnh</a>
      <div style="position: absolute;">
          <span href="#" style="text-align:left;color:#6F6;cursor:pointer;padding: 10px 10px 8px 20px;text-decoration: none;font-size: 20px;display: block;transition: 0.3s" onClick="hienHinh()"><strong>Instructor:</strong><br><strong>Phạm Quốc Cường</strong></span>
          <img id="hinhThay" style="height:0px;" src="https://sites.google.com/site/cuong2803vn/_/rsrc/1511612699646/home/img.jpg" alt="thay cuong">
          <button style="background-color: black; border-color: none; border: none;"><a href="https://sites.google.com/site/cuong2803/"> Click here for more information </a></button>
      </div>
    </div>
  <div class="head">
       <h2> Watering control system </h2>
    </div>
    
    <div class="data"> 
      <h3> Auto/Mannual </h3>
        <label class="switch">
        <input type="checkbox" onchange="toggleManual(this)">
        <span class="slider"></span>
        </label>
        <h3>Turn on/off watering</h3>
        <p>%BUTTONPLACEHOLDER%</p>
     
<script>
  function toggleCheckbox(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
    else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
    xhr.send();
  }
</script>
<script>
function toggleManual(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/updateManual?state=1", true); }
    else { xhr.open("GET", "/updateManual?state=0", true); }
    xhr.send();
  }
</script>
    <div class="measure">
    <table>
    <tr>
        <td>
            <p>
                <i class="fas fa-water" style="color:#8a4b08cc;"></i> 
                <span class="dht-labels"> Dryness:</span>
            </p>
        </td>
        <td>
            <p>
              <span id="dryness">%DRYNESS%</span>
            </p>
        </td>
    </tr>
     <tr>
        <td>
          <p>
            <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
            <span class="dht-labels">Temperature:</span>
          </p>
        </td> 
        <td>
          <p>
            <span id="temperature">%TEMPERATURE%</span>
            <sup class="units">&deg;C</sup>
          </p>
        </td>
     </tr>
     <tr>
      <td>
          <p>
                <i class="fas fa-tint" style="color:#00add6;"></i> 
                <span class="dht-labels">Humidity:</span>
          </p>
        </td>
        <td>
          <p>
                <span id="humidity">%HUMIDITY%</span>
                <sup class="units">%</sup> 
          </p>
        </td>
     </tr>
     <tr>
      <td>
          <p>
                <i class="fas fa-lightbulb" style="color:#d7dc0f;"></i> 
                <span class="dht-labels">Luminous intensity:</span>
            </p>
        </td>
        <td>
            <p>
                <span id="light">%LIGHT%</span>
                <sup class="units">lux</sup>
            </p>
        </td>
      </tr>
      </table>
      </div>
  </div>
</body>
<script>

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 3000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 3000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("dryness").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/dryness", true);
  xhttp.send();
}, 3000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("light").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/light", true);
  xhttp.send();
}, 3000 ) ;


</script>
</html>)rawliteral";
String relayState(int numRelay) {
    if (RELAY_NO) {
        if (digitalRead(relayGPIOs[numRelay - 1])) {
            return "";
        }
        else {
            return "checked";
        }
    }
    else {
        if (digitalRead(relayGPIOs[numRelay - 1])) {
            return "checked";
        }
        else {
            return "";
        }
    }
    return "";
}
// Replaces placeholder with DHT values
String processor(const String& var) {
    if (var == "TEMPERATURE") {
        return String(t);
    }
    else if (var == "HUMIDITY") {
        return String(h);
    }
    else if (var == "DRYNESS")
    {
        return String(Dryness);
    }
    else if (var == "LIGHT")
    {
        return String(lux);
    }
    else if (var == "BUTTONPLACEHOLDER") {
        String buttons = "";
        for (int i = 1; i <= NUM_RELAYS; i++) {
            String relayStateValue = relayState(i);
            buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + "><span class=\"slider\"></span></label>";
        }
        return buttons;
    }
    return String();
}
void setup()
{
    lcd.init();       // start the LCD              
    lcd.backlight(); // start the LCD
    lightMeter.begin();
    pinMode(pin_analog, INPUT);
    pinMode(pin_digital, INPUT);
    pinMode(pin_relay, OUTPUT);
    pinMode(DHTPin, INPUT);
    Serial.begin(115200);
    dht.begin();// start the DHT sensor
    digitalWrite(pin_relay, HIGH);
    for (int i = 1; i <= NUM_RELAYS; i++)
    {
        pinMode(relayGPIOs[i - 1], OUTPUT);
        if (RELAY_NO) {
            digitalWrite(relayGPIOs[i - 1], HIGH);
        }
        else {
            digitalWrite(relayGPIOs[i - 1], LOW);
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Hardware Project");
    lcd.setCursor(0, 3);
    lcd.print("Welcome");
    delay(500);
    Serial.println("Start Connecting: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count <= 10) {
        delay(1000);
        Serial.print(".");
        count++;
    }
    Serial.println("");
    Serial.println("WiFi Connected !");
    if (WiFi.status() == WL_CONNECTED) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi Connected !");
        lcd.setCursor(0, 3);
        lcd.print("Welcome");
        Serial.println("Web IP is: ");
        Serial.println(WiFi.localIP());
    }
    else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Not connected!");
    }
    delay(1000);
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        if(!request->authenticate(http_username,http_password))
          return request->requestAuthentication();
        request->send_P(200, "text/html", index_html, processor);
        });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(t).c_str());
        });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(h).c_str());
        });
    server.on("/dryness", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(Dryness).c_str());
        });
    server.on("/light", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(lux).c_str());
        });
    server.on("/updateManual", HTTP_GET, [](AsyncWebServerRequest* request) { 
        String inputMessage2;
        String inputParam2;
        // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
        if (request->hasParam(PARAM_INPUT_2)) {
            inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
            inputParam2 = PARAM_INPUT_2;
            if (inputMessage2.toInt() == 1)
            {
                Serial.println("Currently using mannual");
                mannual = 1;
            }
            else {
              Serial.println("Currently using auto");
              mannual = 0;
            }
        }
    });
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
        String inputMessage;
        String inputParam;
        String inputMessage2;
        String inputParam2;
        // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
        if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2) & mannual == 1) {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
            inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
            inputParam2 = PARAM_INPUT_2;
            if (RELAY_NO) {
                Serial.print("NO ");
                digitalWrite(relayGPIOs[inputMessage.toInt() - 1], !inputMessage2.toInt());
            }
            else {
                Serial.print("NC ");
                digitalWrite(relayGPIOs[inputMessage.toInt() - 1], inputMessage2.toInt());
            }
        }
        else {
            inputMessage = "No message sent";
            inputParam = "none";
        }
        Serial.println(inputMessage + inputMessage2);
        request->send(200, "text/plain", "OK");
        });
    // Start server
    server.begin();
    Serial.println("HTTP server started");
    lcd.clear();
}
void loop()
{
    lux = lightMeter.readLightLevel();
    Dryness = analogRead(pin_analog);
    bool checkDry = 0;
    bool checkWet = 0;
    unsigned long currentMillis = millis();
    float Temperature = dht.readTemperature();
    float Humidity = dht.readHumidity();
    if (currentMillis - previousMillis >= interval) {
        // save the last time you updated the DHT values
        previousMillis = currentMillis;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Dryness: ");
        lcd.setCursor(11, 0);
        lcd.print(Dryness);
        if (mannual == 0)
        {
            if (Dryness > 900)    // Đất khô
            {
                if (!checkDry)
                {
                    lcd.setCursor(0, 1);
                    lcd.print("Dry -> Watering");
                }
                digitalWrite(pin_relay, LOW);
                // kích hoạt rờ le
                checkDry = 1;
                checkWet = 0;
            }
            else // Đất đủ dộ ẩm
            {
                if (!checkWet)
                {
                    lcd.setCursor(0, 1);
                    lcd.print(" Stop watering ");
                }
                digitalWrite(pin_relay, HIGH);  // tắt rờ le
                checkWet = 1;
                checkDry = 0;
            }
        }
        else {
            digitalWrite(pin_relay, LOW);
            lcd.setCursor(0, 1);
            lcd.print(" Force watering ");
        }
        delay(1000);
        if (isnan(Temperature) || isnan(Humidity))
        {
            lcd.setCursor(0, 0);
            lcd.print(" DHT sensor err ");
            Dryness = analogRead(pin_analog);
            if (mannual == 0) {
                if (Dryness > 900) {// Đất khô
                    if (!checkDry) {
                        lcd.setCursor(0, 1);
                        lcd.print("Dry -> Watering");
                    }
                    digitalWrite(pin_relay, LOW); // kích hoạt rờ le
                    checkDry = 1;
                    checkWet = 0;
                }
                else { // Đất đủ dộ ẩm
                    if (!checkWet) {
                        lcd.setCursor(0, 1);
                        lcd.print(" Stop watering ");
                    }
                    digitalWrite(pin_relay, HIGH);  // tắt rờ le
                    checkWet = 1;
                    checkDry = 0;
                }
            }
            else {
                digitalWrite(pin_relay, LOW);
                lcd.setCursor(0, 1);
                lcd.print(" Force watering ");
            }//alway run pump
            delay(1000);
        }
        else
        {
            t = Temperature;
            h = Humidity;
            Serial.print("Temp:  ");
            Serial.print(t);
            Serial.print("     ");
            Serial.print("Humid:  ");
            Serial.print(h);
            Serial.println();
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print(" Temp:");
            lcd.setCursor(9, 0);
            lcd.print(t);
            Dryness = analogRead(pin_analog);
            if (mannual == 0)
            {
                if (Dryness > 900)    // Đất khô
                {
                    if (!checkDry)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print("Dry -> Watering");
                    }
                    digitalWrite(pin_relay, LOW);
                    // kích hoạt rờ le
                    checkDry = 1;
                    checkWet = 0;
                }
                else // Đất đủ dộ ẩm
                {
                    if (!checkWet)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print(" Stop watering ");
                    }
                    digitalWrite(pin_relay, HIGH);  // tắt rờ le
                    checkWet = 1;
                    checkDry = 0;
                }
            }
            else {
                digitalWrite(pin_relay, LOW);
                lcd.setCursor(0, 1);
                lcd.print(" Force watering ");
            }
            delay(1000);
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print(" Humid:");
            lcd.setCursor(9, 0);
            lcd.print(h);
            Dryness = analogRead(pin_analog);
            if (mannual == 0)
            {
                if (Dryness > 900)    // Đất khô
                {
                    if (!checkDry)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print("Dry -> Watering");
                    }
                    digitalWrite(pin_relay, LOW);
                    // kích hoạt rờ le
                    checkDry = 1;
                    checkWet = 0;
                }
                else // Đất đủ dộ ẩm
                {
                    if (!checkWet)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print(" Stop watering ");
                    }
                    digitalWrite(pin_relay, HIGH);  // tắt rờ le
                    checkWet = 1;
                    checkDry = 0;
                }
            }
            else {
                digitalWrite(pin_relay, LOW);
                lcd.setCursor(0, 1);
                lcd.print(" Force watering ");
            }
            delay(1000);
        }
        if (lux == 65535) {
            lcd.setCursor(0, 0);
            lcd.print("Light sensor err");
            Dryness = analogRead(pin_analog);
            if (mannual == 0) {
                if (Dryness > 900) {// Đất khô
                    if (!checkDry) {
                        lcd.setCursor(0, 1);
                        lcd.print("Dry -> Watering");
                    }
                    digitalWrite(pin_relay, LOW); // kích hoạt rờ le
                    checkDry = 1;
                    checkWet = 0;
                }
                else { // Đất đủ dộ ẩm
                    if (!checkWet) {
                        lcd.setCursor(0, 1);
                        lcd.print(" Stop watering ");
                    }
                    digitalWrite(pin_relay, HIGH);  // tắt rờ le
                    checkWet = 1;
                    checkDry = 0;
                }
            }
            else {
                digitalWrite(pin_relay, LOW);
                lcd.setCursor(0, 1);
                lcd.print(" Force watering ");
            }//alway run pump
        }
        else {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print(" Light:    ");
            lcd.setCursor(9, 0);
            lcd.print(lux);
            Dryness = analogRead(pin_analog);
            if (mannual == 0)
            {
                if (Dryness > 900)    // Đất khô
                {
                    if (!checkDry)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print("Dry -> Watering");
                    }
                    digitalWrite(pin_relay, LOW);
                    // kích hoạt rờ le
                    checkDry = 1;
                    checkWet = 0;
                }
                else // Đất đủ dộ ẩm
                {
                    if (!checkWet)
                    {
                        lcd.setCursor(0, 1);
                        lcd.print(" Stop watering ");
                    }
                    digitalWrite(pin_relay, HIGH);  // tắt rờ le
                    checkWet = 1;
                    checkDry = 0;
                }
            }
            else {
                digitalWrite(pin_relay, LOW);
                lcd.setCursor(0, 1);
                lcd.print(" Force watering ");
            }
        }
        delay(1000);
    }
}
