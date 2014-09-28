#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;

#define LED  7
#define OUT  2
#define S2   5
#define S3   6
#define S0   3
#define S1   4

#define  Filters_R  0
#define  Filters_G  1
#define  Filters_B  2

// définition des broches utilisées 
int trig = 8; 
int echo = 9; 
long lecture_echo; 
long cm;
long distance;
Servo capteur;

//wheels
int pin_servoWheelLeft = 13;
int pin_servoWheelRight = 12;
int pin_capteurMotor = 10;
int variator = 200;                                      // Positive is turn under the direction of clockwise
                                                         // Negative is turn on the oposite of the direction of clockwise
int little_variator = variator/10;                       // Positive is turn under the direction of clockwise
                                                         // Negative is turn on the oposite of the direction of clockwise


long count = 0;
int counter = 0;
int G_flag = 1;
int RGB = 0;
float scaleFactor[3];
float buff[3];
int sem = 0;

int b = 0;        //couleurs permises
int v = 0;
int j = 0;
int r  = 1;
int n = 1;

/******************************************
* IMPORTANT - Make sure you calibrate
* your sensor for White Balance initially
* e.g. place your TCS230/TCS3200 in front
* of a piece of white paper
*******************************************/

void setup()
{
  pinMode(OUT, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
 
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);
  


  Serial.println("Starting...");
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  
  digitalWrite(LED, HIGH);
  delay(0);
  
  for(int i=0; i<3; i++)
  {
    Select_Filters(i);
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    scaleFactor[i] = 255.0/count;

    Serial.print("count: ");
    Serial.println(count);
    Serial.print("counter: ");
    Serial.println(counter);
    Serial.print("scaleFactor: ");
    Serial.println(scaleFactor[i],9);    
    counter=0;
    count=0;
  }
  
  digitalWrite(LED, LOW);
    pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  distance = capteur_son();
  //moteur
  servoLeft.attach(pin_servoWheelLeft);
  servoRight.attach(pin_servoWheelRight);
  capteur.attach(pin_capteurMotor);
  Serial.begin(115200); 
  
  

}

void loop()
{
    //avancer();
  
  digitalWrite(LED, HIGH);
  delay(0);
  
  distance = capteur_son();
  
  Serial.println(distance);
  
  if (distance < 10 && distance != 0){
    //Serial.println("TURN NOW !");
    //tourner_droite();
     reperage();

  }
  avancer(5000);
 
  
  
  
  for(int i=0; i<3; i++)
  {
    RGB=i;
    Select_Filters(i);
    count=0;
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    Serial.print("value: ");
    
    Serial.println(scaleFactor[i]*count);
    Serial.print("counter ");
    Serial.println(counter);
    
    //ajout
    
    buff[i] = scaleFactor[i]*count;
    
  
    counter=0;
    count=0;
    if (i==2)
    {
      Serial.print("buff 1 : ");
      Serial.println(buff[0]);
      Serial.print("buff 2 : ");
      Serial.println(buff[1]);
      Serial.print("buff 3 : ");
      Serial.println(buff[2]);
      if ( buff[0] >=25 && buff[0] <= 40 && buff[1] >= 30 && buff[1] <= 55 && buff[2] >= 60 && buff[2] <= 90 )//&& buff[2] > buff[0] && buff[2] > buff[1])
      {
        Serial.println("BLUE DETECTED");
        servoLeft.detach();                      // Attach left signal to pin 13
        servoRight.detach();
        if(b == 0)       //si 1ere fois bleue
          armurerie();
        else{
          reculer(1000);
          tourner_droite(1500);          
        }
      }
      if (buff[0] >= 20 && buff[0] < 120 && buff[1] >= 60 && buff[1] < 255 && buff[2] >= 40 && buff[2]< 120) //&& buff[1] > buff[0] && buff[1] > buff[2] )
      {
        Serial.println("GREEN DETECTED");
        servoLeft.detach();                      // Attach left signal to pin 13
        servoRight.detach();
      }
      if ( buff[0] >= 10 && buff[0] <= 60 && buff[1] >= 10 && buff[1] <= 60 && buff[2] >= 10 && buff[2] <= 60)
      {
        Serial.println("BLACK DETECTED");
        servoLeft.detach();                      // Attach left signal to pin 13
        servoRight.detach();
      }
      if (buff[0]>=230 && buff[1] >=230 && buff[2] >=230)
      {
        Serial.println("WHITE DETECTED");
       // servoLeft.detach();                      // Attach left signal to pin 13
        //servoRight.detach();
      }
      if ( buff[0] >= 90 && buff[0] <= 290 && buff[1] >= 35 && buff[1] <= 120 && buff[2] >= 35 && buff[2] <= 120 )//&& buff[0] > buff[1] && buff[0] > buff[2])
      {
        Serial.println("RED DETECTED");
        servoLeft.detach();                      // Attach left signal to pin 13
        servoRight.detach();
      }
      if ( buff[0] >= 200 && buff[0] <= 255 && buff[1] >= 190 && buff[1] <= 225 && buff[2] >= 90 && buff[2] <= 130)
      {
        Serial.println("YELLOW DETECTED");
        servoLeft.detach();                      // Attach left signal to pin 13
        servoRight.detach();
        if(j == 0)
          mage();
        else{
          reculer(1000);
          tourner_droite(1500);
        }
      }
    }
  }
  
  digitalWrite(LED, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);  
  
  delay(0);
  digitalWrite(S1, HIGH);
  
    

  
}

