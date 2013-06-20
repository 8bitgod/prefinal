int speed=0;

import hypermedia.net.*;                    

import controlP5.*;                  

ControlP5 cp5;                              

UDP udp;

int myColorBackground = color( 50);    

String myString;                      

boolean keyLocked;                 

boolean clearFirst=true;               

boolean[] keys = new boolean[526];

int tiltAngle = 0;                     

int TF = 0;

String string1="Temperature";

String string2="Gas";

String ip;          

int port;

boolean speedLock = false;             

int speedLockSpeed=0;                  


//Instances of buttons and widgets from ControlP5



Textarea console,sensor1,sensor2;

Textfield comField, ipAddress, portNumber;

Button upButton, downButton, rightButton, leftButton,ACT; 

Toggle lockingSwitch, lightSwitch;

Textlabel lightLabel,SensorLabel,WelcomeLabel;

Slider sliderSpeed;




void setup()
{

 size(340,300);     
 
 smooth();          
 
 noStroke();       
 
 udp = new UDP(this, 6000);
 
 udp.listen(true);
 
 cp5 = new ControlP5(this); 

console = cp5.addTextarea("txt").setPosition(5,20).setSize(150, 255).setFont(createFont("proggy",9)).setColor(color(255,128,0)).setColorBackground(color(0)); 

comField = cp5.addTextfield("Send Message Text").setPosition(5,280).setSize(110, 18).setFont(createFont("proggy",9)).setColor(color(255,0,0)); 

ipAddress = cp5.addTextfield("ipAddress").setPosition(190,50).setSize(110, 18).setFont(createFont("proggy",9)).setColor(color(255,128,0)).setColorBackground(color(0)).setValue("192.168.1.177");

portNumber = cp5.addTextfield("portNumber").setPosition(190,85).setSize(110, 18).setFont(createFont("proggy",9)).setColor(color(255,128,0)).setColorBackground(color(0)).setValue("8888");  

sensor1 = cp5.addTextarea(" txt").setPosition(190,125).setSize(110, 30).setFont(createFont("proggy",9)).setColor(color(255,128,0)).setColorBackground(color(0));

ACT = cp5.addButton("ACT").setPosition(302,130).setSize(30,18);

cp5.addBang("Send").setPosition(120, 280).setSize(35, 18).getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);  

upButton = cp5.addButton("up").setPosition(225, 180).setSize(25,25).setImages(loadImage("UPUn.PNG"), loadImage("UPOver.PNG"), loadImage("UPPress.PNG")); 

downButton = cp5.addButton("down").setPosition(225, 210).setSize(25,25).setImages(loadImage("DOWNUn.PNG"), loadImage("DOWNOver.PNG"), loadImage("DOWNPress.PNG"));

rightButton = cp5.addButton("right").setPosition(260, 210).setSize(25,25).setImages(loadImage("RIGHTUn.PNG"), loadImage("RIGHTOver.PNG"), loadImage("RIGHTPress.PNG")); 

leftButton = cp5.addButton("left").setPosition(190, 210).setSize(25,25).setImages(loadImage("LEFTUn.PNG"), loadImage("LEFTOver.PNG"), loadImage("LEFTPress.PNG")); 

SensorLabel = cp5.addTextlabel("Sensors").setPosition(188,160).setValueLabel("REAL TIME SENSORS");

WelcomeLabel = cp5.addTextlabel("''").setPosition(80,5).setValueLabel("HOMECON IS A FRUIT OF PERSISTENCE");

lightLabel = cp5.addTextlabel("lightLabel").setPosition(216,285).setValueLabel("TOGGLE LIGHT");

lockingSwitch = cp5.addToggle("LockingSwitch").setPosition(190,245).setSize(30,18).setValue(true);

sliderSpeed = cp5.addSlider("speed").setPosition(190, 30).setRange(0,90).setNumberOfTickMarks(91).snapToTickMarks(true).showTickMarks(false);  

lightSwitch = cp5.addToggle("lightSwitch").setPosition(290,280).setSize(30,18).setValue(false);


}

