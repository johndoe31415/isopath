/**
 *	isopath - Iso-path game analytics
 *	Copyright (C) 2018-2018 Johannes Bauer
 *
 *	This file is part of isopath.
 *
 *	isopath is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	isopath is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

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

#define CANONICAL_LOCFLAG_TOP		(1 << 0)
#define CANONICAL_LOCFLAG_LEFT		(1 << 1)
#define CANONICAL_LOCFLAG_RIGHT		(1 << 2)
#define CANONICAL_LOCFLAG_BOTTOM	(1 << 3)
#define CANONICAL_LOCFLAG_NORTH		(1 << 4)
#define CANONICAL_LOCFLAG_EQUATOR	(1 << 5)
#define CANONICAL_LOCFLAG_SOUTH		(1 << 6)
#define CANONICAL_LOCFLAG_TELEPORT	(1 << 7)
#define CANONICAL_LOCMASK_BORDER	(CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_LEFT | CANONICAL_LOCFLAG_RIGHT | CANONICAL_LOCFLAG_BOTTOM)

#define ADJACENT_TOP_LEFT			(1 << 0)
#define ADJACENT_TOP_RIGHT			(1 << 1)
#define ADJACENT_LEFT				(1 << 2)
#define ADJACENT_RIGHT				(1 << 3)
#define ADJACENT_BOTTOM_LEFT		(1 << 4)
#define ADJACENT_BOTTOM_RIGHT		(1 << 5)
#define ADJACENT_MASK_ALL			(ADJACENT_TOP_LEFT | ADJACENT_TOP_RIGHT | ADJACENT_LEFT | ADJACENT_RIGHT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT)

struct canonical_position_t {
	unsigned int tile_index;
	uint8_t n;
	unsigned int row_number;
	unsigned int col_number;
	unsigned int row_width;
	uint8_t loc_flags, adj_flags;
	uint8_t adjacent_count;
	unsigned int adjacent_tiles[6];
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