void Select_Filters(int RGB)
{
  switch(RGB)
  {
    case Filters_R:          //Red
    digitalWrite(S2, LOW);    
    digitalWrite(S3, LOW);
    Serial.println("-----select Red color");
    break;
    
    case Filters_G:          //Green
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Green color");
    break;
    
    case Filters_B:          //Blue
    digitalWrite(S2, LOW);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Blue color");
    break;
    
    default:                  //Clear(no filter)
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, LOW);
    Serial.println("-----no filter");
    break;
  }
}

long capteur_son(){
 digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  /*Serial.print("Distance en cm : "); 
  Serial.println(cm);*/
  return(cm);
}

void reperage(){
  long scanned_values = 0;
  long distanceMur;
  long temp = 0;
  int tempI = 0;
  int scan_positions[7] = {0,30,60,90,120,150,180};
  int degre_virage[7] = {600,400,200,0,200,400,600};
  
  reculer(800);
  
  servoLeft.detach();
  servoRight.detach();

  delay(100);
  
  for (int i = 0; i < 7; ++i)
  {

    capteur.write(scan_positions[i]);
    temp = capteur_son();
    if( temp > scanned_values)
    {
      scanned_values = temp;
      tempI = i;
    }
    delay(200);

  }
  capteur.write(scan_positions[3]);
  
  servoLeft.attach(pin_servoWheelLeft);
  servoRight.attach(pin_servoWheelRight);
  
  if(tempI > 3){
    //tourner gauche
    tourner_gauche(degre_virage[tempI]);
  } else{
    //tourner droite
    tourner_droite(degre_virage[tempI]);
  }
}

void avancer(int time){
  servoLeft.writeMicroseconds(1300);         
  servoRight.writeMicroseconds(1700);
  delay(time);
}

void reculer(int time){
  servoLeft.writeMicroseconds(1700);         
  servoRight.writeMicroseconds(1300);
  delay(time);
}

void tourner_gauche(int time){
   servoLeft.writeMicroseconds(1300);   // Left wheel counterclockwise
   servoRight.writeMicroseconds(1300); // Right wheel counterclockwise
   delay(time);
}

void tourner_droite(int time){
 servoLeft.writeMicroseconds(1700);   // Left wheel counterclockwise
   servoRight.writeMicroseconds(1700); // Right wheel counterclockwise
   delay(time);
}

void avancer_timer(int time){
  servoLeft.writeMicroseconds(1300);         
  servoRight.writeMicroseconds(1700);
  delay(time);
}

void armurerie(){
  b = 1;
  // Scan et calibrage pendant arret.
  servoLeft.attach(pin_servoWheelLeft);
  servoRight.attach(pin_servoWheelRight);
  
  avancer_timer(3000);
  
  reculer(1500);
  
  /* Echauffement, "coups d'epée dans le vent" */
  
  avancer_timer(500);
  
  tourner_droite(500);
  
  reculer(1200);
  
  tourner_gauche(500);
  
  tourner_droite(1500);
  
  avancer_timer(500);
 
  
}

void mage(){
  // Scan et calibrage pendant arret.
  j = 1;
  servoLeft.attach(pin_servoWheelLeft);
  servoRight.attach(pin_servoWheelRight);
  
  tourner_droite(800);
  
  tourner_gauche(1600);
  
  avancer_timer(500);
  
 reculer(1000);
  
  avancer_timer(500);
  
  tourner_droite(2000);

  
}
