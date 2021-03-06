/********************************************************************************
IncuScope.c 
W. Zipfel, Tae Cooke                         V2.0                   10/2011

IncuScope  PIC code command set: 
Query command:  q - Returns a string which lists state of all LEDs
All LEDS off:  a  
Step motor forward for n cycles:   fn,  where n = 0 to 255
Step motor in reverse for n cycles:   rn, where n = 0 to 255
Toggle dark field illuminator ON/OFF:  d           
Toggle white LED ON/OFF :  w       
Toggle red LED ON/OFF :  1 
Toggle green LED ON/OFF :  2
Toggle blue LED ON/OFF :  3       

******************************************************************************/
#include <18F2455.h>
#fuses HSPLL,NOWDT, NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN, MCLR
#use delay(clock=48000000)
#include <stdlib.h>

// In usb.c/h - tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE    64  //allocate 64 bytes in the hardware for transmission   

#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_RX_SIZE    64    // allocate 64 bytes in the hardware for reception   

#include <pic18_usb.h>            // Microchip 18Fxx5x hardware layer for usb.c
#include "IncuScope_HIDdesc64.h"  // HID descriptor file
#include <usb.c>                  // handles usb setup tokens and get descriptor reports

#define  MOTOR_DRIVE   PIN_A4                // Motor: on = 1 off = 0
#define  MOTOR_DIR     PIN_A5                // Direction: Forward=0 Reverse=1
#define  LED_DF        PIN_B4                // Turn the Darkfield LED on
#define  LED_WHITE     PIN_B5                // Turn the White LED on
#define  LED_RED       PIN_C2                // Turn the Red LED on
#define  LED_GREEN     PIN_C1                // Turn the Green LED on
#define  LED_BLUE      PIN_C0                // Turn the Blue LED on
#define  MOTOR_DELAY       10                // number of ms to delay per for loop

// prototypes
void ExecuteCmd(void); 
void pulse(void);

//Global Variables
int8 command[64];      // command received from system (via usb)
int8 LEDStatus;        // byte that holds LED on/off status
int16 delay;
int16 i;
// bit 0 = Dark Field
// bit 1 = white LED
// bit 2 = Red LED
// bit 3 = Green LED
// bit 4 = blue LED
// the rest are unused for now

void main()
{
 //TurnLEDsOff();
 usb_init();               // initialize usb communication
 setup_adc_ports(NO_ANALOGS);
 setup_spi(FALSE);
 setup_timer_0(RTCC_INTERNAL);
 setup_timer_1(T1_DISABLED);
 setup_timer_2(T2_DISABLED,0,1);
 setup_comparator(NC_NC_NC_NC);
 setup_vref(FALSE);
 setup_oscillator(False);
 delay=4;
 output_high(PIN_B5);
 output_low(PIN_B6);
 
 while (TRUE) {
 output_low(PIN_B7);
  while (input(PIN_B6)) {  
 
      output_high(PIN_B6);
    
      output_high(PIN_B7);
      delay_ms(delay);
      output_low(PIN_B7);
      delay_ms(delay);
      
      
     
      
      
      if (usb_enumerated()) {         
          if (usb_kbhit(1)) {   // Check for USB commands
              usb_gets(1, command, 64, 100);
              ExecuteCmd();
              
          }
      }
      
    }
   output_low(PIN_B6);
   
   if (usb_enumerated()) {         
       if (usb_kbhit(1)) {   // Check for USB commands
           usb_gets(1, command, 64, 100);
           ExecuteCmd();
           
 
         }
   }
 }
}



//   Executes a command based on the 1st byte in the HUD string 
void ExecuteCmd(void) 
{
 int8 n; 
 
 switch (command[0]) {  
                  
      case '1':
         output_high(PIN_A0);
         output_high(PIN_A1);
         /*output_high(PIN_A2);
         output_high(PIN_A3);
         output_high(PIN_A4);
         output_high(PIN_A5);*/
        break;
    
          
      case '2':
         output_low(PIN_A0);
         output_low(PIN_A1);
         /*output_low(PIN_A2);
         output_low(PIN_A3);
         output_low(PIN_A4);
         output_low(PIN_A5);*/
         break;
       
      case '3':
         output_high(PIN_B5);
         break;
         
      case '4':
         output_low(PIN_B5);
         break;
         
         case '5':
         output_high(PIN_B6);
         /*output_high(PIN_B5);
         output_high(PIN_B4);
         output_high(PIN_B3);
         output_high(PIN_B2);
         output_high(PIN_B1);*/
         break;
         
     case '6':
         output_low(PIN_B6);
         /*output_low(PIN_B5);
         output_low(PIN_B4);
         output_low(PIN_B3);
         output_low(PIN_B2);
         output_low(PIN_B1);*/
         break;
         
     case '7':
         output_low(PIN_B4);
         break;
         
     case '8':
         output_high(PIN_B4);
         break;
         
     case 'a':
         output_high(PIN_B0);
         break;
         
     case 'b':
         output_low(PIN_B0);
         break;
          
     case 'c':  //1pulse
         pulse();
         break;
         
     case 'd'://2pulses
          for (i=0; i<2; i++){
           pulse();
       }
         break;
     case 'e'://4pulses
          for (i=0; i<4; i++){
           pulse();
       }
         break;
         
     case 'f'://8pulses
       for (i=0; i<8; i++){
           pulse();
       }
         break;    
     case 'g'://200pulses - 100um step
       for (i=0; i<40; i++){
           pulse();
       }
         break;  
         
         
      default:
       
   }
}
void pulse(void)
{
      output_high(PIN_B7);
      delay_ms(delay);
      output_low(PIN_B7);
      delay_ms(delay);
}
  

