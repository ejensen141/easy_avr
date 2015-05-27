# Easy AVR
Lightweight yet easy to use Library for AVR microcontrollers


In the name of speed, many of the functions are defined as inline void, and will execute just as fast as if you wrote the bits "by hand".



## Interfaces

```c++

spi _spi;

_spi.write(0xff);


uint8_t value = 0xff;
usart0 _serial;

_serial.write(value);


```


## Bit Set Functions

These Functions are defined as Macros, meaning they will be faster than their arduino counterparts.
```c++

 setBitHigh(port,bit);

 setBitLow(port,bit);

 readBit(port,bit);

 setInput(port,bit);

 setOutput(port,bit);

 toggle(port,bit);

```
