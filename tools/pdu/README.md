# PDU test code

This test code is aiming to perform the unit test. Since this is for developers, not users, this is not user-friendly.

## How to use

First of all. you should compile pGramsFC software (see [documentation](../../docs/installation.md)) with option -DGB_USE_BAYCAT=ON and -DGB_USE_FT232H.

And then, you should compile test codes.

```[shell]
cd tools/pdu
mkdir build
cd build
cmake ..
make
```

If you want to read values from ADC, `./pdu_read <Baycat or FT232H>`. You can specify which device you want to use. And if you want to control DAC, you can use `./pdu_test <Baycat or FT232H>`. If you want to test enables, you can use `./enable_test <Baycat or FT232H>`.

## Modification of the code

If you want to change the chip select. You should modify the software.

In pdu_test.cc

```[cpp]
dac.setCS(1); // you can specify the chip select channel.
```

In pdu_read.cc, you can modify this part:

```[cpp]
adc.setCS(i);
```

These channels are expressed in Multiplexer channel.
After modification, you should recompile the pdu test codes.

In enable_test.cc, you can modify:

```[cpp]
const int chip_select = 0; // PLEASE MODIFY
const bool is_high = true; // If high, please specify true, otherwise false.
```

This channel is not multiplexer channel, just chip select channel (DIOx...).
