# Floor Cleaner

The project consists of a floor cleaner, which is controlled by a Desktop Application. The user can draw a map on this app and the robot will try to go over each available cell in the map.

---

## Desktop Application

The Desktop application has been developped in Python with the support of PyQT6 for the graphical interface.

### Map Representation
The map is represented by a grid of cells, each of these cover an area of 5cm * 5cm. There are 3 types of cells:

- UNAVAILABLE (grey in the grid): the cleaner should not go through it
- TO_CLEAN (blue in the grid): this cell must be cleaned in order to complete the cleaning procedure
- CLEANER_POSITION (black in the grid): the initial position of the cleaner

Initially, all cells are unavailable, but the user can mark them as to-clean by selecting them with a mouse press and release action. To place the cleaner in the map, search for the *Cleaner* label in the menu and then click on *Place Cleaner*. The next cell to be selected will be the cleaner position.

### Serial communication details
Once the map is ready, the user can connect the cleaner with an USB cable and go to *File* -> *Send* to start the sending procedure.

**Note**: it is important to connect the cleaner before opening the send dialog because it is initialized with the currently available ports:

```
from serial.tools import list_ports, list_ports_common

# ...

for i, serial_data in enumerate(list_ports.comports()):
    self._combo_box_serial.insertItem(
        i, serial_data.description, userData=serial_data)
```

After selecting the identifier for the Nucleo Board, the python app starts to transmit the bytes of the map with a baudrate of 9600.

### Map Transformation

The transmitted map is the smallest rectangle that includes all the to-clean cells in the grid. This means that there will never be a boundary row or column where all cells are unavailable.

Then the optimized grid is transformed into a matrix and each cell becomes a byte, in particular:

- UNAVAILABLE = 0x00
- TO_CLEAN = 0x01
- CLEANER_POSITION = 0x03

The 0x02 value does not exist yet but corresponds to the ALREADY_CLEANED value.

### Sending Protocol

In order for the map to be correctly interpreted by the Nucleo board, data are transmitted using a simple but quite efficient protocol. These are, in order, the sent bytes:

1. The character `&`, which represents the start (and the end) of the data flow
2. The number of rows in the map
3. The character `|`
4. The number of columns in the map
5. The character `|`
6. The map's cells, row by row from left to right
7. The character `&`

---

## Floor Cleaner

The cleaner will go through each to-clean cell in the received grid and avoids those that are marked as unavailable.

### Architecture and Components
The architecture is described by this Fritzing schema:

![Architecture](./presentation/floor-cleaner/public/images/cleaning_robot_architecture.png)

#### Power Source
The power source consists of 2 batteries of 9V each, linked in parallel in order to increase the amperage. Each battery has an amperage of 1000mAh. Unfortunately, I have not found a light battery with a voltage of 12V, which would have increased the amount of components I could have added to the circuit.

#### L298N

![L298N](./presentation/floor-cleaner/public/images/l298n.jpg)

This component is directly connected with the batteries and supplies power to:
- The 2 motors (wheels) using the output pins from OUT1 to OUT4
- The Nucleo board using the 5V pin, enabled via a jumper that regulates the input voltage

I can use the pins IN1-IN4 to control the motors direction, as described by the following table:

| IN1 | IN2 | Spinning direction |
| --- | --- | ------------------ |
| 0 | 0 | Motor OFF |
| 0 | 1 | Backward |
| 1 | 0 | Forward |
| 1 | 1 | Motor OFF |

ENA and ENB are used to modulate the voltage that the output ports OUT1-OUT4 receive. These pins are connected to 2 PWM enabled pins, which are A0 and A1 in the Nucleo board.

#### IR Distance Sensor

![IR distance sensor](./presentation/floor-cleaner/public/images/ir-distance-sensor.png)

The map is built by the user, but the cleaner might find some unpredictable obstacles on its way. To avoid any collisions, an IR distance sensor is used. This sensor has 3 pins:

- OUT: When a module detects an obstacle, the OUT port emits a low level signal that will be processed by the Nucleo board.
- GND
- VCC: this port is connected to the 3.3V pin of the Nucleo board

This component also has a potentiomenter that can adjust the IR sensor sensibility. Basically, it can detect obstacle from 2cm to 30cm, depending on the potentiometer value.

#### Buzzer

A buzzer informs the user about the cleaner's state:

- A single beep of 500ms: the cleaner is ready to receive a new map (and has successfully cleaned the previous map is present).
- 2 beeps of 500ms, interspersed by a beep of 300ms: the cleaner has received the map and is ready to drive
- 3 beeps of 300ms, interspersed by 2 beeps of 150ms: the cleaner has received an error while driving

---

### Map Processing

The cleaner receives the map from the UART pin PA3. I have used the polling mode because the cleaner does not need to do anything else in the meanwhile.

The reading process is optimized to speed up the data saving. In fact, after reading the `row_count` and `column_count`, I allocate 2 arrays:

- `(CellType**) map_rows`: one of size `row_count` that contains the pointers to the map rows
- `(CellType*) map`: another one of size `row_count * column_count` containing the map cells

Assuming to have a map of 3 rows and 2 columns, this is how the 2 arrays are filled:

```

map:                                ┌───┬───┐
                      ┌───────────> │   │   │
                      │             ├───┼───┤
                      │   ┌───────> │   │   │
                      │   │         ├───┼───┤
                      │   │   ┌───> │   │   │
                      │   │   │     └───┴───┘
                      │   │   │
                      │   │   │
                      │   │   │
                    ┌───┬───┬───┐
map_rows:           │   │   │   │
                    └───┴───┴───┘
```

In this way, I can read the entire map at once by passing the `map` array as buffer. I am sure that the `map` array is a continuous block of memory, but I can access it through the `map_rows` array because it contains the pointers to the map rows.

The map data are placed in a struct of type `MapInfo`:

```
typedef struct MapInfo {
	uint8_t** map;
	uint16_t row_count;
	uint16_t column_count;
} MapInfo;
```

---

### Driving Preparation

Once the cleaner has received the entire map, the user can instruct the cleaner to start by pressing the user button on the Nucleo board. After about 1 second, the `start_drive` method is called and the cleaner will start driving:

```















