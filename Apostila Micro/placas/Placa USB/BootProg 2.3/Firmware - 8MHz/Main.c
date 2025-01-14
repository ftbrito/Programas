/*
ERASE
<STX:1><Cmd:1><nBlocks:2>
WRITE
<STX:1><Cmd:1><address:2><DADOS:32>
START
<STX:1><Cmd:1>
INFO
<STX:1><Cmd:1>

*/

#include <Built_in.h>

enum Cmd { cmdNONE, cmdINFO, cmdSTART, cmdWRITE, cmdERASE };
enum McuType { PIC16 = 1, PIC18, PIC18FJ, PIC24, DSPIC = 10, PIC32 = 20 };

enum Cmd CmdCode = cmdNONE;

const STX = 0x0F;

const unsigned long BOOTLOADER_SIZE   = 7432;
const unsigned long BOOTLOADER_START  = (( __FLASH_SIZE - BOOTLOADER_SIZE) / _FLASH_ERASE) * _FLASH_ERASE;
const unsigned char RESET_VECTOR_SIZE = 4;

typedef struct
{
     char McuType;
     unsigned long FlashSize;
     unsigned EraseBlock;
     unsigned WriteBlock;
     unsigned long BootStart;
     unsigned EepromSize;
     char Device[19];
}BootInfo;

const BootInfo mcuBootInfo = { PIC18,
                               __FLASH_SIZE,
                               _FLASH_ERASE,
                               _FLASH_WRITE_LATCH,
                               BOOTLOADER_START,
                               256,
                               "PIC18F4550"
                               };

unsigned Address = 0;
unsigned EraseBlockSize at Address;

//Endere�o da USB RAM
unsigned char HidReadBuff[64]  absolute 0x500;
unsigned char HidWriteBuff[64] absolute 0x540;
unsigned char Reserve4thBankForUSB[256] absolute 0x400;

char flags = 0;
     sbit WriteData at flags.B0;
     
#define ConfigMem()   OrgAll(BOOTLOADER_START-RESET_VECTOR_SIZE); \
                      FuncOrg(main, BOOTLOADER_START); \
                      FuncOrg(StartProgram, BOOTLOADER_START-RESET_VECTOR_SIZE);\
                      if (Reserve4thBankForUSB) ;

//Envia uma resposta ao PC
void Ack( char cmd )
{
   HidWriteBuff[0] = STX;
   HidWriteBuff[1] = cmd;   //Comando
   while( !HID_Write(&HidWriteBuff, 64) );
}

//Verifica um comando recebido
void CheckCmd()
{
   if( CmdCode == cmdNONE )
   {
     if( HidReadBuff[0] != STX )   //cmdSTART
        return;
     
     CmdCode =     HIDReadBuff[1]; //Comando
     Hi(Address) = HidReadBuff[2]; //Address
     Lo(Address) = HidReadBuff[3]; //Address
   }
}

char EnterBootloader()
{
unsigned timer = 2000;
   while( timer-- )
   {
       USB_Polling_Proc();
       
       if( HID_Read() )
       {
            CheckCmd();
            
            if( CmdCode == cmdINFO )
            {
                  *(BootInfo*)(&HidWriteBuff+2) = mcuBootInfo;
                  Ack( cmdINFO );
                  CmdCode = cmdNONE;
                  return 1;
             }
       }
       
       Delay_ms(1);
   }
   
   return 0;
}

void StartBootloader()
{
   while(1)
   {
      USB_Polling_Proc();
      
      if( HID_Read() )
      {
        CheckCmd();

        if( CmdCode == cmdWRITE )
        {
          if( WriteData == 0 )
          {
               WriteData = 1;
          }
          else
          {
               if( Address < BOOTLOADER_START )
                          Flash_Write_32( Address, &HidReadBuff[4] );
          }
          Ack( cmdWRITE );
          CmdCode = cmdNONE;
        }
        
        else if( CmdCode == cmdERASE )
        {
           unsigned EraseBlocks;
           unsigned long max;
           for( EraseBlocks = 0; EraseBlocks < EraseBlockSize; EraseBlocks++ )
           {
               max = EraseBlocks * _FLASH_ERASE;
               if( max < BOOTLOADER_START )
                   Flash_Erase_64( max );
           }
           Ack( cmdERASE );
           CmdCode = cmdNONE;
        }
        
        else if( CmdCode == cmdSTART )
        {
           Ack( cmdSTART );
           return;
        }
     }
  }
}

//25276, 0x62BC
void StartProgram()
{
  asm sleep;
}

//25280, 0x62C0
void main()
{
  ConfigMem();
  
  HID_Enable( &HidReadBuff, &HidWriteBuff );
     
  if( EnterBootloader() )
  {
     StartBootloader();
  }
  
  Hid_Disable();
  Delay_ms(10);
  StartProgram();
}