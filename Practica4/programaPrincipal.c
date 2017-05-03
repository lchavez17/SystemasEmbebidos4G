#include <18F4620.h>
#include <stdlib.h>
#fuses INTRC_IO, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT 
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#use RS232(BAUD=9600,BITS=8,PARITY=N,XMIT=PIN_C6,RCV=PIN_C7,STOP=1)  
#use standard_io(b)
#define tamanoBufer 11
//Variables globales
char Bufer[11];
int banderaError=0;
int posicion=0;
#int_rda
void rda_isr()
{     
      banderaError=0;
      Bufer[posicion]=getc();
      //Abilitar interrupci�n TMR0       
      //enable_interrupts(int_TMR0);
      //Resetear TMR0 "Desde donde va a correr" 
      //set_timer0(0);
      if(((Bufer[posicion]<'0' && Bufer[posicion]>'9') && Bufer[posicion]!='-' && Bufer[posicion]!='.'))     //Si no ingreso un numero, un - o un .   El dato es incorrecto 
      {
         banderaError=1;
      }
   //Desabiitar interrupci�n TMR0
   posicion++;
}
void limpiarBufer();
void main(void)
{
   //Configuraciones
   setup_oscillator(OSC_16MHZ);
   enable_interrupts(global);
   enable_interrupts(int_rda);
   SETUP_ADC_PORTS(NO_ANALOGS);
   //Variables
   inicio:   //Punto de retorno
   banderaError=0;
   float primerOperando=0, segundoOperando=0, resultado=0;
   char operacion='0';
   ////
   while(true)
   {   
      posicion=0;
      limpiarBufer();
      //enable_interrupts(int_rda);
      printf("Practica No. 4\r");
      printf("Calculadora Aritmetica \r\r");
      printf("Ingresa el primer operando \r");    //Indica al usuario que ingrese el primer operando
           
      while((!kbhit()) || ((Bufer[posicion-1]!=0x0D) && (Bufer[posicion-1]!=0x0A) && (posicion-1<tamanoBufer)));                          //Toque de tecla
      if(Bufer[posicion-1]==13)    Bufer[posicion-1]=0;
      if(banderaError==1)
      {
         printf("Dato incorrecto");
         goto inicio;
      }
      posicion=0;
      primerOperando=atof(Bufer);                 //Asigna la cifra casteandola a un flotante, a primer operando en "primerOperando"
      limpiarBufer();
      //enable_interrupts(int_rda);
      printf("%4.4f\r",primerOperando);              //Muestra el operando ingresado
      printf("Ingresa el segundo operando \r");   //Indica al usuario que ingrese el segundo operando
      
      while((!kbhit()) || ((Bufer[posicion-1]!=13) && (posicion-1<tamanoBufer)));                          //Toque de tecla        
      if(Bufer[posicion-1]==13)    Bufer[posicion-1]=0;
      if(banderaError)
      {
         printf("Dato incorrecto");
         goto inicio;
      }
      segundoOperando=atof(Bufer);                //Asigna la cifra casteandola a un flotante, a segundo operando en "segundoOperando"
      posicion=0;
      printf("%4.4f\r",segundoOperando);              //Muestra el operando ingresado
      limpiarBufer();
      //enable_interrupts(int_rda);
      //printf("%4.4f\r",segundoOperando);             //Muestra el operando ingresado
      //Pide Operacion
      printf("Seleccione una operacion: \r 1)Suma  2)Resta  3)Multiplicacion  4)Division \r");    //Indica al usuario que selecione segun su n�mero, una operaci�n 
      while(!kbhit());                          //Toque de tecla
      operacion=Bufer[0];                      //Asigna la operaci�n casteandola a un flotante, a operacion
  
      //Saca resultado con los dos operadores ingresados y deacuerdo a la operaci�n seleccionada
      if(operacion=='1')   resultado=primerOperando+segundoOperando;          //suma
      else if(operacion=='2')   resultado=primerOperando-segundoOperando;     //resta
      else if(operacion=='3')   resultado=primerOperando*segundoOperando;     //multiplicacion
      else if(operacion=='4')   resultado=primerOperando/segundoOperando;     //divicion
      //Muestra resultado
      printf("El resultado de la operacion es: %4.4f\r\r",resultado);
   } 
}
void limpiarBufer()
{
   int i=0;
   for(i=0; i<tamanoBufer; i++)
   {
      Bufer[i]=0;
   }
}