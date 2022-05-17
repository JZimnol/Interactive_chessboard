# Interactive chessboard
This project has been created to be presented at the Scientific Club Conference (we won btw). 

## Project assumptions
The aim of the project was to create an interactive 3D chessboard that would be able to interact with players.
The chessboard have dimensions of approx. 30x30 cm and has been created with the pieces from scratch using 3D printing technology.
After connecting the board to the power supply, individual fields light up depending on the mode selected by the users: 
- **free play mode** - in this mode the fields of the chessboard will be highlighted only in white only in places where pieces are placed
- **educational mode** – each user during his turn will be able to display possible moves of each piece 

<p align="center">
<img src="https://github.com/JZimnol/Interactive_chessboard/blob/main/img/chess_example_moves.png" width="300">
</p>
<p align="center">
Example of chessboard educational mode light sequence 
</p>

The innovation of the project is the ability to distinguish individual pieces on the chessboard by the program.
This is ensured by voltage dividers created on each field between the resistors placed in the pieces and the reference resistor located under the chessboard field,
and then the voltage readings using 8 analog multiplexers. The ability to display several colors on each field (64 RGB LEDs) is be provided by proprietary system
made of 32 SIPO shift registers. All logic is be controlled by one microcontroller from the STM32F4 series.    

In the future, the project can be developed using open-software algorithms for predicting user movements so that only one user can play with the computer using this chessboard. 

<p align="center">
<img src="https://github.com/JZimnol/Interactive_chessboard/blob/main/img/chess_prototype.png" width="550">
</p>
<p align="center">
Chessboard prototype created with handmade PCBs 
</p>
