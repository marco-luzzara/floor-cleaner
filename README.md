# Floor Cleaner

This repo is made of 2 parts:

- the folder `app` contains the Python app to create and send the map for the cleaning
- the folder `cleaner` contains the C project for the STM32 Nucleo board that will take care of the cleaning procedure

---

## Protocol for Serial Communication

The app sends the map info through the `/dev/ttyACM0` serial device on Linux. These are the data sent throughout the procedure:

1. The character `&`, which represents the start (and the end) of the data flow
2. The number of rows in the grid
3. The character `|`, which is the separator character
4. The number of columns in the grid
5. The character `|`
6. From now on, every byte represents a cell in the grid, row by row from left to right: 
    - `1` stands for cell unavailable (to **not** clean)
    - `2` stands for cell that must be cleaned
    - `3` stands for the cleaner position (direction: up)
7. The character `&`

This is an example of data flow:

```
&3|4|211222231222&
```

For the following map:

```
░██░
░░░X
█░░░

░ = cell to clean
█ = cell to not clean
X = cleaner position
```





