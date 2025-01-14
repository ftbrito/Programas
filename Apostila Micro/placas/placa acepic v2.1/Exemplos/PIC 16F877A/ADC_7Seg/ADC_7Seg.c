/*******************************************************************************
*                     Kit de desenvolvimento ACEPIC PRO V2.0                   *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Teste Display 7 segmentos + conversoses Anal�gicos Digitais AN0 e AN1
*Obs.:     Ligar chave 1 do DIP DP1 e chaves 1, 5, 6, 7 e 8 do DIP DP2         * 
*          Ap�s a grava��o, tire o cabo serial                                 * 
********************************************************************************/
#include<16F877A.h> 
#device ADC = 10
#use delay (clock=8000000)        
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP 

#use fast_io(a)            

int conta=0;
int16 adc, d1, d2, d3, d4;

/*Matriz de 10 posi��es contendo o valor a ser enviado para a porta D a fim de
   mostrar o d�gito referente nos displays */
int digito[10] =
               {0b00111111,    //D�gito 0
                0b00000110,    //D�gito 1
                0b01011011,    //D�gito 2
                0b01001111,    //D�gito 3
                0b01100110,    //D�gito 4
                0b01101101,    //D�gito 5
                0b01111101,    //D�gito 6
                0b00000111,    //D�gito 7
                0b01111111,    //D�gito 8
                0b01101111     //D�gito 9
               };

#INT_RTCC               //Identifica��o da interrup��o do Timer 0
void trata_t0()            //fun��o de tratamento da interrup��o do Timer 0   
{
Conta++;                     //Incrementa a vari�vel conta
if (conta>4) conta = 1;         //Se conta > 4, faz conta = 1

switch(conta)               //�Chaveia� o valor da vari�vel conta
          {
         case 1: OUTPUT_D(digito[D1]);   //Coloca na Porta D o valor referente ao d�gito 0
                       OUTPUT_A(0x00); 
                       OUTPUT_E(0X01);      //Liga o Display 1 e desliga os demais 
                       break; 
      case 2: OUTPUT_D(digito[D2]);   //Coloca na Porta D o valor referente ao d�gito 1
                       OUTPUT_E(0X02);      //Liga o Display 2 e desliga os demais
                       break; 
      case 3: OUTPUT_D(digito[D3]);   //Coloca na Porta D o valor referente ao d�gito 2
                       OUTPUT_E(0X04);      //Liga o Display 3 e desliga os demais
                       break; 
      case 4: OUTPUT_D(digito[D4]);   //Coloca na Porta D o valor referente ao d�gito 2
                       OUTPUT_E(0x00); 
                       OUTPUT_A(0X20);      //Liga o Display 4 e desliga os demais
                       break; 
          } 

SET_TIMER0(100);
  
}


void main()
{
SET_TRIS_A(0x01);    //Direciona o RA0 como entrada e o restante como sa�da.                  

ENABLE_INTERRUPTS(GLOBAL|INT_TIMER0);   //Habilita as interrup��es Global e do Timer 0/
SETUP_TIMER_0(RTCC_INTERNAL | RTCC_DIV_32 | RTCC_8_BIT); //Seleciona o prescaler de 1:32
SET_TIMER0(100);                  //Inicia o registrador TMR0

SETUP_ADC_PORTS(AN0);       //Configurada a entrada anal�gica, somente a entrada RA0
SETUP_ADC(ADC_CLOCK_INTERNAL);    //Configurado o conversor AD interno   
SET_ADC_CHANNEL(0);            //Configurado o canal de leitura 0
delay_us(10);


  
while(true)                     //Loop principal
        {
        adc = READ_ADC();
        d1 = adc / 1000;
        d2 = (adc/100) % 10;
        d3 = (adc/10) % 10;
        d4 = (adc/1) % 10;
        delay_ms(500);          
        }
}
