/******************************************************************************
 * This file is a part of the Interactive Chessboard Project
 ******************************************************************************/

/**
 * @file     chess.h
 * @author   JZimnol
 * @date     Mar 2022
 * @brief    File containing structures and declarations of Interactive Chessboard
 * @version  0.1
 */

#ifndef INC_CHESS_H_
#define INC_CHESS_H_

#include "spi.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>


//******************************************************************************
// Global definitions
//******************************************************************************

#define BLUE_LIGHT           (0x08) // 00001000
#define RED_LIGHT            (0x04) // 00000100
#define GREEN_LIGHT          (0x02) // 00000010
#define NO_LIGHT             (0x00) // 00000000
#define PURPLE_LIGHT         (RED_LIGHT | BLUE_LIGHT)
#define YELLOW_LIGHT         (RED_LIGHT | GREEN_LIGHT)
#define WHITE_LIGHT          (RED_LIGHT | GREEN_LIGHT | BLUE_LIGHT)

#define REF_RESISTOR         (220)
#define ADC_MAX_VALUE        (4095)
#define ADC_MARGIN           (50)

#define P1_PAWN_RESISTOR     (100)
#define P1_ROOK_RESISTOR     (130)
#define P1_KNIGHT_RESISTOR   (150)
#define P1_BISHOP_RESISTOR   (220)
#define P1_QUEEN_RESISTOR    (330)
#define P1_KING_RESISTOR     (470)

#define P2_PAWN_RESISTOR     (680)
#define P2_ROOK_RESISTOR     (1000)
#define P2_KNIGHT_RESISTOR   (1300)
#define P2_BISHOP_RESISTOR   (1800)
#define P2_QUEEN_RESISTOR    (3000)
#define P2_KING_RESISTOR     (5600)

#define P1_PAWN_VALUE        (ADC_MAX_VALUE*P1_PAWN_RESISTOR/(P1_PAWN_RESISTOR + REF_RESISTOR))
#define P1_ROOK_VALUE        (ADC_MAX_VALUE*P1_ROOK_RESISTOR/(P1_ROOK_RESISTOR + REF_RESISTOR))
#define P1_KNIGHT_VALUE      (ADC_MAX_VALUE*P1_KNIGHT_RESISTOR/(P1_KNIGHT_RESISTOR + REF_RESISTOR))
#define P1_BISHOP_VALUE      (ADC_MAX_VALUE*P1_BISHOP_RESISTOR/(P1_BISHOP_RESISTOR + REF_RESISTOR))
#define P1_QUEEN_VALUE       (ADC_MAX_VALUE*P1_QUEEN_RESISTOR/(P1_QUEEN_RESISTOR + REF_RESISTOR))
#define P1_KING_VALUE        (ADC_MAX_VALUE*P1_KING_RESISTOR/(P1_KING_RESISTOR + REF_RESISTOR))

#define P2_PAWN_VALUE        (ADC_MAX_VALUE*P2_PAWN_RESISTOR/(P2_PAWN_RESISTOR + REF_RESISTOR))
#define P2_ROOK_VALUE        (ADC_MAX_VALUE*P2_ROOK_RESISTOR/(P2_ROOK_RESISTOR + REF_RESISTOR))
#define P2_KNIGHT_VALUE      (ADC_MAX_VALUE*P2_KNIGHT_RESISTOR/(P2_KNIGHT_RESISTOR + REF_RESISTOR))
#define P2_BISHOP_VALUE      (ADC_MAX_VALUE*P2_BISHOP_RESISTOR/(P2_BISHOP_RESISTOR + REF_RESISTOR))
#define P2_QUEEN_VALUE       (ADC_MAX_VALUE*P2_QUEEN_RESISTOR/(P2_QUEEN_RESISTOR + REF_RESISTOR))
#define P2_KING_VALUE        (ADC_MAX_VALUE*P2_KING_RESISTOR/(P2_KING_RESISTOR + REF_RESISTOR))

//******************************************************************************
// Global variable declarations
//******************************************************************************

/**-----------------------------------------------------------------------------
 * @brief simple enum containing chess pieces
 */
typedef enum {
    emptyField = 0,
    /* player_1 pieces, odd numbers */
    P1_pawn    = 1,
    P1_rook    = 3,
    P1_knight  = 5,
    P1_bishop  = 7,
    P1_queen   = 9,
    P1_king    = 11,
    /* player_2 pieces, even numbers */
    P2_pawn    = 2,
    P2_rook    = 4,
    P2_knight  = 6,
    P2_bishop  = 8,
    P2_queen   = 10,
    P2_king    = 12
} Pieces_t;

/**-----------------------------------------------------------------------------
 * @brief simple struct containing player info
 */
typedef struct {
    uint8_t currentField[2];
    uint8_t retryCounter;
    bool hasPieces;
} Player_t;

/**-----------------------------------------------------------------------------
 * @brief game status struct
 */
typedef struct {
    enum {Normal, Interactive} mode;
    enum {P1, P2} turn;
    Player_t P1;
    Player_t P2;
    uint8_t SPI2_x;  /* SPI2 buffer row counter */
    uint8_t SPI2_y;  /* SPI2 buffer column counter */
} GameStatus_t;

volatile extern GameStatus_t Game;

/**
 * @brief chessboard buffer, contains info about pieces on specific fields
 */
