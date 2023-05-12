# IoT

## Interface

Max payload size is at 20 bytes EXCLUDING the header.

Total of 20 bytes can be sent up or back at once.

### Uplink

| Type        | Description       | Position    | Size        |
| ----------- | ----------------- | ----------- | ----------- |
| Float       | Temperature       | 0           | 1 byte      |
| Float       | CO2               | 1           | 1 byte      |
| Float       | Humidity          | 2           | 1 byte      |
| Float       | Id                | 3           | 1 byte      |
| **TOTAL:    |                   |             | 4 byte

### Downlink

| Type        | Description           | Position    | Size        |
| ----------- | -----------------     | ----------- | ----------- |
| Float       | Max Temperature       | 0           | 1 byte      |
| Float       | Min Temperature       | 1           | 1 byte      |
| Float       | Max C02               | 2           | 1 byte      |
| Float       | Min C02               | 3           | 1 byte      | 
| Float       | Max Humidity          | 4           | 1 byte      | 
| Float       | Min Humidity          | 5           | 1 byte      |
| Float       | Id                    | 6           | 1 byte      | 
| **TOTAL:    |                       |             | 7 byte      | 

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

