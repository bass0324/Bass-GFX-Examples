My configuration

STM32F4-Discovery + LCD HY32D

HY32D: SSD1289 (LCD Controller) + TSC2046 (touch controller)

* LCD Connections

- GPIO

DB[0-3]  -> PD[0-3]
DB[4-15] -> PE[4-15]
CS       -> PD7
WR       -> PD8
RD       -> PD9
RESET    -> PD10
RS       -> PD11

- FSMC

D0    -> PD14
D1    -> PD15
D2    -> PD0
D3    -> PD1
D4    -> PE7
D5    -> PE8
D6    -> PE9
D7    -> PE10
D8    -> PE11
D9    -> PE12
D10   -> PE13
D11   -> PE14
D12   -> PE15
D13   -> PD8
D14   -> PD9
D15   -> PD10
CS    -> PD7
RS    -> PD11
RD    -> PD4
WR    -> PD5
RESET -> PD6

* TouchScreen

TS IRQ   -> PC4
TS CS    -> PC6

// SPI2 STM32F407
TS SCK   -> PB13
TS MISO  -> PB14
TS MOSI  -> PB15

