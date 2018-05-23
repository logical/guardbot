

//This example show how you can get Authenticated by the NTAG213,215,216 by default the tags are unprotected in order to protect them we need to write 4 different values:
// Using mfrc522.MIFARE_Ultralight_Write(PageNum, Data, #Databytes))
//1.- we need to write the 32bit passWord to page 0xE5 !for ntag 213 and 215 page is different refer to nxp documentation!
//2.- Now Write the 16 bits pACK to the page 0xE6 use the 2 high bytes like this: pACKH + pACKL + 00 + 00 after an authentication the tag will return this secret bytes
//3.- Now we need to write the first page we want to protect this is a 1 byte data in page 0xE3 we need to write 00 + 00 + 00 + firstPage  all pages after this one are write protected
// Now WRITE protection is ACTIVATED so we need to get authenticated in order to write the last data
//4.- Finally we need to write an access record in order to READ protect the card this step is optional only if you want to read protect also write 80 + 00 + 00 + 00 to 0xE4
//After completeing all these steps you will nee to authentiate first in order to read or write ant page after the first page you selected to protect
//To disengage proection just write the page (0xE3) to 00 + 00 + 00 + FF that going to remove all protection
//Made by GARGANTUA from RoboCreators.com & paradoxalabs.com

#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>

#define RST_PIN  PA3     // Configurable, see typical pin layout above
#define SS_PIN  PA4     // Configurable, see typical pin layout above
#define IRQ_PIN PA2

MFRC522Extended mfrc522(SS_PIN, RST_PIN, &SPI,SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE0));   // Create MFRC522 instance
/**
 * Dumps memory contents of a MIFARE Ultralight PICC.
 */
void PICC_DumpMifareUltralightToSerial1() {
	MFRC522::StatusCode status;
	byte byteCount;
	byte buffer[18];
	byte i;
	char t[16];
	//Serial.println(F("Page  0  1  2  3"));
	// Try the mpages of the original Ultralight. Ultralight C has more pages.
		byte page=4;
		// Read pages
		byteCount = sizeof(buffer);
		status = mfrc522.MIFARE_Read(page, buffer, &byteCount);
		if (status != MFRC522::STATUS_OK) {
			Serial1.print(F("MIFARE_Read() failed: "));
			Serial1.println(MFRC522::GetStatusCodeName(status));
			return;//break;
		}
		// Dump data
		byte offset = 3;

		for (byte index = 0; index < 4; index++) {
			i = 4 * offset + index;
			sprintf(&t[index],"%c",buffer[i]);

		}

		page=8;
		// Read pages
		byteCount = sizeof(buffer);
		status = mfrc522.MIFARE_Read(page, buffer, &byteCount);
		if (status != MFRC522::STATUS_OK) {
			Serial1.print(F("MIFARE_Read() failed: "));
			Serial1.println(MFRC522::GetStatusCodeName(status));
			return;//break;
		}
		// Dump data
		offset = 0;



		for (byte index = 0; index < 4; index++) {
			i = 4 * offset + index;
/*			if(buffer[i] < 0x10)
				Serial1.print(F(" 0"));
			else
				Serial1.print(F(" "));
*/
			sprintf(&t[4+index],"%c",buffer[i]);

		}
		t[9]='\0';
		Serial1.print(t);
} // End PICC_DumpMifareUltralightToSerial()



void disableAllPeripheralClocks() {

    rcc_clk_disable(RCC_ADC1);
    rcc_clk_disable(RCC_ADC2);
    rcc_clk_disable(RCC_ADC3);
    rcc_clk_disable(RCC_AFIO);
    rcc_clk_disable(RCC_BKP);
    rcc_clk_disable(RCC_CRC);
    rcc_clk_disable(RCC_DAC);
   rcc_clk_disable(RCC_DMA1);
    rcc_clk_disable(RCC_DMA2);
    rcc_clk_disable(RCC_FLITF);
    rcc_clk_disable(RCC_FSMC);
    rcc_clk_disable(RCC_GPIOB);
    rcc_clk_disable(RCC_GPIOC);
    rcc_clk_disable(RCC_GPIOD);
    rcc_clk_disable(RCC_GPIOE);
    rcc_clk_disable(RCC_GPIOF);
    rcc_clk_disable(RCC_GPIOG);
    
    rcc_clk_disable(RCC_I2C1);
    rcc_clk_disable(RCC_I2C2);
    rcc_clk_disable(RCC_PWR);
    rcc_clk_disable(RCC_SPI2);
    rcc_clk_disable(RCC_SPI3);
    rcc_clk_disable(RCC_SRAM);

    rcc_clk_disable(RCC_TIMER2);
    rcc_clk_disable(RCC_TIMER3);
    rcc_clk_disable(RCC_TIMER4);
    rcc_clk_disable(RCC_TIMER5);
    rcc_clk_disable(RCC_TIMER6);

    rcc_clk_disable(RCC_TIMER7);
    rcc_clk_disable(RCC_TIMER8);
    rcc_clk_disable(RCC_TIMER9);
    rcc_clk_disable(RCC_TIMER10);
    rcc_clk_disable(RCC_TIMER11);
    rcc_clk_disable(RCC_TIMER12);
    rcc_clk_disable(RCC_TIMER13);
    rcc_clk_disable(RCC_TIMER14);

    rcc_clk_disable(RCC_USART2);
    rcc_clk_disable(RCC_USART3);
    rcc_clk_disable(RCC_UART4);
    rcc_clk_disable(RCC_UART5);
    rcc_clk_disable(RCC_USB);

}

void setGPIOModeToAllPins(gpio_pin_mode mode) {
  for(uint8_t i = 0; i < 16; i++) {
    gpio_set_mode(GPIOA, i, mode);
    gpio_set_mode(GPIOB, i, mode);
    gpio_set_mode(GPIOC, i, mode);
  }
}

void setup() {
  setGPIOModeToAllPins(GPIO_INPUT_ANALOG);  
  adc_disable_all();
  disableAllPeripheralClocks();

  Serial1.begin(9600);
  Serial1.write("AT+DEFAULT\r\n");
  Serial1.write("AT+RESET\r\n");
  Serial1.write("AT+NAME=Controller\r\n");
  Serial1.write("AT+ROLE\r\n");
  Serial1.write("AT+TYPE1"); //Simple pairing


  SPI.begin();      // Init SPI bus
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  mfrc522.PCD_Init();   // Init MFRC522  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

}

void loop() {

    delay(300);
    digitalWrite(RST_PIN,true);
    mfrc522.PCD_SoftPowerUp();
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      mfrc522.PCD_SoftPowerDown();
      digitalWrite(RST_PIN,false);
      return;
    }
  
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      mfrc522.PCD_SoftPowerDown();
      digitalWrite(RST_PIN,false);
      return;
    }
  
  
    mfrc522.PICC_DumpMifareUltralightToSerial1(); //This is a modifier dunp just cghange the for cicle to < 232 instead of < 16 in order to see all the pages on NTAG216//
    mfrc522.PICC_HaltA();
    mfrc522.PCD_SoftPowerDown();
    digitalWrite(RST_PIN,false);
}
