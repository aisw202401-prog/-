#include <Adafruit_NeoPixel.h>
#include <BluetoothSerial.h>

// ESP32 내장 블루투스 시리얼 객체
BluetoothSerial SerialBT;

// 핀 번호 설정 (기존 실습 코드 반영)
const int PWMA = 19;
const int PWMB = 18;
const int AIN1 = 33;
const int AIN2 = 32;
const int BIN1 = 25;
const int BIN2 = 26;
const int STBY = 5;
const int LED_PIN = 27;
const int LED_COUNT = 2; // 네오픽셀 개수
const int motorSpeed = 90;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int pins[] = {PWMA, PWMB, AIN1, AIN2, BIN1, BIN2, STBY};

// 모터 제어 기본 함수
void motor(int leftSpeed, int rightSpeed, int a1, int a2, int b1, int b2){
  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
  digitalWrite(AIN1, a1);
  digitalWrite(AIN2, a2);
  digitalWrite(BIN1, b1);
  digitalWrite(BIN2, b2);
}

// 이동 함수 정의
void forward(){
  motor(motorSpeed, motorSpeed, LOW, HIGH, HIGH, LOW);
}

void backward(){
  motor(motorSpeed, motorSpeed, HIGH, LOW, LOW, HIGH);
}

void left(){
  motor(motorSpeed, motorSpeed, LOW, HIGH, LOW, HIGH);
}

void right(){
  motor(motorSpeed, motorSpeed, HIGH, LOW, HIGH, LOW);
}

void stopMotor(){
  motor(0, 0, LOW, LOW, LOW, LOW);
}

// 네오픽셀 색상 제어 함수
void setLEDs(uint32_t color0, uint32_t color1) {
  strip.setPixelColor(0, color0);
  strip.setPixelColor(1, color1);
  strip.show();
}

void setup(){
  // 핀 모드 설정 및 STBY 활성화
  for(int i = 0; i < 7; i++) pinMode(pins[i], OUTPUT);
  digitalWrite(STBY, HIGH);

  // 네오픽셀 초기화
  strip.begin();
  strip.show();

  // 블루투스 시작 (앱인벤터에서 검색될 이름 설정)
  SerialBT.begin("ESP32-johan"); 
}

void loop(){
  // 앱인벤터로부터 블루투스 데이터가 들어왔는지 확인
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    // 색상 정의
    uint32_t WHITE  = strip.Color(255, 255, 255); // 전조등
    uint32_t RED    = strip.Color(255, 0, 0);     // 후미등
    uint32_t YELLOW = strip.Color(255, 150, 0);   // 방향지시등
    uint32_t OFF    = strip.Color(0, 0, 0);       // 소등

    // 명령에 따른 모터 및 네오픽셀 동시 제어
    switch (cmd) {
      case 'F': // 전진
        forward();
        setLEDs(WHITE, WHITE);
        break;

      case 'B': // 후진
        backward();
        setLEDs(RED, RED);
        break;

      case 'L': // 좌회전
        left();
        setLEDs(YELLOW, OFF);
        break;

      case 'R': // 우회전
        right();
        setLEDs(OFF, YELLOW);
        break;

      case 'S': // 정지
        stopMotor();
        setLEDs(OFF, OFF);
        break;
    }
  }
}