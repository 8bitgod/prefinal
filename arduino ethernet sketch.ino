
#include <SPI.h>

#include <Ethernet.h>

#include <EthernetUdp.h>   

#include <stdlib.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char gas[10],temp[10],gase[20],tempe[20],bat[10],bate[20];

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

IPAddress ip(192,168,1,177);

unsigned int localPort = 8888;

float vol,Vout,Vin;

#define Z1 70200

#define Z2 45500

EthernetUDP Udp;

int enableA = 3;

int pinA1 = 4;

int pinA2 =5;

int enableB =6;

int pinB1 =8 ;

int pinB2 =9;

void setup()

{
  Ethernet.begin(mac,ip);
  
  Udp.begin(localPort);

  Serial.begin(9600);
 
  Salutations();       
 
  pinMode(enableA, OUTPUT);

  pinMode(pinA1, OUTPUT);
 
  pinMode(pinA2, OUTPUT);

  pinMode(enableB, OUTPUT);

  pinMode(pinB1, OUTPUT);
 
  pinMode(pinB2, OUTPUT);

  pinMode(13,OUTPUT);

  enableMotors();
    
}
void loop()
{ 
  int packetSize = Udp.parsePacket();
  
  if(packetSize)
  {
    
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    
    String myString(packetBuffer);
    
    splitAndDirect(myString);
  }
}
void splitAndDirect(String packet)

{
  Serial.println(packet);
  
  int parameter=0;
  
  String firstHalf = packet.substring(0,5);
  
  String secondHalf = packet.substring(5,10);
  
  for(int i=0; i<5; i++){
   
    if(secondHalf.charAt(i)=='0')
   { 
   
   } 
   else 
   {
     String secondHalf2 = secondHalf.substring(i,5);
     
     char tmpArray[secondHalf2.length() + 1];
    
     secondHalf2.toCharArray(tmpArray, sizeof(tmpArray));
     
     parameter = atoi(tmpArray);
     
     break;
  
  }
 
  }
  
  int par = parameter;
  
  if(firstHalf=="front"){forward(par);}                 
 
  else if(firstHalf=="left0"){turnLeft(par);}           
  
  else if(firstHalf=="right"){turnRight(par);}       
  
  else if(firstHalf=="back0"){backward(par);}  
 
  else if(firstHalf=="light"){flashLight(par);}            
       
  else if(firstHalf=="clear"){clearValues();}  

  else if(firstHalf=="Senso"){sensor(par);}
  
}
void flashLight(int par)
{
  
  if(par)
  {
    digitalWrite(13,1);
    
    Serial.println("HIGH");
  
  } 
  else
  
  {
    digitalWrite(13,0);
    
    Serial.println("LOW");
  
  }
}

