/******************************************************************************
 * This file is a part of the Interactive Chessboard Project
 ******************************************************************************/

/**
 * @file     chess.c
 * @author   JZimnol
 * @date     Mar 2022
 * @brief    File containing function definitions for Interactive Chessboard
 * @version  0.1
 */

#include "chess.h"
#include "gpio.h"

//******************************************************************************
// Global variable definitions
//******************************************************************************

volatile Pieces_t Chessboard[8][8];
volatile uint8_t SPI2_Buffer[8][8];
volatile GameStatus_t Game;

//******************************************************************************
// Private function prototypes
//******************************************************************************

bool IsOnChessboard(uint8_t row, uint8_t col);
bool CheckSingleField(uint8_t row, uint8_t col);

//******************************************************************************
// Function definitions
//******************************************************************************

/**-----------------------------------------------------------------------------
 * @brief     Check for diagonal movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckDiagonalMovements(uint8_t row, uint8_t col) {
    int8_t x;
    int8_t y;

    /* check up-right movements */
    x = -1;
    y = 1;
    while( IsOnChessboard(row+x, col+y) ) {
        if( CheckSingleField(row+x, col+y) ) {
            x--;
            y++;
        }
        else break;
    }

    /* check up-left movements */
    x = -1;
    y = -1;
    while( IsOnChessboard(row+x, col+y) ) {
        if( CheckSingleField(row+x, col+y) ) {
            x--;
            y--;
        }
        else break;
    }

    /* check down-right movements */
    x = 1;
    y = 1;
    while( IsOnChessboard(row+x, col+y) ) {
        if( CheckSingleField(row+x, col+y) ) {
            x++;
            y++;
        }
        else break;
    }

    /* check down-left movements */
    x = 1;
    y = -1;
    while( IsOnChessboard(row+x, col+y) ) {
        if( CheckSingleField(row+x, col+y) ) {
            x++;
            y--;
        }
        else break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief     Check for vertical movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckVerticalMovements(uint8_t row, uint8_t col) {
    int8_t x;

    /* check up movements */
    x = -1;
    while( IsOnChessboard(row+x, col) ) {
        if( CheckSingleField(row+x, col) ) {
            x--;
        }
        else break;
    }

    /* check down movements */
    x = 1;
    while( IsOnChessboard(row+x, col) ) {
        if( CheckSingleField(row+x, col) ) {
            x++;
        }
        else break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief     Check for horizontal movements and light up necessary LEDs
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void CheckHorizontalMovements(uint8_t row, uint8_t col) {
    int8_t y;

    /* check right movements */
    y = 1;
    while( IsOnChessboard(row, col+y) ) {
        if( CheckSingleField(row, col+y) ) {
            y++;
        }
        else break;
    }

    /* check left movements */
    y = -1;
    while( IsOnChessboard(row, col+y) ) {
        if( CheckSingleField(row, col+y) ) {
            y--;
        }
        else break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief     Check possible knight movements and light up necessary LEDs
 * @param[in] row of a given knight
 * @param[in] column of a given knight
 */
void CheckKnightMovements(uint8_t row, uint8_t col) {
    /* hard coded movements */
    if( IsOnChessboard(row+2, col+1) ) CheckSingleField(row+2, col+1);
    if( IsOnChessboard(row+2, col-1) ) CheckSingleField(row+2, col-1);
    if( IsOnChessboard(row-2, col+1) ) CheckSingleField(row-2, col+1);
    if( IsOnChessboard(row-2, col-1) ) CheckSingleField(row-2, col-1);
    if( IsOnChessboard(row+1, col+2) ) CheckSingleField(row+1, col+2);
    if( IsOnChessboard(row+1, col-2) ) CheckSingleField(row+1, col-2);
    if( IsOnChessboard(row-1, col+2) ) CheckSingleField(row-1, col+2);
    if( IsOnChessboard(row-1, col-2) ) CheckSingleField(row-1, col-2);
}

/**-----------------------------------------------------------------------------
 * @brief     Check possible pawn movements and light up necessary LEDs
 * @param[in] row of a given pawn
 * @param[in] column of a given pawn
 */
void CheckPawnMovements(uint8_t row, uint8_t col) {
    switch( Game.turn ) {
    case P1:
        /* check up-right field */
        if( IsOnChessboard(row-1, col+1) ) {
            if( Chessboard[row-1][col+1]%2 == 0 &&
                Chessboard[row-1][col+1] != emptyField )
            {
                SPI2_Buffer[row-1][col+1] = RED_LIGHT;
            }
        }
        /* check up-left field */
        if( IsOnChessboard(row-1, col-1) ) {
            if( Chessboard[row-1][col-1]%2 == 0 &&
                Chessboard[row-1][col-1] != emptyField )
            {
                SPI2_Buffer[row-1][col-1] = RED_LIGHT;
            }
        }

        /* check up movement */
        if( IsOnChessboard(row-1, col) ) {
            if( Chessboard[row-1][col] == emptyField )
            {
                SPI2_Buffer[row-1][col] = WHITE_LIGHT;
            }
        }
        /* check first move */
        if( row == 6 ) {
            if( Chessboard[row-1][col] == emptyField &&
                Chessboard[row-2][col] == emptyField )
            {
                SPI2_Buffer[row-2][col] = WHITE_LIGHT;
            }
        }
        break;
    case P2:
        /* check down-right field */
        if( IsOnChessboard(row+1, col+1) ) {
            if( Chessboard[row+1][col+1]%2 == 0 &&
                Chessboard[row+1][col+1] != emptyField )
            {
                SPI2_Buffer[row+1][col+1] = RED_LIGHT;
            }
        }
        /* check down-left field */
        if( IsOnChessboard(row+1, col-1) ) {
            if( Chessboard[row+1][col-1]%2 == 0 &&
                Chessboard[row+1][col-1] != emptyField )
            {
                SPI2_Buffer[row+1][col-1] = RED_LIGHT;
            }
        }

        /* check down movement */
        if( IsOnChessboard(row+1, col) ) {
            if( Chessboard[row+1][col] == emptyField ) {
                SPI2_Buffer[row+1][col] = WHITE_LIGHT;
            }
        }
        /* check first move */
        if( row == 1 ) {
            if( Chessboard[row+1][col] == emptyField &&
                Chessboard[row+2][col] == emptyField )
            {
                SPI2_Buffer[row+2][col] = WHITE_LIGHT;
            }
        }
        break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief     Check possible king movements and light up necessary LEDs
 * @param[in] row of a given king
 * @param[in] column of a given king
 */
void CheckKingMovements(uint8_t row, uint8_t col) {
    /* hard coded movements */
    if( IsOnChessboard(row,   col+1) ) CheckSingleField(row, col+1);
    if( IsOnChessboard(row,   col-1) ) CheckSingleField(row, col-1);
    if( IsOnChessboard(row+1, col+1) ) CheckSingleField(row+1, col+1);
    if( IsOnChessboard(row+1, col-1) ) CheckSingleField(row+1, col-1);
    if( IsOnChessboard(row-1, col+1) ) CheckSingleField(row-1, col+1);
    if( IsOnChessboard(row-1, col-1) ) CheckSingleField(row-1, col-1);
    if( IsOnChessboard(row-1, col) )   CheckSingleField(row-1, col);
    if( IsOnChessboard(row+1, col) )   CheckSingleField(row+1, col);
}

/**-----------------------------------------------------------------------------
 * @brief Send whole SPI2_Buffer to the shift registers on the chessboard
 */
void UpdateWholeChessboard() {
    Game.SPI2_x = 0;
    Game.SPI2_y = 0;
    uint8_t dataToSend = 0;

    while( Game.SPI2_x != 8 ) {
        if( Game.SPI2_x%2 == 0 ) {
            dataToSend = ~((SPI2_Buffer[Game.SPI2_x][Game.SPI2_y]<<4) |
                            SPI2_Buffer[Game.SPI2_x][Game.SPI2_y+1]);
        }
        else {
            dataToSend = ~((SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y]<<4) |
                            SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y-1]);

        }
        // blocking transmission, 7.11 us
        HAL_SPI_Transmit(&hspi2, &dataToSend, 1, 100000);

        Game.SPI2_y += 2;
        if( Game.SPI2_y == 8 ) {
            Game.SPI2_y = 0;
            Game.SPI2_x++;
            if( Game.SPI2_x == 8 ) {
                HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, GPIO_PIN_RESET);
            }
        }
    }
}

/**-----------------------------------------------------------------------------
 * @brief     find possible moves for a given piece
 * @param[in] piece ID
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 */
void FindMoves(Pieces_t piece, uint8_t row, uint8_t col) {
    switch( piece ) {
    case P1_pawn:
    case P2_pawn:
        CheckPawnMovements(row, col);
        break;
    case P1_rook:
    case P2_rook:
        CheckHorizontalMovements(row, col);
        CheckVerticalMovements(row, col);
        break;
    case P1_knight:
    case P2_knight:
        CheckKnightMovements(row, col);
        break;
    case P1_bishop:
    case P2_bishop:
        CheckDiagonalMovements(row, col);
        break;
    case P1_queen:
    case P2_queen:
        CheckDiagonalMovements(row, col);
        CheckHorizontalMovements(row, col);
        CheckVerticalMovements(row, col);
        break;
    case P1_king:
    case P2_king:
        CheckKingMovements(row, col);
        break;
    default:
        break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief Find next available piece and light up certain LEDS
 *        For P1 goes from bottom left of a chessboard ([7][0]) to right each row
 *        For P2 goes from top right of a chessboard ([0][7]) to left each row
 */
void FindNextPiece() {
    int8_t i;
    int8_t j;
    uint8_t x;
    uint8_t y;

    switch( Game.turn ) {
    case P1:
        Game.P1.retryCounter = 0;
        i = 1;
        j = 0;
        while( Game.P1.retryCounter < 64 ) {
            if( (Game.P1.currentField[1]+i)%8 == 0 ) {
                j--;
                if( Game.P1.currentField[0]+j < 0 ) {
                    j += 8;
                }
            }
            x = Game.P1.currentField[0] + j;
            y = (Game.P1.currentField[1] + i)%8;
            if( (Chessboard[x][y] != emptyField) && (Chessboard[x][y]%2 == 1) ) {
                SPI2_Buffer[x][y] = GREEN_LIGHT;
                FindMoves(Chessboard[x][y], x, y);
                Game.P1.currentField[0] = x;
                Game.P1.currentField[1] = y;
                Game.P1.hasPieces = true;
                UpdateWholeChessboard();

                return;
            }
            i++;
            Game.P1.retryCounter++;
        }

        // no piece found
        Game.P1.hasPieces = false;
        if( Game.P2.hasPieces == true ) {
            Game.turn = P2;
            FindNextPiece();
        }
        else {
            Game.mode = Normal;
        }
        break;
    case P2:
        Game.P2.retryCounter = 0;
        i = 1;
        j = 0;
        while( Game.P2.retryCounter < 64 ) {
            if( Game.P2.currentField[1]-i < 0 ) {
                j++;
                i -= 8;
                if( Game.P2.currentField[0]+j > 7 ) {
                    j -= 8;
                }
            }
            x = Game.P2.currentField[0] + j;
            y = Game.P2.currentField[1] - i;
            if( (Chessboard[x][y] != emptyField) && (Chessboard[x][y]%2 == 0) ) {
                SPI2_Buffer[x][y] = GREEN_LIGHT;
                FindMoves(Chessboard[x][y], x, y);
                Game.P2.currentField[0] = x;
                Game.P2.currentField[1] = y;
                Game.P2.hasPieces = true;
                UpdateWholeChessboard();

                return;
            }
            i++;
            Game.P2.retryCounter++;
        }

        // no piece found
        Game.P2.hasPieces = false;
        if( Game.P1.hasPieces == true ) {
            Game.turn = P1;
            FindNextPiece();
        }
        else {
            Game.mode = Normal;
        }
        break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief Find previous available piece and light up certain LEDS
 *        For P1 goes from top right left of a chessboard ([0][7]) to left each row
 *        For P2 goes from bottom left of a chessboard ([7][0]) to right each row
 */
void FindPrevPiece() {
    int8_t i;
    int8_t j;
    uint8_t x;
    uint8_t y;

    switch( Game.turn ) {
    case P1:
        Game.P1.retryCounter = 0;
        i = 1;
        j = 0;
        while( Game.P1.retryCounter < 64 ) {
            if( Game.P1.currentField[1]-i < 0 ) {
                j++;
                i -= 8;
                if( Game.P1.currentField[0]+j > 7 ) {
                    j -= 8;
                }
            }
            x = Game.P1.currentField[0] + j;
            y = Game.P1.currentField[1] - i;
            if( (Chessboard[x][y] != emptyField) && (Chessboard[x][y]%2 == 1) ) {
                SPI2_Buffer[x][y] = GREEN_LIGHT;
                FindMoves(Chessboard[x][y], x, y);
                Game.P1.currentField[0] = x;
                Game.P1.currentField[1] = y;
                Game.P1.hasPieces = true;
                UpdateWholeChessboard();

                return;
            }
            i++;
            Game.P1.retryCounter++;
        }

        // no piece found
        Game.P1.hasPieces = false;
        if( Game.P2.hasPieces == true ) {
            Game.turn = P2;
            FindNextPiece();
        }
        else {
            Game.mode = Normal;
        }
        break;
    case P2:
        Game.P2.retryCounter = 0;
        i = 1;
        j = 0;
        while( Game.P2.retryCounter < 64 ) {
            if( (Game.P2.currentField[1]+i)%8 == 0 ) {
                j--;
                if( Game.P2.currentField[0]+j < 0 ) {
                    j += 8;
                }
            }
            x = Game.P2.currentField[0] + j;
            y = (Game.P2.currentField[1] + i)%8;
            if( (Chessboard[x][y] != emptyField) && (Chessboard[x][y]%2 == 0) ) {
                SPI2_Buffer[x][y] = GREEN_LIGHT;
                FindMoves(Chessboard[x][y], x, y);
                Game.P2.currentField[0] = x;
                Game.P2.currentField[1] = y;
                Game.P2.hasPieces = true;
                UpdateWholeChessboard();

                return;
            }
            i++;
            Game.P2.retryCounter++;
        }

        // no piece found
        Game.P2.hasPieces = false;
        if( Game.P1.hasPieces == true ) {
            Game.turn = P1;
            FindNextPiece();
        }
        else {
            Game.mode = Normal;
        }
        break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief Initialize buffers and game status
 */
void GameInit() {
    Game.mode = Normal;
    Game.turn = P1;

    Game.P1.currentField[0] = 7;
    Game.P1.currentField[1] = 0;
    Game.P1.retryCounter = 0;
    Game.P1.hasPieces = true;

    Game.P2.currentField[0] = 0;
    Game.P2.currentField[1] = 7;
    Game.P2.retryCounter = 0;
    Game.P2.hasPieces = true;

    Game.SPI2_x = 0;
    Game.SPI2_y = 0;

    ClearPiecesBuffer();
    ClearSPI2Buffer();
}

/**-----------------------------------------------------------------------------
 * @brief Sets all fields in SPI2Buffer to zero
 */
void ClearSPI2Buffer() {
    for( uint8_t i=0; i<8; i++ ) {
        for( uint8_t j=0; j<8; j++ ) {
            SPI2_Buffer[i][j] = 0;
        }
    }
}

/**-----------------------------------------------------------------------------
 * @brief Sets all fields in Chessboard buffer to zero
 */
void ClearPiecesBuffer() {
    for( uint8_t i=0; i<8; i++ ) {
        for( uint8_t j=0; j<7; j++ ) {
            Chessboard[i][j] = emptyField;
        }
    }
}

/**-----------------------------------------------------------------------------
 * @brief Set specific output pins to drive multiplexers
 */
void SetMuxOutputs(uint8_t num) {
    HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (num>>0)%2);
    HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (num>>1)%2);
    HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (num>>2)%2);
}

/**-----------------------------------------------------------------------------
 * @brief     Check if the value is within the margin of error
 * @param[in] reference value of a piece
 * @param[in] possible margin (ADC error value)
 * @param[in] value to be checked
 * @return    true if value meets the conditions
 */
bool IsBetween(uint16_t reference, uint16_t margin, uint16_t value) {
    bool retval = (value >= reference-margin)&&(value <= reference+margin);
    return retval;
}

//******************************************************************************
// Private function definitions
//******************************************************************************

/**-----------------------------------------------------------------------------
 * @brief     Check if coordinates belong to the chessboard
 * @param[in] row of a given piece
 * @param[in] column of a given piece
 * @return    true  if coordinates belong
 */
bool IsOnChessboard(uint8_t row, uint8_t col) {
    bool retval = (row>=0 && row<=7 && col>=0 && col<=7);
    return retval;
}

/**-----------------------------------------------------------------------------
 * @brief     Checks single field and lights up necessary LEDs
 * @param[in] row of a given  piece
 * @param[in] column of a given piece
 * @return    true if field is empty;
 */
bool CheckSingleField(uint8_t row, uint8_t col) {
    bool result = true;
    if( Chessboard[row][col] != emptyField ) {
        if( (Game.turn == P1 && Chessboard[row][col]%2 == 0) ||
            (Game.turn == P2 && Chessboard[row][col]%2 == 1) )
        {
            SPI2_Buffer[row][col] = RED_LIGHT;
        }
        result = false;
    }
    else {
        SPI2_Buffer[row][col] = WHITE_LIGHT;
    }

    return result;
}
