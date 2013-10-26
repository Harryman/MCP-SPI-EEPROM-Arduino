/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ The Everything Corp
/ Inspired by Eric Brundick <spirilis@linux.com> https://bitbucket.org/spirilis/spieep/overview
/ If you find this useful please pitch in: 1Coaxrz39TwnPkuQDDioDimNbMbKKoeQEV 
 ^^^^^^^^^^Include when copying^^^^^^^^^^^^^^

/ This is a really bare bones library that allows you to use all of the base
 functionality of the MCP25LC512, You will have to do some modification to make
 it work with others in the series. This library was written to support a product with 
 datalogging and play back so it is optimised to be read and written in a consecutive 
 manner so it may not be that useful to everyone's needs for more general purpose library
 I suggest you check out the one linked above which I used to help create this library.
*/

#include <SPIPRM.h>
#include <SPI.h>

// Microchip 25LC512 SPI EEPROM Instruction Set
#define READ 0x03 
#define WRITE 0x02
#define WREN 0x06 //write enable
#define WRDI 0x04 //write disable
#define RDSR 0x05 //read status register
#define WRSR 0x01 //write status register
#define PE 0x42   //page erase
#define SE 0xD8   //sector erase
#define CE 0xC7   //chip erase 199
#define RDID 0xAB //release from deep power down 171
#define DPD 0xB9  //deep power down mode 185

// Microchip 25LC512 SPI EEPROM Status Register Bits
#define SPIPRM_STATUS_WPEN 7
#define SPIPRM_STATUS_BP1 3
#define SPIPRM_STATUS_BP0 2
#define SPIPRM_STATUS_WEL 1
#define SPIPRM_STATUS_WIP 0



//page size in bytes, size in bytes, chip select pin number
SPIPRM::SPIPRM(uint16_t pagesize, uint32_t totalsize, uint8_t cspin){
  _pagesize = pagesize;
  _highestaddr = totalsize  - 1;
  _cspin = cspin;
  pinMode(_cspin, OUTPUT);
  digitalWrite(_cspin, HIGH);  // CS pin is high when idle.
  SPI.begin();
  SPIset();
  digitalWrite(_cspin, LOW);
  SPI.transfer(WREN);
  digitalWrite(_cspin, HIGH);
}

void SPIPRM::SPIset(){
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0); 
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void SPIPRM::writeStart(uint16_t addr){
  wren();
  lastPage = addr>>3;
  pointer = addr;
  digitalWrite(_cspin, LOW);
  SPI.transfer(WRITE);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr & 255);
}

void SPIPRM::writeData(uint8_t data){
  if(pointer > 65535){
    pointer = 0;
  }
  if(lastPage != pointer >> 3){
    endCmd();
    writeStart(pointer);
    lastPage = pointer >>3;
  }
  SPI.transfer(data);
  pointer++;
}

void SPIPRM::endCmd(){
  digitalWrite(_cspin, HIGH);
}

void SPIPRM::write16(uint16_t data){
  writeData(data >> 8);
  writeData(data & 255);
}

void SPIPRM::readStart(uint16_t addr){
  pointer = addr;
  digitalWrite(_cspin,LOW);
  SPIset();
  SPI.transfer(READ);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr & 255);
}

uint8_t SPIPRM::readData(){
  uint8_t ret;
  if(pointer > 65535){
    pointer = 0;
  }
  ret = SPI.transfer(0);
  pointer++;
  return ret;
}

uint16_t SPIPRM::read16(){
  uint16_t ret;
  ret = readData();
  ret = ret<<8;
  ret |= readData();
  return ret;
}

void SPIPRM::wren() {
  digitalWrite(_cspin, LOW);
    SPI.transfer(WREN);
  digitalWrite(_cspin, HIGH);
}

void SPIPRM::rdid(){
  digitalWrite(_cspin, LOW);
  SPI.transfer(RDID);
  digitalWrite(_cspin, HIGH);
}

void SPIPRM::rdsr(){
  digitalWrite(_cspin, LOW);
  SPI.transfer(RDSR);
  SPI.transfer(0);
  digitalWrite(_cspin, HIGH);

}