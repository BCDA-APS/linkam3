/******************************************************************************
* Linkam Scientific Instruments
* 2018-10-16
* Copyright © 2018 Linkam Scientific Instruments
*
* Usage:
*       This is an example program to test basic communications connection
*       to the controller and ability to get the primary heater value.
*
*       This program may be freely used as a learning tool and as a basis
*       to develop more advanced applications.
*
******************************************************************************/
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "LinkamSDK.h"
#include <sys/time.h>

/*!
 *  \struct     ProgramArguments
 *  \brief      A simple structure to contain command line parameters for
 *              this example application.
 */
struct ProgramArguments
{
	bool            bUSB;           // Type of connection required: true for USB, else false for serial.
	unsigned int    vendorID;       // USB setting: The Linkam vendor ID (0x16DA).
	unsigned int    productID;      // USB setting: The product ID, typically set to discover mode (0x0001).
	unsigned int    baudrate;       // Serial setting: The seed of the serial connection.
	unsigned int    bytesize;       // Serial setting: How big the serial byte is, must be 8 bits.
	unsigned int    flowcontrol;    // Serial setting: Type of flow control.
	unsigned int    parity;         // Serial setting: Use of the parity bit.
	unsigned int    stopbits;       // Serial setting: Number of bits to indicate a stop.
	std::string     port;           // Serial setting: Name of the serial port to connect to.

	ProgramArguments()
	{
		bUSB = false;
		vendorID = 0;
		productID = 0;
		baudrate = 0;
		bytesize = 0;
		flowcontrol = 0;
		parity = 0;
		stopbits = 0;
		port = std::string("COM1");
	}
};

/*!
 *  \brief      This function handles the initialisation of a Serial connection to
 *              a Linkam controller.
 *  \param[in]  handle          A reference to a CommsHandle to receive the controller handle generated by the CommsDriver.
 *  \param[in]  args            A copy of the ProgramArguments structure that holds the command line information.
 *  \param[in]  result          A reference to a LinkamSDK::Variant to receive the connection result.
 *  \return     Returns true if connected, else false.
 */
bool initSerialDevice(CommsHandle& handle, LinkamSDK::Variant& result)
{
	LinkamSDK::CommsInfo info;
	LinkamSDK::Variant param1;
	LinkamSDK::Variant param2;
	bool connected = false;

	linkamInitialiseSerialCommsInfo(&info, "/dev/ttyUSB0");

	param1.vPtr = &info;
	param2.vPtr = &handle;

	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_OpenComms, 0, &result, param1, param2);
	if (result.vConnectionStatus.flags.connected) {
		printf("We got a connection to the Serial device!\n");
		connected = true;
	} else {
		printf( "Error openning connection:\n\nstatus.connected = %d\nstatus.flags.errorAllocationFailed = %d\nstatus.flags.errorAlreadyOpen = %d\nstatus.flags.errorCommsStreams = %d\nstatus.flags.errorHandleRegistrationFailed = %d\nstatus.flags.errorMultipleDevicesFound = %d\nstatus.flags.errorNoDeviceFound = %d\nstatus.flags.errorPortConfig = %d\nstatus.flags.errorPropertiesIncorrect = %d\nstatus.flags.errorSerialNumberRequired = %d\nstatus.flags.errorTimeout = %d\nstatus.flags.errorUnhandled = %d\n\n",
				result.vConnectionStatus.flags.connected,
				result.vConnectionStatus.flags.errorAllocationFailed,
				result.vConnectionStatus.flags.errorAlreadyOpen,
				result.vConnectionStatus.flags.errorCommsStreams,
				result.vConnectionStatus.flags.errorHandleRegistrationFailed,
				result.vConnectionStatus.flags.errorMultipleDevicesFound,
				result.vConnectionStatus.flags.errorNoDeviceFound,
				result.vConnectionStatus.flags.errorPortConfig,
				result.vConnectionStatus.flags.errorPropertiesIncorrect,
				result.vConnectionStatus.flags.errorSerialNumberRequired,
				result.vConnectionStatus.flags.errorTimeout,
				result.vConnectionStatus.flags.errorUnhandled
		);
	}

	return connected;
}

