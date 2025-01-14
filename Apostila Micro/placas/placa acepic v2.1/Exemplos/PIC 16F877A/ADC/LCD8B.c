#define EN PIN_E1			//Definimos o pino 1 da porta E como EN
#define RS PIN_E0			//Definimos o pino 0 da porta E como RS
#define DATA OUTPUT_D		//Definimos a sa�da para a porta D como DATA


void lcd_cmd(byte cmd)		//Fun��o para envio de comandos para o LCD
{
 DATA(cmd);				//Coloca o conte�do da vari�vel cmd na porta D
 OUTPUT_HIGH(EN);			//Pulso alto (1) no pino EN (Pino 1 da porta E)
 OUTPUT_LOW(EN);			//Retorna a n�vel baixo (0) em EN
}


void lcd_envia_byte(boolean nivel,byte dado) 	//Fun��o para envio de dados ou escrita para o 
 												//LCD
{
 OUTPUT_LOW(RS);			//Coloca em n�vel baixo o pino RS (Pino 0 da porta E)
 OUTPUT_BIT(RS,nivel);		//Coloca o pino RS no n�vel da vari�vel nivel
 delay_us(100);				//Atraso de 100 us
 OUTPUT_LOW(EN);			//Coloca em n�vel baixo o pino EN
 lcd_cmd(dado);				//Chama a fun��o lcd_cmd j� com os dados a serem enviados   
 							//para a porta D
}



void lcd_pos_xy(byte x, byte y)	//Fun��o de posicionamento do cursor			
{
 byte endereco;		 		//Vari�vel de informa��o para o endere�o do cursor 
 if (y!=1)			 		//Se o valor de y for 2
         endereco = 0xC0;	 	//ent�o endereco vai ser igual a 0xC0 (endere�o da segunda linha)
 else				 		//Sen�o
         endereco = 0x80;	 	//endere�o vai ser igual a 0x80 (endere�o da primeira linha) 
 endereco += x-1;         	 	//Aqui decrementa o valor da vari�vel x e o resultado � somado com
 				 			//a vari�vel endere�o...
 lcd_envia_byte(0,endereco);  //Chama a fun��o lcd_envia_byte, com o valor 0, informando para
 				  			  //o LCD que ser� enviado um dado e o dado est� contido na
				  			  //vari�vel endere�o...		
 }
 
void lcd_escreve(char c)		//Fun��o para envio dos caracteres e/ou dados para o LCD
{
 switch(c)						//comando switch com a vari�vel c
 {
  case '\f' : lcd_envia_byte(0,1);	//Caso c seja �\f�, o dado 1 ser� enviado ao LCD para 
								//limpar todo o seu conte�do.
                  delay_ms(2);		//Atraso de 2 ms
                  break;				//Comando break, terminou o processo acima, j� n�o testa 
 								//nenhum outro caso... 
  case '\n' : 					//Caso c seja �\n�
  case '\r' : lcd_pos_xy(1,2);		//ou �\r�, muda o cursor para a segunda linha do LCD
                  break;				//Comando break
  case '\b' : lcd_envia_byte(0,0x10); //Caso c seja �\b� ent�o desloca o cursor para a esquerda
                   break;				//Comando break
  default    : lcd_envia_byte(1,c);	//caso seja um caractere qualquer, ent�o este ser� escrito no 
 									//LCD pela fun��o lcd_envia_byte�
                   break;				//Comando break
 }
}  



void lcd_ini()						//Fun��o de inicializa��o do LCD
{
 byte conta;						//Vari�vel de contagem 
 	
 DATA(0x00);						//Coloca na porta D o valor 0x00
 OUTPUT_LOW(RS);					//Coloca em n�vel baixo o pino RS
 OUTPUT_LOW(EN);					//Coloca em n�vel baixo o pino EN
 
 delay_ms(15);						//Atraso de 15ms
 
 for (conta=1;conta<=3;conta++)	//Para conta = 1 at� conta = 3, ou seja executar� 3 vezes os  
 									//comandos abaixo
      {
      lcd_cmd(0x30);					//Envia o comando 0x30 para o LCD
      delay_ms(5);					//Atraso de 5ms
      }
 
 lcd_cmd(0x38); 		//Escreve comando para interface de 8 vias de dados
 lcd_cmd(0x01); 		//Escreve comando para limpar todo o display
 
 delay_ms(1);			//Atraso de 1ms
 
 lcd_cmd(0x0C); 		//Escreve comando para ligar o display sem cursor
 lcd_cmd(0x06); 		//Escreve comando para incrementar automaticamente � direita
} 
