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


/* Canonical positions (row_number, col_number):
 *       0,0     0,1     0,2     0,3
 *   1,0     1,1     1,2     1,3     1,4
 * [...]
 */

#define CANONICAL_POSFLAG_TOP		(1 << 0)
#define CANONICAL_POSFLAG_LEFT		(1 << 1)
#define CANONICAL_POSFLAG_RIGHT		(1 << 2)
#define CANONICAL_POSFLAG_BOTTOM	(1 << 3)

struct canonical_position_t {
	unsigned int tile_index;
	uint8_t n;
	unsigned int row_number;
	unsigned int col_number;
	unsigned int row_width;
	uint8_t pos_flags;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void tile_index_to_canonical_pos(unsigned int tile_index, uint8_t n, struct canonical_position_t *canonical_pos);
void dump_canonical_pos(const struct canonical_position_t *canonical_pos);
void dump_canonical_board(uint8_t n);
void board_dump(const struct board_t *board);
struct board_t *board_init(uint8_t n);
void board_free(struct board_t *board);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
