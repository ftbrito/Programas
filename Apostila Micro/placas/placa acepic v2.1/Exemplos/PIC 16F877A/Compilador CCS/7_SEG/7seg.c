/*******************************************************************************
*                     Kit de desenvolvimento ACEPIC PRO V2.0                   *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Teste Display 7 segmentos                                           *
*Obs.:     Ligar chave 1, 5, 6, 7 e 8 do DIP DP2                               *
*          Ap�s a grava��o, tire o cabo serial                                 * 
********************************************************************************/
#include<16F877A.h>           
#use delay (clock=8000000)        
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP 

int conta=0;

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

#INT_TIMER0
void trata_t0()
{
conta++;
if (conta>4) conta = 1;

switch(conta)
          {
         case 1: OUTPUT_D(digito[0]);
                       OUTPUT_A(0X04);
                       break; 
      case 2: OUTPUT_D(digito[1]);
                       OUTPUT_A(0X08);
                       break; 
      case 3: OUTPUT_D(digito[2]);
                       OUTPUT_HIGH(PIN_E0);
                       OUTPUT_A(0X00);
                       break; 
      case 4: OUTPUT_D(digito[3]);
                       OUTPUT_LOW(PIN_E0);
                       OUTPUT_A(0X20);
                       break; 
          } 

SET_TIMER0(100); 
}


void main()
{
ENABLE_INTERRUPTS(GLOBAL | INT_TIMER0);
SETUP_TIMER_0(RTCC_INTERNAL | RTCC_DIV_16 |RTCC_8_BIT);
SET_TIMER0(100);

OUTPUT_A(0X00);
OUTPUT_LOW(PIN_E0);

while(true);
}