volatile extern Pieces_t Chessboard[8][8];
/**
 * @brief SPI2 buffer, contains info about colors on specific fields
 */
volatile extern uint8_t SPI2_Buffer[8][8];

//******************************************************************************
// Function declarations
//******************************************************************************

/**-----------------------------------------------------------------------------
 * @brief     Check for diagonal movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckDiagonalMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief     Check for vertical movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckVerticalMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief     Check for horizontal movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckHorizontalMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief     Check possible knight movements and light up necessary LEDs
 * @param[in] row of a given knight
 * @param[in] column of a given knight
 */
void CheckKnightMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief     Check possible pawn movements and light up necessary LEDs
 * @param[in] row of a given pawn
 * @param[in] column of a given pawn
 */
void CheckPawnMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief     Check possible king movements and light up necessary LEDs
 * @param[in] row of a given king
 * @param[in] column of a given king
 */
void CheckKingMovements(uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief Send whole SPI2_Buffer to the shift registers on the chessboard
 */
void UpdateWholeChessboard();

/**-----------------------------------------------------------------------------
 * @brief     find possible moves for a given piece
 * @param[in] piece ID
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void FindMoves(Pieces_t piece, uint8_t row, uint8_t col);

/**-----------------------------------------------------------------------------
 * @brief Find next available piece and light up certain LEDS
 *        For P1 goes from bottom left of a chessboard ([7][0]) to right each row
 *        For P2 goes from top right of a chessboard ([0][7]) to left each row
 */
void FindNextPiece();

/**-----------------------------------------------------------------------------
 * @brief Find previous available piece and light up certain LEDS
 *        For P1 goes from top right left of a chessboard ([0][7]) to left each row
 *        For P2 goes from bottom left of a chessboard ([7][0]) to right each row
 */
void FindPrevPiece();

/**-----------------------------------------------------------------------------
 * @brief Initialize buffers and game status
 */
void GameInit();

/**-----------------------------------------------------------------------------
 * @brief Sets all fields in SPI2Buffer to zero
 */
void ClearSPI2Buffer();

/**-----------------------------------------------------------------------------
 * @brief Sets all fields in Chessboard buffer to zero
 */
void ClearPiecesBuffer();

/**-----------------------------------------------------------------------------
 * @brief Set specific output pins to drive multiplexers
 */
void SetMuxOutputs(uint8_t num);

/**-----------------------------------------------------------------------------
 * @brief     Check if the value is within the margin of error
 * @param[in] reference value of a piece
 * @param[in] possible margin (ADC error value)
 * @param[in] value to be checked
 * @return    true if value meets the conditions
 */
bool IsBetween(uint16_t reference, uint16_t margin, uint16_t value);

/*

                           << BLACK PIECES (P2) START SIDE >>
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [0] [0] | [0] [1] | [0] [2] | [0] [3] | [0] [4] | [0] [5] | [0] [6] | [0] [7] |
[8]|         |         |         |         |         |         |         |         |
   |<--      |<--      |<--      |<--      |<--      |<--      |<--      |<--      |
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [1] [0] | [1] [1] | [1] [2] | [1] [3] | [1] [4] | [1] [5] | [1] [6] | [1] [7] |
[7]|         |         |         |         |         |         |         |        ^|
   |      -->|      -->|      -->|      -->|      -->|      -->|      -->|        ||
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [2] [0] | [2] [1] | [2] [2] | [2] [3] | [2] [4] | [2] [5] | [2] [6] | [2] [7] |
[6]|^        |         |         |         |         |         |         |         |
   ||        |<--      |<--      |<--      |<--      |<--      |<--      |<--      |
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [3] [0] | [3] [1] | [3] [2] | [3] [3] | [3] [4] | [3] [5] | [3] [6] | [3] [7] |
[5]|         |         |         |         |         |         |         |        ^|
   |      -->|      -->|      -->|      -->|      -->|      -->|      -->|        ||
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [4] [0] | [4] [1] | [4] [2] | [4] [3] | [4] [4] | [4] [5] | [4] [6] | [4] [7] |
[4]|^        |         |         |         |         |         |         |         |
   ||        |<--      |<--      |<--      |<--      |<--      |<--      |<--      |
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [5] [0] | [5] [1] | [5] [2] | [5] [3] | [5] [4] | [5] [5] | [5] [6] | [5] [7] |
[3]|         |         |         |         |         |         |         |        ^|
   |      -->|      -->|      -->|      -->|      -->|      -->|      -->|        ||
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [6] [0] | [6] [1] | [6] [2] | [6] [3] | [6] [4] | [6] [5] | [6] [6] | [6] [7] |
[2]|^        |         |         |         |         |         |         |         |
   ||        |<--      |<--      |<--      |<--      |<--      |<--      |<--      |
   +---------+---------+---------+---------+---------+---------+---------+---------+
   | [7] [0] | [7] [1] | [7] [2] | [7] [3] | [7] [4] | [7] [5] | [7] [6] | [7] [7] |
[1]|         |         |         |         |         |         |         |        ^|
-->|      -->|      -->|      -->|      -->|      -->|      -->|      -->|        ||
|  +---------+---------+---------+---------+---------+---------+---------+---------+
|      [a]       [b]       [c]       [d]       [e]       [f]       [g]       [h]
|                         << WHITE PIECES (P1) START SIDE >>
|_SPI SIGNAL IN

*/

#endif /* INC_CHESS_H_ */
