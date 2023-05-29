---
# try also 'default' to start simple
theme: default
# apply any windi css classes to the current slide
class: 'text-center'
# https://sli.dev/custom/highlighters.html
highlighter: shiki
# show line numbers in code blocks
lineNumbers: false
# persist drawings in exports and build
drawings:
  persist: false
# page transition
transition: slide-left
# use UnoCSS
css: unocss
layout: cover
---

# Floor Cleaner

---

## Overview

<div v-clicks>

  - **What** 
    * A floor cleaner that cleans an area represented by a map, which is drawn in a Desktop App
  - **How**
    * Python App for the map drawing
    * The Nucleo-L053R8 controls the cleaner
</div>

---
layout: image-right
image: ./images/app-create-map.png
---

## Python App (Map)

- Each cell represents an area of about 5cm * 5cm 
- A blue cell must be cleaned, a grey cell must not
- The black cell represents the position of the cleaner

---

## Python App (Map Conversion)

<div class="grid grid-row-1 grid-col-3 grid-flow-col centered-grid gap-x-10">

  ![Initial Map](/images/initial-map.png)

  <material-symbols-arrow-right-alt-rounded class="text-5xl"/>

  ```
  { 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
  ```

</div>

---

## Python App (Map Sending)

The Python app detects the Nucleo board, which is connected through USB, and can start to send the map:

```python {all|1|2|3-6|8-10|12|all}
with serial.Serial(port=serial_data.device, baudrate=9600) as cleaner_serial:
    cleaner_serial.write(b'&')
    cleaner_serial.write(bytes(str(len(cleaning_map)), encoding='ascii'))
    cleaner_serial.write(b'|')
    cleaner_serial.write(bytes(str(len(cleaning_map[0])), encoding='ascii'))
    cleaner_serial.write(b'|')

    for row in cleaning_map:
        for cell in row:
            cleaner_serial.write(cell.value.to_bytes(1, byteorder='big'))

    cleaner_serial.write(b'&')
```

---

## Cleaner Architecture

<div>
<img src="/images/cleaner_architecture.png" class="w-140 h-100" />

<EmptyCircle radius="80%" xcenter="10px" ycenter="30px" />
</div>

---