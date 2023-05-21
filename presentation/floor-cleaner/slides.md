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
    * Nucleo-L053R8 controls the cleaner
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

## Python App (Map Sending)

<div class="grid grid-row-2 grid-col-5 centered-grid">

  ![Initial Map](/images/initial-map.png)

  <material-symbols-arrow-right-alt-rounded class="text-5xl"/>

  

</div>

---