void draw() 
{

  background(myColorBackground);

   if(clearFirst==true&&!keyPressed)
  {   
    packageAndSend("clear", 0);
   
    clearFirst=false;
  }
}



void packageAndSend(String address, int parameter) 
{
  
  String stringParam = ""+parameter;         //convert parameter to string
  
  int i = address.length();
  
  int j = stringParam.length();
  
  int k = 10 - i - j;                        //figure out how many zeros program needs
  
  String zeroString = "";
  
  int counter = 0;
  
  while(counter < k){
  
    zeroString = zeroString + "0";
  
    counter++;
  }
  
  String finalString = address + zeroString + stringParam;   //throw everything together; 10 char in length
  
  ip = ipAddress.getText();
  
  port = int(portNumber.getText());
  
  udp.send(finalString, ip, port);          //send this bad boy to the Arduino
}



//If user presses a key, come here

void keyPressed() 
{

  if(!keyLocked)

  {      

    keys[keyCode] = true;

    if(checkKey(UP)){up();}

    else if(checkKey(DOWN)){down();}

    else if(checkKey(LEFT)){left();}

    else if(checkKey(RIGHT)){right();}

    else if(checkKey(KeyEvent.VK_L)){lightSwitch.toggle();}

    else if(checkKey(KeyEvent.VK_K)){ACT();}
  
  else {}

  }

else{ }

}

public void Send()

{

  String txtSend = comField.getText();

  addToConsole(txtSend);

  if(txtSend.length()<10)

{
  packageAndSend(txtSend, 0);
}

  else
  {                               //Don't go to packageAndSend if it's already 10 char long
    ip = ipAddress.getText();

    port = int(portNumber.getText());

    udp.send(txtSend, ip, port);

    }  
  
  console.scroll(1);                  
  
  comField.clear();                  
}

void receive( byte[] data)
{
  
    for(int i=0; i<data.length; i++)
  {
  
    myString = new String(data);                  //convert the data to string
  
  }

if(myString.charAt(0)==string1.charAt(0))
{
  MySensor1(myString);
}
else 
{
  addToConsole(myString); 
}
}


void up() 

{

  if(!speedLock){speed = 160 + int(sliderSpeed.getValue());}
  
  packageAndSend("front",speed);
  
  clearFirst=true; 
  
  addToConsole("Forward");
}

//Move Reginald backwards; check to see if "down" controls velocity or position

void down()
{
  
  if(!speedLock){speed = 160 + int(sliderSpeed.getValue());}
    
  packageAndSend("back0",speed);
    
  clearFirst=true; 
  
  addToConsole("Back");

}

void right()
{
  if(!speedLock)
{
  
  if(!speedLock){speed = 160 + int(sliderSpeed.getValue());}
  
  packageAndSend("right",speed);
 
  clearFirst=true;
  
  addToConsole("turning right");
}

}

void left() 
{
  if(!speedLock)
{
  
  if(!speedLock){speed = 160 + int(sliderSpeed.getValue());}
  
  packageAndSend("left0",speed);

  clearFirst=true;
  
  addToConsole("turning left");
}
}

void lightSwitch(boolean theFlag) {if(theFlag==true){addToConsole("Light activated"); packageAndSend("light", 1);}else{addToConsole("Light deactivated"); packageAndSend("light", 0);}}

void LockingSwitch(boolean theFlag) {if(theFlag==true){keyLocked = true; addToConsole("Keys are locked");}else{keyLocked = false; addToConsole("Keys enabled");}}

void ACT() {if(!speedLock){ addToConsole("Sensing.."); packageAndSend("Senso", 1);}}


void addToConsole(String stringToAdd)
{

String textInBox = console.getText(); 

console.setText(textInBox + "\n " + stringToAdd);

console.scroll(1);
}


void MySensor1(String stringToAdd)

{
  
  String textInBox = sensor1.getText(); 

sensor1.setText(textInBox +  stringToAdd);

sensor1.scroll(1);

}


boolean checkKey(int k)

{
  if (keys.length >= k) 
  {
    return keys[k];  
  }
  return false;

}
void keyReleased()

{ 

  keys[keyCode] = false; 

}


