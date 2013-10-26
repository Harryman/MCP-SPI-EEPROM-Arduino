// file generated using https://github.com/Harryman/Arduino-header-keyword-creator

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

#ifndef SPIPRM_h
#define SPIPRM_h

#include "Arduino.h"

class SPIPRM{
  public:
    SPIPRM(uint16_t pagesize, uint32_t totalsize, uint8_t cspin);
    void SPIset();
    void writeStart(uint16_t addr);
    void writeData(uint8_t data);
    void endCmd();
    void write16(uint16_t data);
    void readStart(uint16_t addr);
    uint8_t readData();
    uint16_t read16();
    void wren() ;
    void rdid();
    void rdsr();

    uint16_t lastPage;
    uint32_t pointer;
    //????ret;

  private:

    uint16_t _pagesize;
    uint32_t _highestaddr;
    uint8_t _cspin;

};

#endif