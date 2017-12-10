#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <SPI.h>       // SPI communication library required by BnrOne.cpp
#include <Servo.h>
BnrOneA one;           // declaration of object variable to control the Bot'n Roll ONE A

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication

#define VELOCIDADE_VIRAGEM 26   // velocidade dos motores na viragem
#define VELOCIDADE_FRENTE 30   // velocidade dos motores na marcha para a frente
#define VELOCIDADE_TRAS 25      // velocidade dos motores na marcha para trás

#define GRIPPER_MAX_CIMA 130
#define GRIPPER_MAX_BAIXO 180

#define GRIPPER_MAX_ABERTO 140
#define GRIPPER_MAX_FECHADO 0

Servo gripperAbreFecha;
Servo gripperCimaBaixo;

void setup()
{
    Serial.begin(57600);     // set baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
    one.lcd1(" CPR - AEPS");
      
    gripperAbreFecha.attach(3);
    gripperCimaBaixo.attach(5);

    gripperCimaBaixo.write(GRIPPER_MAX_BAIXO);
    gripperAbreFecha.write(GRIPPER_MAX_FECHADO); 

    while(one.readButton()==0){
        one.lcd2("Pressione botao!");
        delay(50);
      }
}

/**
 * Função responsável pela abertura da garra
 * Parâmetros: não tem 
 * retorno: não tem
 */
void AbreGarra(){
    gripperAbreFecha.write(GRIPPER_MAX_ABERTO);  
    delay(1000); 
}

/**
 * Função responsável por fechar a garra
 * Parâmetros: não tem 
 * retorno: não tem
 */
void FechaGarra(){
    gripperAbreFecha.write(GRIPPER_MAX_FECHADO);  
    delay(1000); 
}

/**
 * Função responsável por levantar a garra
 * Parâmetros: não tem 
 * retorno: não tem
 */
void LevantaGarra(){
    gripperCimaBaixo.write(GRIPPER_MAX_CIMA); 
    delay(1000); 
}

/**
 * Função responsável por baixar a garra
 * Parâmetros: não tem 
 * retorno: não tem
 */
void BaixaGarra(){
    gripperCimaBaixo.write(GRIPPER_MAX_BAIXO); 
    delay(1000); 
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
void moveParaFrente (int segundos)
{
  int i=0;
  byte obstaculo;
    
   do{
      obstaculo=one.obstacleSensors(); //Read obstacle sensors 
      one.lcd2("  Frente ");
      one.move(VELOCIDADE_FRENTE,VELOCIDADE_FRENTE);          
      i++;
     }
     while (obstaculo == 0 &&  segundos > i/40 ); 
    
    
    one.brake(VELOCIDADE_FRENTE,VELOCIDADE_FRENTE);
    delay(500);
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
    one.lcd2("     Parado   ");
    one.stop(); 
   
    while(one.readButton()==0){
        one.lcd2("Pressione botao!");
        delay(50);
      }
    return;
}

void loop()
{

moveParaFrente(8);
viraParaEsquerda(1);
moveParaFrente(2);

AbreGarra();
moveParaFrente(1);
FechaGarra();
LevantaGarra();
moveParaTras(3);
viraParaEsquerda(1);
moveParaFrente(8);
para();


}
