# Autonomous Vehicle Simulation

A simulation of a self-driving car on a 2D grid, built as a university project for an Object-Oriented Programming course. The car reads its surroundings with three sensors, combines their readings and drives to a list of GPS targets. On the way it slows down for other cars, bikes and red lights, and stops at STOP signs.

**Tech:** C++17, Make (no external libraries)

## Features

- Three sensors with different range and accuracy: LiDAR (9 cells, 99%), radar (12 cells straight ahead, moving objects only) and camera (7 cells, recognizes signs and light colours)
- All sensors inherit from an abstract `Sensor` class and override `detect()`
- Sensor fusion: readings of the same object are combined with a weighted average by confidence. Objects with low confidence are ignored, except bikes
- The car slows down for cars and bikes within 2 cells and for red or yellow lights within 3 cells
- It stops once at each STOP sign and then continues
- It turns towards the next GPS target on every step
- Random world with moving cars, bikes, parked cars, traffic lights and STOP signs. The same seed gives the same world
- The grid and the area around the car are printed in the terminal on every step

## Setup

You need a C++17 compiler (g++ or clang++) and `make`.

```bash
make                 # builds ./oopproj_2025
make run-example     # runs an example with a fixed seed
make help            # shows all options
```

Example:

```bash
./oopproj_2025 --seed 42 --dimX 40 --dimY 40 --numMovingCars 5 --gps 10 10 30 30 --noDelay
```

| Option | Description | Default |
|---|---|---|
| `--gps x1 y1 [x2 y2 ...]` | GPS targets (required) | – |
| `--seed n` | Random seed | current time |
| `--dimX n`, `--dimY n` | World size | 40 × 40 |
| `--numMovingCars n`, `--numMovingBikes n` | Moving cars and bikes | 3, 4 |
| `--numParkedCars n`, `--numStopSigns n`, `--numTrafficLights n` | Parked cars, STOP signs, traffic lights | 5, 2, 2 |
| `--simulationTicks n` | Maximum number of steps | 100 |
| `--minConfidenceThreshold x` | Minimum confidence for sensor fusion | 0.4 |
| `--enableLogging` | Saves the car's position on every step to `oopproj_2025.log` | off |
| `--fullVisualization` | Prints the whole grid on every step | off |
| `--noDelay` | Runs without the 0.5 s pause between steps | off |

## How it works

The car starts in the middle of the grid. On every step it:

1. collects readings from the three sensors
2. combines them in `SensorFusionEngine`
3. decides what to do in `NavigationSystem` (slow down, stop, turn, speed up, move)
4. runs those commands in `SelfDrivingCar`

The car has three speeds: stopped, half speed (1 cell per step) and full speed (2 cells per step). The simulation ends when the car reaches the last GPS target or the steps run out.

## Project structure

```
include/   header files
src/       source files
Makefile
```

```
WorldObject
├── StaticObject
│   ├── TrafficLight
│   └── TrafficSign
├── MovingObject
└── SelfDrivingCar

Sensor
├── LidarSensor
├── RadarSensor
└── CameraSensor
```