/*!
 *  \brief      This function provides the main program functionality, which is to get temperature from the stage.
 *  \param[in]  handle          A reference to a CommsHandle to receive the controller handle generated by the CommsDriver.
 *  \param[in]  result          A reference to a LinkamSDK::Variant to receive result information.
 */
void proc(CommsHandle& handle, LinkamSDK::Variant& result)
{
	LinkamSDK::Variant param1;
	LinkamSDK::Variant param2;
	char name[1024] = { 0 };
	char serial[1024] = { 0 };

	result.vUint64 = 0;

	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetStatus, handle, &result);
	printf("ControllerStatus.flags.controllerError               = %d\n", result.vControllerStatus.flags.controllerError);
	printf("ControllerStatus.flags.heater1RampSetPoint           = %d\n", result.vControllerStatus.flags.heater1RampSetPoint);
	printf("ControllerStatus.flags.heater1Started                = %d\n", result.vControllerStatus.flags.heater1Started);
	printf("ControllerStatus.flags.heater2RampSetPoint           = %d\n", result.vControllerStatus.flags.heater2RampSetPoint);
	printf("ControllerStatus.flags.heater2Started                = %d\n", result.vControllerStatus.flags.heater2Started);
	printf("ControllerStatus.flags.vacuumRampSetPoint            = %d\n", result.vControllerStatus.flags.vacuumRampSetPoint);
	printf("ControllerStatus.flags.vacuumCtrlStarted             = %d\n", result.vControllerStatus.flags.vacuumCtrlStarted);
	printf("ControllerStatus.flags.vacuumValveClosed             = %d\n", result.vControllerStatus.flags.vacuumValveClosed);
	printf("ControllerStatus.flags.vacuumValveOpen               = %d\n", result.vControllerStatus.flags.vacuumValveOpen);
	printf("ControllerStatus.flags.humidityRampSetPoint          = %d\n", result.vControllerStatus.flags.humidityRampSetPoint);
	printf("ControllerStatus.flags.humidityCtrlStarted           = %d\n", result.vControllerStatus.flags.humidityCtrlStarted);
	printf("ControllerStatus.flags.lnpCoolingPumpOn              = %d\n", result.vControllerStatus.flags.lnpCoolingPumpOn);
	printf("ControllerStatus.flags.lnpCoolingPumpAuto            = %d\n", result.vControllerStatus.flags.lnpCoolingPumpAuto);
	printf("ControllerStatus.flags.HumidityDesiccantConditioning = %d\n", result.vControllerStatus.flags.HumidityDesiccantConditioning);
	printf("ControllerStatus.flags.motorTravelMinX               = %d\n", result.vControllerStatus.flags.motorTravelMinX);
	printf("ControllerStatus.flags.motorTravelMaxX               = %d\n", result.vControllerStatus.flags.motorTravelMaxX);
	printf("ControllerStatus.flags.motorStoppedX                 = %d\n", result.vControllerStatus.flags.motorStoppedX);
	printf("ControllerStatus.flags.motorTravelMinY               = %d\n", result.vControllerStatus.flags.motorTravelMinY);
	printf("ControllerStatus.flags.motorTravelMaxY               = %d\n", result.vControllerStatus.flags.motorTravelMaxY);
	printf("ControllerStatus.flags.motorStoppedY                 = %d\n", result.vControllerStatus.flags.motorStoppedY);
	printf("ControllerStatus.flags.motorTravelMinTST             = %d\n", result.vControllerStatus.flags.motorTravelMinZ);
	printf("ControllerStatus.flags.motorTravelMaxTST             = %d\n", result.vControllerStatus.flags.motorTravelMaxZ);
	printf("ControllerStatus.flags.motorStoppedTST               = %d\n", result.vControllerStatus.flags.motorStoppedZ);
	printf("ControllerStatus.flags.sampleCal                     = %d\n", result.vControllerStatus.flags.sampleCal);
	printf("ControllerStatus.flags.motorDistanceCalTST           = %d\n", result.vControllerStatus.flags.motorDistanceCalTST);
	printf("ControllerStatus.flags.cssRotMotorStopped            = %d\n", result.vControllerStatus.flags.cssRotMotorStopped);
	printf("ControllerStatus.flags.cssGapMotorStopped            = %d\n", result.vControllerStatus.flags.cssGapMotorStopped);
	printf("ControllerStatus.flags.cssLidOn                      = %d\n", result.vControllerStatus.flags.cssLidOn);
	printf("ControllerStatus.flags.cssRefLimit                   = %d\n", result.vControllerStatus.flags.cssRefLimit);
	printf("ControllerStatus.flags.cssZeroLimit                  = %d\n", result.vControllerStatus.flags.cssZeroLimit);

	result.vUint64 = 0;
	param1.vPtr = serial;
	param2.vUint32 = 1024;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetControllerSerial, handle, &result, param1, param2);
	printf("Controller Serial = %s\n", serial);

	result.vUint64 = 0;
	param1.vPtr = name;
	param2.vUint32 = 1024;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetControllerName, handle, &result, param1, param2);
	printf("Controller Name = %s\n", name);

	result.vFloat32 = 0;
	param1.vStageValueType = LinkamSDK::eStageValueTypeHeater1Limit;
	param2.vUint32 = 0;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetMaxValue, handle, &result, param1, param2);
	printf("Heater1Limit MAX = %f\n", result.vFloat32);

	result.vFloat32 = 0;
	param1.vStageValueType = LinkamSDK::eStageValueTypeHeater1Limit;
	param2.vUint32 = 0;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetMinValue, handle, &result, param1, param2);
	printf("Heater1Limit MIN = %f\n", result.vFloat32);

	printf("Setting limit (%d) to 40 deg... ", LinkamSDK::eStageValueTypeHeater1Limit);
	param1.vStageValueType = LinkamSDK::eStageValueTypeHeater1Limit;
	param2.vFloat32 = 40.0;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_SetValue, handle, &result, param1, param2);
	if (result.vBoolean)
		printf(" successful!\n");
	else
		printf(" failed.\n");

	param1.vBoolean = true;
	//linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_StartHeating, handle, &result, param1, param2);
	printf("Started heating (%d)\n", result.vBoolean);
	// Get 200 seconds of temperature data at a rate of 2 requests per second.
	unsigned int count = 400;
	while (count > 0)
	{
		struct timeval before, after;
		gettimeofday(&before, NULL);
		result.vUint64 = 0;
		param1.vStageValueType = LinkamSDK::eStageValueTypeHeater1Temp;
		param2.vUint32 = 0;
		linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_GetValue, handle, &result, param1, param2);
		gettimeofday(&after, NULL);
		printf("Heater 1 = %.3f\n", result.vFloat32);
		printf("Time delta %.3f\n", (double)((after.tv_sec-before.tv_sec)*1000000 + after.tv_usec-before.tv_usec));

		usleep(500000);
		--count;
	}
	param1.vBoolean = false;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_StartHeating, handle, &result, param1, param2);
	printf("Stop heating (%d)\n", result.vBoolean);

	result.vUint64 = 0;
	linkamProcessMessage(LinkamSDK::eLinkamFunctionMsgCode_CloseComms, handle, &result);
	if (result.vBoolean)
		printf("Device handle closed successfully!\n");
	else
		printf("Failed to close device handle!\n");
}

/*!
 *  \brief      Entry point function, this starts the program.
 *  \param[in]  argc    The number of command line arguments to parse.
 *  \param[in]  argv    An array of char pointers to the individual command line strings.
 *  \return     Returns zero.
 */
int main(int argc, char* argv[])
{
	LinkamSDK::Variant result;
	CommsHandle handle = 0;

	if (linkamInitialiseSDK()) {
		printf("We have just initialised the LinkamSDK!\n");

		char version[256];
		if (linkamGetVersion(version, 256)) {
			printf("LinkamSDK, version %s\n", version);

			if (initSerialDevice(handle, result)) {
				proc(handle, result);
			}
		} else {
			printf("Something went wrong attempting to get the version number of the LinkamSDK!\n");
		}
	} else {
		printf("Something went wrong attempting to initialise the LinkamSDK!\n");
	}

	linkamExitSDK();
	printf("Exited the LinkamSDK!\n");
	printf("Good bye\n");

	return 0;
}

