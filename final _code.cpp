#include <UTFT.h>
#include <URTouch.h>
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include <Servo.h> 
Servo myservo;
int blue1 = 72;
int blue2 = 12;
int blue3 = 11;
int blue4 = 81;
int green = 78;
int redPin = 73;
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
UTFT myGLCD(ITDB32S, 82, 83, 84, 85);
URTouch myTouch(48, 49, 50, 51, 52);
FPS_GT511C3 fps(8, 7);
int x, y;
char stCurrent[20] = "";
int stCurrentLen = 0;
char stLast[20] = "";
String str = "";
String pwd = "";
void waitForIt(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1 - 1, y1 - 1, x2 + 1, y2 + 1);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(0, 0, 0);
  myGLCD.drawRoundRect (x1 - 1, y1 - 1, x2 + 1, y2 + 1);
}

void enroll()
{
  // Enroll test

  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid == true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  String b = "Press finger to Enroll #" + enrollid;
  myGLCD.print(b, 24, 120);
  while (fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.Enroll1();
    while (fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while (fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.Enroll2();
      while (fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while (fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          Serial.println("Enrolling Successfull");
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
        }
      }
      else Serial.println("Failed to capture third finger");
    }
    else Serial.println("Failed to capture second finger");
  }
  else Serial.println("Failed to capture first finger");
}
void lightUp(String pin)
{

  if (pin.length() == 0)
  {
    digitalWrite(blue1, LOW);
    digitalWrite(blue2, LOW);
    digitalWrite(blue3, LOW);
    digitalWrite(blue4, LOW);
  }
  if (pin.length() == 1)
  {
    digitalWrite(blue1, HIGH);
    digitalWrite(blue2, LOW);
    digitalWrite(blue3, LOW);
    digitalWrite(blue4, LOW);
  }
  if (pin.length() == 2)
  {
    digitalWrite(blue1, HIGH);
    digitalWrite(blue2, HIGH);
    digitalWrite(blue3, LOW);
    digitalWrite(blue4, LOW);
  }
  if (pin.length() == 3)
  {
    digitalWrite(blue1, HIGH);
    digitalWrite(blue2, HIGH);
    digitalWrite(blue3, HIGH);
    digitalWrite(blue4, LOW);
  }
  if (pin.length() == 4)
  {
    digitalWrite(blue1, HIGH);
    digitalWrite(blue2, HIGH);
    digitalWrite(blue3, HIGH);
    digitalWrite(blue4, HIGH);
  }
  delay(100);
}
void touchFunc(String pin)
{
  if (str.length() < 4)
  {
    str += pin;
    lightUp(str);
  }
}
void flappyBird()
{
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
}

void lockLCD()
{
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  start();
}
void changepwd(String pin)
{
  if (pwd.length() < 4)
  {
    pwd += pin;
    lightUp(pwd);
  }
  else
  newpwd();
}
void newpwd()
{
  pwd = "";
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
    int n = 0;
  String nums[] = {"1", "4", "7", "2", "5", "8", "3", "6", "9", "GO", "0", "BK"};

  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.setColor(255, 255, 255);
      myGLCD.fillRect(12 + x * 77, 12 + y * 76, 77 + 77 * x, 76 + y * 76);
      myGLCD.setBackColor(255, 255, 255);
      myGLCD.setColor(0, 0, 0);
      if (n != 9 || n != 11)
      {
        myGLCD.print(nums[n], 37 + x * 77, 35 + y * 76);
      }

      else {
        myGLCD.print(nums[n], 32 + x * 77, 32 + y * 76);
      }
      n++;
    }
  }
  while (true)
  {
    x = 0;
    y = 0;
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
      if ((y >= 12) && (y <= 76)) // Upper row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 12, 77, 76);
          changepwd("1");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 12, 154, 76);
          changepwd("2");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 12, 231, 76);
          changepwd("3");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 12, 308, 76);
          myGLCD.setFont(BigFont);
          myGLCD.clrScr();
          myGLCD.setBackColor(0, 0, 0);
          myGLCD.setColor(255, 255, 255);
          if(pwd.length() == 4)
          {
          myGLCD.print("YOUR NEW PASSWORD IS", 20, 50);
          myGLCD.print(pwd, 120, 140);
          delay(2000);
          mainMenu();
          }
        }
      }
      if ((y >= 88) && (y <= 152)) // Center row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 88, 77, 152);
          changepwd("4");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 88, 154, 152);
          changepwd("5");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 88, 231, 152);
          changepwd("6");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 88, 308, 152);
          changepwd("0");
        }
      }
      if ((y >= 164) && (y <= 228)) // Center row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 164, 77, 228);
          changepwd("7");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 164, 154, 228);
          changepwd("8");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 164, 231, 228);
          changepwd("9");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 164, 308, 228);
          if (pwd.length() > 0)
          {
            pwd = pwd.substring(0, pwd.length() - 1);
            lightUp(pwd);
            delay(100);
          }
          else
          {
            pwd = "";
            lightUp(pwd);
            delay(100);
          }
        }
      }
    }
  }
}
void addFing()
{
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  //enroll();
}

