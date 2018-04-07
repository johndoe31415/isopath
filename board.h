#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>

enum tile_state_t {
	EMPTY_NEUTRAL,
	EMPTY_TRENCH,
	EMPTY_CLIMB,
	PIECE_TRENCH,
	PIECE_CLIMB,
};

/* Iso-Path(n) has #P = 3 n² - 3n + 1 */
/* i.e., Iso-Path(4) has #P = 37 */
#define NUMBER_TILES(n)			((3 * (n * n)) - (3 * n) + 1)
#define BOARD_SIZE_BYTES(n)		(sizeof(struct board_t) + (sizeof(uint8_t) * NUMBER_TILES(n)))
struct board_t {
	uint8_t n;
	uint8_t tiles[];
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void board_dump(const struct board_t *board, uint8_t n);
struct board_t *board_init(uint8_t n);
void board_free(struct board_t *board);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
