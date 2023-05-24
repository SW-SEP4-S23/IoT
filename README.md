# IoT

## Interface

Max payload size is at 20 bytes excluding the header.

The header is at 20 bytes. It is not sent with downlink, only uplink.  
Sending more than two uplinks in the span of 5 minutes will severely increase the next  
uplink message delay

Total of 20 bytes can be sent up or back at once.
Currently using 10.

### Uplink - 4 bytes
| Type  | Description | Position | Size   |
| ----- | ----------- | -------- | ------ |
| Float | Temp C      | 0        | 1 byte |
| Float | CO2 % ppm   | 1        | 1 byte |
| Float | Hum RH %    | 2        | 1 byte |
| int   | ID          | 3        | 1 byte |

### Downlink - 6 bytes
| Type  | Description | Position | Size   |
| ----- | ----------- | -------- | ------ |
| Float | TempMax     | 0        | 1 byte |
| Float | CO2Max      | 1        | 1 byte |
| Float | HumMax      | 2        | 1 byte |
| Float | TempMin     | 3        | 1 byte |
| Float | CO2Min      | 4        | 1 byte |
| Float | HumMin      | 5        | 1 byte |
| int   | ID          | 6        | 1 byte |

## Setup

When setting up the project it is crucial that one initialises the submodules after cloning the project.

### 1

```git
git clone https://github.com/SW-SEP4-S23/IoT.git
```

### 1

```git
git submodule init
```

### 2

```git
git submodule update
```

These steps must be gone through **before** opening the software in **Atmel Studio 7**.

## Atmel studio

To open the project, all ready for programming, in atmel studio, choose the .atsln project file as shown in the picture below.

![example](docs/pic/exampleImg.jpg)

## 7-segment display status codes:

Theese codes are displayed in the 7 segment display, when the folowing action is performed

1. When temperature is raised
2. When temperature is lowered
3. C02 raise
4. C02 lowered
5. Sending data
6. Reciving data
