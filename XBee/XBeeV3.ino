#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <SPI.h>       // SPI communication library required by BnrOne.cpp


BnrOneA one;           // declaration of object variable to control the Bot'n Roll ONE A

#define SSPIN  2              // Slave Select (SS) pin for SPI communication
#define VELOCIDADE_VIRAGEM 15 // velocidade dos motores na viragem
#define VELOCIDADE_FRENTE 70  // velocidade dos motores na marcha para a frente
#define VELOCIDADE_TRAS 30    // velocidade dos motores na marcha para trás
#define DELAY_CONTROL 5 

int data;
byte obstaculo=0;
bool amoverfrente=false;
void setup() 
{
    Serial.begin(57600);          // sets baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);        // starts the SPI communication module
    one.stop();                   // stops motors
    one.lcd1("  CPR-AEPS  ");     // informação LCD1
    one.lcd2("REMOTE Via XBEE");  // informação LCD2
    one.stop();                   // stop motors
    one.obstacleEmitters(ON);     // activate obstacles detection
    delay(1000);                  // waits 1 second
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
    delay(DELAY_CONTROL);
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
    delay(DELAY_CONTROL);
    return;
}

/**
 * Declaração da função moveParaFrente
 */
void moveParaFrente ()
{
    one.lcd2("  Frente ");
    one.move(VELOCIDADE_FRENTE,VELOCIDADE_FRENTE);          
    delay(DELAY_CONTROL);
    return;
}

/**
 * Declaração da função moveParaTras
 */
void moveParaTras ()
{
    one.lcd2("  Tras ");
    one.move(-VELOCIDADE_TRAS,-VELOCIDADE_TRAS);         
    delay(DELAY_CONTROL);
    return;
}

/**
 * Declaração da função para
 * Esta função é responsável por parar os motores
 */
void para ()
{
    one.lcd2("     Parado   ");
    one.stop(); 
    return;
}

/**
 * Declaração da função para ler bateria
 * Esta função é responsável ler a voltagem da bateria e envia dados serial
 */
void leBateria ()
{
    float battery = one.readBattery(); // read battery voltage
    one.lcd2("Bateria V: ",battery); // print data on LCD line 2
    Serial.println(battery);         // print data on serial monitor. 
    return;
}




/**
 * Funcção principal do programa
 */
void loop()
{

  obstaculo = 0;
  obstaculo = one.obstacleSensors(); //Read obstacle sensors 
  delay(DELAY_CONTROL);
  
  if(obstaculo != 0 && amoverfrente==true){
    para();
  }
  
  if (Serial.available())
  {
    data = Serial.read();
    delay(DELAY_CONTROL);
    switch(data)
    {
      case 'F': // move frente
          moveParaFrente();
          amoverfrente=true;
          break;
          
      case 'T': // move trás
          moveParaTras();
          amoverfrente=false;
          break;
          
      case 'D': // Vira direita
          viraParaDireita(1);
          amoverfrente=false;  
          break;
          
      case 'E': // Vira esquerda
          viraParaEsquerda(1);  
          amoverfrente=false;
          break;

      case 'P': // Vira esquerda
          para();  
          break;

       case 'B': //Leitura bateria
          leBateria();
          break;    
    }
  }
}