void sensor(int par)
{
  
  if(par)
  {
   
  float temp1 = (5.0 * analogRead(A0) * 100.0) / 1024;
 
  String dataString2 = "Temperature:";

  delay(750);
  
  dtostrf(temp1,1,2,temp);

  dataString2 += temp;
 
  dataString2.toCharArray(tempe,20);
  
  Serial.println(tempe);
  
  int sensorValue = analogRead(A5);
  
  delay(750);

  vol=(float)sensorValue/1024*5.0;
 
  dtostrf(vol,1,2,gas);

  String dataString3 = "Gas:";
  
  dataString3 += gas;
  
  dataString3.toCharArray(gase,20);
  
  Serial.println(gase);
  
  delay(750);
  
  Vout = analogRead(A0)*(5.0/1023.0);
  
  Vin = ((Z1 + Z2)* Vout)/(Z2) ;
  
  dtostrf(Vin,1,2,bat);
  
  String datastring4 = "VOLTAGE:";
  
  datastring4 +=bat;
  
  datastring4.toCharArray(bate,20);
  
  Serial.println(bate);
  
  //if(Vin<10)
  //{
    
  //Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
    
  //Udp.write("WARNING!!LOW VALUE!!");

  //Udp.write('\n');  

  //Udp.write(bate);

  //Udp.endPacket(); 
  
  //}
    
 
  if(vol>0.6)
  {
  
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

  Udp.write("WARNING!! HIGH VALUE!!");

  Udp.write('\n');  

  Udp.write(gase);

  Udp.endPacket(); 
  
    }    
    
 else if(temp1>40)
  {
  
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

  Udp.write("WARNING!! HIGH VALUE!!");

  Udp.write('\n');  

  Udp.write(tempe);

  Udp.endPacket(); 
  
}
 else
  {
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
   
  Udp.write(tempe); 
  
  Udp.write('\n');
    
  Udp.write(gase);

  Udp.write('\n');  
  
  Udp.write(bate);
           
  Udp.write('\n'); 
  
  Udp.endPacket(); 
  }
}
}
void clearValues()
{
  Serial.println("clear");

  brake();
}
void Salutations() 
{
 
  boolean flag = true;
  
  boolean flag2 = true;
 
  while(flag)
  
{     
 
    Serial.println("Here");
  
    delay(250);        
  
    int packetSize = Udp.parsePacket();
  
    if(packetSize)
   
{
     
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
     
      String wakeUp(packetBuffer);
    
      Serial.println(wakeUp);
    
      if(wakeUp == "wake000000") 
    
  {    
        
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
       
        Udp.write("Hello,I am HomeCon,my dad is Irshad;I am a personal surveillance bot; what is the password?");
       
        Udp.endPacket();
        
        while(flag2)
  {
        
          delay(150);  
         
          int packetSize2 = Udp.parsePacket();
         
          if(packetSize2)
        
     {
         
            Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
            
            Serial.println("The password you entered is");
            
            String password(packetBuffer);
           
            Serial.println(password);
            
            if(password == "homecon000")       //"password" is password, the GUI adds a "0" to the end of the packet because it's 9 char long
           
         {
            
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            
              Udp.write("Welcome; you now have access.");       
             
              Udp.endPacket(); 
             
              flag = false;
             
              flag2 = false;     
            }
            else                        
            {
           
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                      
             Udp.write("Sorry intruder; you do not have access.");       
           
              Udp.endPacket(); 
            }
          }
        }
      }
    }
  }
}
void motorAOn()
{
 analogWrite(enableA,250);
}
void motorBOn()
{
 analogWrite(enableB,250);
}
void motorAOff()
{
 digitalWrite(enableB, LOW);
}
void motorBOff()
{
 digitalWrite(enableA, LOW);
}

void motorABrake()
{
 digitalWrite(pinA1, HIGH);

 digitalWrite(pinA2, HIGH);
}
void motorBBrake()
{
 digitalWrite(pinB1, HIGH);

 digitalWrite(pinB2, HIGH);
}
void enableMotors()
{
 motorAOn();
 
 motorBOn();
}
void disableMotors()
{
 motorAOff();
 
 motorBOff();
}
void turnLeft(int par)
{
 analogWrite(enableA, par);  
  
 digitalWrite(pinA1, HIGH);

 digitalWrite(pinA2, LOW);
 
 analogWrite(enableB,par);
  
 digitalWrite(pinB1, HIGH);

 digitalWrite(pinB2, LOW);
 
 
}
void turnRight(int par)
{
 analogWrite(enableA,par); 
 
 digitalWrite(pinA1, LOW);

 digitalWrite(pinA2, HIGH);
 
 analogWrite(enableB,par);
  
 digitalWrite(pinB1, LOW);

 digitalWrite(pinB2, HIGH);

 }
void forward(int par)
{
 analogWrite(enableA,par);
  
 digitalWrite(pinA1, LOW);

 digitalWrite(pinA2, HIGH);
 
 analogWrite(enableB,par);
  
 digitalWrite(pinB1, HIGH);

 digitalWrite(pinB2, LOW);
 
 }
void backward(int par)
{
 analogWrite(enableA,par);  
  
 digitalWrite(pinA1, HIGH);

 digitalWrite(pinA2, LOW);
 
 analogWrite(enableB,par);
  
 digitalWrite(pinB1, LOW);

 digitalWrite(pinB2, HIGH);
 
}
void brake()
{
 motorABrake();
 
 motorBBrake();
 
}


