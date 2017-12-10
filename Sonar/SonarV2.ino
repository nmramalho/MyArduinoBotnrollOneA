#include <SPI.h>              
#include <BnrOneA.h>

//constants definitions
#define SSPIN  2    //Slave Select (SS) pin for SPI communication

#define MAX_SERVO1_DIREITA 5
#define MAX_SERVO1_ESQUERDA 180
#define MAX_SERVO2_CIMA 30
#define MAX_SERVO2_BAIXO 160
#define MAX_SERVO1_MEIO 100
#define MAX_SERVO2_FRENTE 120

#define VELOCIDADE_VIRAGEM 25   // velocidade dos motores na viragem
#define VELOCIDADE_FRENTE 30   // velocidade dos motores na marcha para a frente
#define VELOCIDADE_TRAS 30      // velocidade dos motores na marcha para trás

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED

BnrOneA one;         //declaration of object variable to control the Bot'n Roll ONE

#define maximumRange 200   // Maximum range needed (200cm)
#define minimumRange   0   // Minimum range needed

int Sonar()
{
    unsigned long duration;  // Duration used to calculate distance
    int distance;
    unsigned long tempo=micros();
  
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
  
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 11640);
    delayMicroseconds(16000 - (micros()-tempo));  // esta rotina demora sempre o mesmo tempo (maximo 16 milisegundos)
  
    //Calculate the distance (in cm) based on the speed of sound
    distance = (int)(duration/58.2);
    if (distance >= maximumRange || distance <= minimumRange)
      distance=-1;
  
    return distance;
}

void setup()
{
    //setup routines
    Serial.begin(57600);    // set baud rate to 115200bps for printing values in serial monitor.
    one.spiConnect(SSPIN);  // start SPI module
    one.stop();              // stop motors

    one.obstacleEmitters(ON);// activate obstacles detection
  
    while(one.readButton()==0){
        one.lcd2("Pressione botao!");
        delay(50);
      }
 
    // SONAR setup
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(LEDPin,  OUTPUT); // Use LED indicator (if required)
}

/**
 * Declaração da função viraParaDireita
 * Esta função recebe como argumento um inteiro que corresponde 
 * ao número de segundos de viragem.
 */
void viraParaDireita (int segundos)
{
    one.lcd2("  Direita ");
    one.move(VELOCIDADE_VIRAGEM,-VELOCIDADE_VIRAGEM); 
    delay(segundos*700);
    one.stop();              
    delay(500);
    return;  
}
/**
 * Declaração da função viraParaEsquerda
 * Esta função recebe como argumento um inteiro que corresponde 
 * ao número de segundos de viragem.
 */
void viraParaEsquerda (int segundos)
{
    one.lcd2("  Esquerda ");
    one.move(-VELOCIDADE_VIRAGEM,VELOCIDADE_VIRAGEM);         
    delay(segundos*700);
    one.stop();              
    delay(500);
    return;
}

/**
 * Declaração da função moveParaFrente
 * Esta função recebe como argumento um inteiro que corresponde 
 * ao número de segundos de viragem.
 */
void moveParaFrente ()
{
    one.move(VELOCIDADE_FRENTE,VELOCIDADE_FRENTE);          
    return;
}

/**
 * Declaração da função moveParaTras
 * Esta função recebe como argumento um inteiro que corresponde 
 * ao número de segundos de espera.
 */
void moveParaTras (int segundos)
{
    one.lcd2("  Trás ");
    one.move(-VELOCIDADE_TRAS,-VELOCIDADE_TRAS);         
    delay(segundos*1000);
    one.brake(VELOCIDADE_FRENTE,VELOCIDADE_FRENTE);
    delay(500);
    return;
}

/**
 * Declaração da função para
 * Esta função é responsável por parar os motores
 */
void para ()
{
    one.stop(); 
    return;
}

void OlhaDireita(){
  one.servo1 (MAX_SERVO1_DIREITA); 
  }
  
void OlhaEsquerda(){
  one.servo1 (MAX_SERVO1_ESQUERDA);
  }


void OlhaCima(){
  one.servo2 (MAX_SERVO2_CIMA);
  }


void OlhaBaixo(){
  one.servo2 (MAX_SERVO2_BAIXO);
  }

void OlhaFrente(){
  one.servo2 (MAX_SERVO2_FRENTE);
  one.servo1 (MAX_SERVO1_MEIO);
 }

void loop(){
    
     long distance;
    //moveParaFrente();
    OlhaFrente();
    distance=Sonar();
    one.lcd2("distance: ", distance);
    Serial.println(distance);
    delay(20);
    
   if(distance > 0 && distance < 20){
      para();
      OlhaDireita();
      delay(100);
      distance = Sonar();
      delay(100);
        if (distance > 20 || distance < 0){
            viraParaDireita(1);
            moveParaFrente();
        }
        else{
            OlhaEsquerda();
            delay(100);
            distance = Sonar();
                if (distance > 20 || distance < 0){
                  viraParaEsquerda(1);
                  moveParaFrente();
                }
                else{
                  moveParaTras(1);
                  viraParaDireita(1);
                  }
         }
        
    }
   else{ 
      moveParaFrente(); 
    }
}

