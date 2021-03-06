// Do not remove the include below
#include 	"Gest_Modbus.h"
#include	"config_practicas.h"
#include 	"P09_SensorAnalogico_All.h"

//The setup function is called once at startup of the sketch
void setup()
{
	//Iniciamos Modbus en Modo RTU
	Init_RTU_Modbus();

	//Leer Configuración práctica actual
	load_Config();


	DEBUGLN(DEVICENAME(domoboard.BOTON1.name));
}

// The loop function is called in an endless loop
void loop()
{
	EXECUTELOOP(){
		UPDATELOOP();

		if(Aregs[MB_SELPRACT] < PRACTICAS_MODBUS){
			domoboard.leerAllSensor();
		}else mbDomoboard.leerAllSensor();

		RTU_ModBus();

		LOOP_x100ms{
			AccionesTemporizadas_Timer();
		}
	}
}
