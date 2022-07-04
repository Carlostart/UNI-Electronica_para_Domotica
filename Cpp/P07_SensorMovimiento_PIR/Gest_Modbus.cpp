/*
 * Gest_Modbus.cpp
 *
 *  Created on: 16/05/2014
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	<Arduino.h>
#include	"Gest_Modbus.h"
#include 	"utils_domoBoard.h"
#include	"config_practicas.h"

/****************************************************************************/
/***        Variables Locales                                             ***/
/****************************************************************************/
/* First step MBS: create an instance */
ModbusSlave mbs;

/****************************************************************************/
/***                 Definici�n de funciones                              ***/
/****************************************************************************/
void writecoil(unsigned int addrReg);
void writeholdingregister(unsigned int addrReg);

/****************************************************************************/
/***                 Functions                                            ***/
/****************************************************************************/
void Init_RTU_Modbus()
{
	/* configure modbus communication
	 * 19200 bps, 8E1, two-device network */
	/* Second step MBS: configure */
	/* the Modbus slave configuration parameters */
	const unsigned char 	SLAVE 	= ADDR_SLAVE;		//Address SLAVE
	const long 				BAUD 	= SERIAL_BPS;
	const char 				PARITY 	= SERIAL_PARITY;
	const char 				TXENPIN = 0; //EN_485;

	//Inicialmente configuramos 485 para recibir
	if(TXENPIN != 0)
		digitalWrite(EN_485, LOW);
	else digitalWrite(EN_485, HIGH);

	//Para la conexi�n 485/ModBus usamos
	Serial485 = &Serial;

	//We configure the ModBus Register Banks
	mbs.set_BankCoils(Cregs, MB_O_COILS);
	mbs.set_BankDiscreteInputs(Dregs, MB_I_CONTATCS);
	mbs.set_BankOutputHoldingRegister(Aregs, MB_A_REGS);

	mbs.configure(SLAVE,BAUD,PARITY,TXENPIN);

	//Sets up event that occurred when Output Discrete register are updated
	mbs.writecoil = writecoil;
	mbs.writeholdingregister = writeholdingregister;
}


/*
 *
 */

void writecoil(unsigned int addrReg){
	switch(addrReg){
	case	MB_RELE:
		domoboard.setActuator(&domoboard.RELE,Cregs[MB_RELE]!=0x00);
		break;

	case	MB_TRIAC:
		domoboard.setActuator(&domoboard.TRIAC,Cregs[MB_TRIAC]!=0x00);
			break;

	case   MB_ACTPIR:
		mbDomoboard.PIR_MOV.Sensor->Activo = Cregs[addrReg]!=0x00;
		break;

	}
}

/*
 *
 */

void writeholdingregister(unsigned int addrReg){
	//char msg[50];

	switch(addrReg){
	case MB_SELPRACT:
		//Go To Selecction Configuration
		SelectionConfiguration(Aregs[MB_SELPRACT]&0xFF);
		break;
	}
}


void RTU_ModBus()
{
	unsigned long wdog = 0;         /* watchdog */

	if(mbs.update()){
		wdog = millis();

		if ((millis() - wdog) > 5000)  {      // no comms in 5 sec
			//regs[MB_CTRL] = 0;	// turn off led
		}
	}
}
