#include "intelliSense.h"	// for IntelliSense
#include "mbed.h"
#include <vector>
#include "TextLCD.h"		// configure LCD at TextLCD_Config.h

// set baudrate at mbed_config.h default 115200
// I2C scanner included, derived from Arduino I2C scanner
// http://www.gammon.com.au/forum/?id=10896

#define I2C1_SDA  PB_9
#define I2C1_SCL  PB_8 

// Host PC Communication
Serial pc(SERIAL_TX, SERIAL_RX);

// LCD I2C Communication
I2C i2c(I2C1_SDA, I2C1_SCL);

uint16_t lcdAddr = 0X3F;   // LCD 2004 address determined as 0X3F in 16-bit
TextLCD_I2C lcd(&i2c, lcdAddr << 1, TextLCD::LCD20x4);

void I2C_scan(); 

int main()
{
	// I2C Scanner.. comment out if not used... 
	//I2C_scan();
	
	// LCD Initialize
	lcd.cls();
	lcd.setMode(TextLCD_Base::LCDMode::DispOn);
	lcd.setBacklight(TextLCD::LCDBacklight::LightOn);

	lcd.printf("Hello World"); 

	// LCD Countdown shutdown
	lcd.locate(0, 1);
	lcd.printf("Shutdown in: ");
	for (size_t i = 10; i > 0; i--)
	{
		lcd.locate(13, 1); 
		lcd.printf("%d ", i); 
		wait(1); 
	}
	lcd.setPower(false); 
	lcd.setBacklight(TextLCD::LCDBacklight::LightOff); 
}

void I2C_scan()
{
	// Initializing I2C Scanner
	pc.printf("I2C Scanner initializing.... \n");
	uint8_t error, address;
	char mydata[2]; 
	mydata[1] = 0x00; 
	std::vector<uint8_t> addrList; 

	pc.printf("I2C Scanner start scanning. \n");

	// Start scanning using 8 bit address
	pc.printf("\n8-bit address.... \n"); 
	for (address = 1; address < 128; address++)
	{
		error = i2c.write(address, mydata, sizeof(mydata));					// error: 0 - success (ack), 1 - failure (nack)
		pc.printf("Address: %#X return %d\n", address, error);
		if (error == 0)
			addrList.push_back(address);	// add to addrList if address receive ack
	}
	for (auto &val : addrList)
	{
		pc.printf("Valid Address: %#X ", val); 		// print valid address
	}

	// Initialize and start scanning using 16-bit (8-bit shifted left) address
	addrList.clear(); 
	pc.printf("\n16-bit address.... \n");
	for (address = 1; address < 128; address++)
	{
		error = i2c.write((uint16_t)(address << 1), mydata, sizeof(mydata));  // error: 0 - success (ack), 1 - failure (nack)
		pc.printf("Address: %#X return %d\n", (uint16_t)(address << 1), error);
		if (error == 0)
			addrList.push_back(address);	// add to addrList if address receive ack
	}
	for (auto &val : addrList)
	{
		pc.printf("Valid Address: %#X ", val);		// print valid address
	}


}