void mainMenu()
{
  int n = 0;
  String nums[] = {"LOCK", "SET PWD", "ADD FING"};
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  for (int x = 0; x < 3; x++)
  {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(255, 255, 255);
    myGLCD.fillRect(4 + x * 104, 70, 104 + x * 104, 170);
    myGLCD.setBackColor(255, 255, 255);
    myGLCD.setColor(0, 0, 0);
    myGLCD.print(nums[x], 24 + x * 104, 120);
  }
  while (true)
  {
    x = 0;
    y = 0;
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
      if ((x > 4 && x < 104 ) || (x > 108 && x < 208) || (x > 212 && x < 312)) {
        break;
      }
    }
  }
  if (x > 4 && x < 104)
  {
    waitForIt(4, 70, 104, 170);
    myservo.write(1);
    delay(2000);
    lockLCD();
  }
  if (x > 108 && x < 208)
  {
    waitForIt(108, 70, 208, 170);
    newpwd();
  }
  if (x > 212 && x < 312)
  {
    waitForIt(212, 70, 312, 170);
    addFing();
  }

}

boolean pwdConfirm(String a)
{

  if (a.equals(pwd))
  {

    digitalWrite(blue1, LOW);
    digitalWrite(blue2, LOW);
    digitalWrite(blue3, LOW);
    digitalWrite(blue4, LOW);
    myservo.write(179);
    mainMenu();
    return true;
  }

  else
  {

    return false;
  }
}

void screenFunc()
{
  int n = 0;
  String nums[] = {"1", "4", "7", "2", "5", "8", "3", "6", "9", "GO", "0", "BK"};

  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.setColor(255, 255, 255);
      myGLCD.fillRect(12 + x * 77, 12 + y * 76, 77 + 77 * x, 76 + y * 76);
      myGLCD.setBackColor(255, 255, 255);
      myGLCD.setColor(0, 0, 0);
      if (n != 9 || n != 11)
      {
        myGLCD.print(nums[n], 37 + x * 77, 35 + y * 76);
      }

      else {
        myGLCD.print(nums[n], 32 + x * 77, 32 + y * 76);
      }
      n++;
    }
  }
  while (true)
  {

    x = 0;
    y = 0;
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
      if ((y >= 12) && (y <= 76)) // Upper row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 12, 77, 76);
          touchFunc("1");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 12, 154, 76);
          touchFunc("2");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 12, 231, 76);
          touchFunc("3");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 12, 308, 76);
          if (pwdConfirm(str))
          {
            digitalWrite(green, HIGH);
            delay(1000);
            digitalWrite(green, LOW);
            //Serial.println("ACCESS GRANTED");
            str = "";
            delay(100);
            break;
          }
          else
          {
            digitalWrite(redPin, HIGH);
            delay(1000);
            digitalWrite(redPin, LOW);
            str = "";
            digitalWrite(blue1, LOW);
            digitalWrite(blue2, LOW);
            digitalWrite(blue3, LOW);
            digitalWrite(blue4, LOW);
            //Serial.println("ACCESS DENIED");
            delay(100);
          }

        }
      }
      if ((y >= 88) && (y <= 152)) // Center row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 88, 77, 152);
          touchFunc("4");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 88, 154, 152);
          touchFunc("5");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 88, 231, 152);
          touchFunc("6");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 88, 308, 152);
          touchFunc("0");
        }
      }
      if ((y >= 164) && (y <= 228)) // Center row
      {
        if ((x >= 12) && (x <= 77)) // Button: 1
        {
          waitForIt(12, 164, 77, 228);
          touchFunc("7");
        }
        if ((x >= 89) && (x <= 154)) // Button: 1
        {
          waitForIt(89, 164, 154, 228);
          touchFunc("8");
        }
        if ((x >= 166) && (x <= 231)) // Button: 1
        {
          waitForIt(166, 164, 231, 228);
          touchFunc("9");
        }
        if ((x >= 243) && (x <= 308)) // Button: 1
        {
          waitForIt(243, 164, 308, 228);
          if (str.length() > 0)
          {
            str = str.substring(0, str.length() - 1);
            lightUp(str);
            delay(100);
          }
          else
          {
            str = "";
            lightUp(str);
            delay(100);
          }
        }
      }
    }
  }
}
void setup()
{
  Serial.begin(9600);
  myservo.attach(44);
  pinMode(blue1, OUTPUT);
  pinMode(blue2, OUTPUT);
  pinMode(blue3, OUTPUT);
  pinMode(blue4, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(45, OUTPUT);
  start();
}
void start() {
  // put your setup code here, to run once:
  
  delay(100);
  fps.Open();
  fps.SetLED(true);
  digitalWrite(redPin, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue1, LOW);
  digitalWrite(blue2, LOW);
  digitalWrite(blue3, LOW);
  digitalWrite(blue4, LOW);
  digitalWrite(45, HIGH);
  myservo.write(1);
  checkFinger();
}

void checkFinger() {
  // put your main code here, to run repeatedly:
  // Identify fingerprint test
  while(true)
  {
  if (fps.IsPressFinger())
  {

    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id < 200)
    {
      fps.SetLED(false);
      digitalWrite(45, LOW);
      myGLCD.InitLCD();
      myGLCD.setFont(BigFont);
      myGLCD.clrScr();
      myTouch.InitTouch();
      myTouch.setPrecision(PREC_HI);
      screenFunc();
      //    digitalWrite(blue1, HIGH);
      //    digitalWrite(blue2, HIGH);
      //    digitalWrite(blue3, HIGH);
      //    digitalWrite(blue4, HIGH);
    }
  }
  else
  {
    digitalWrite(blue1, LOW);
    digitalWrite(blue2, LOW);
    digitalWrite(blue3, LOW);
    digitalWrite(blue4, LOW);
  }
  delay(100);
}
}
void loop()
{
  delay(100);
}

