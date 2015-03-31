Skeleton projects, linker scripts etc for working with obscure micro controllers. By obscure I basically mean anything that isn't an Atmega186 based board with silly header spacing :). I have a fetish for machines that aren't well known outside of Japan like the H8 and SuperH and for classics like the Motorola M68K. That is the sort of stuff that will be featured here..

## Machines/Archs I'm using and stuff that is currently available for each ##
  * Generic stuff
    * JJY time decoder (Untested with a real receiver as the one I got didn't work)
    * Some stuff to work with Intel flash chips.
  * Z80 http://akizukidenshi.com/catalog/g/gK-00123/
  * H8/3069F [aki-3069f](http://akizukidenshi.com/catalog/g/gK-02298/) [Software Manual](http://documentation.renesas.com/eng/products/mpumcu/rej09b0213_h8300h.pdf)
    * skeleton project for newlib running on top of the excellent [h8mon](http://www.shudoshiki.net/electronics/h8mon/h8mon-0.90.html)
  * LPC2294 (ARM7TDMI) - Olimex LPC-H2294
    * Basic skeleton for newlib
    * Example of skeleton + lwip for TCP/IP via SLIP on uart1
  * 68sec000 (Basically 68000)
  * SH7216 (SH2-A)
  * SH7751R (SH4)
    * Some stuff for Linux on the Landisk machines from IODATA.
  * RX62N
  * SAM9G45 (Atmel ARMv5)
    * [My Linux tree](http://code.google.com/p/linux-picosam9g45/)

## Machines/Archs I would like to get my hands on ##

Some of the other now Renesas kit.. like R8C
Weird Toshiba M68K MCUs.

## Places to get boards ##

For H8,SH2,RX,R8C and some nice looking Z80 boards [Akizuki Denshi Tsusho](http://akizukidenshi.com/) is the place to go. They only ship within Japan though. Renesas dev kits turn up on ebay but people think they can get the Digikey price for them, so they aren't cheap. I have haggled and got SH2-A and RX RDKs for $60 though. So it's worth haggling :)

For ARM (of different vendors) the best boards are from [Olimex](http://olimex.com/dev/index.html). Note that their boards aren't cheap and documentation is really lacking.. The build quality is a lot better than anything you will find on ebay however.