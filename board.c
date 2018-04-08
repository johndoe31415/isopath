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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"

static const char* dump_tile_char[] = {
	[EMPTY_NEUTRAL] = "…  ",
	[EMPTY_TRENCH] = "_  ",
	[EMPTY_CLIMB] = "¯  ",
	[PIECE_TRENCH] = "⚫ ",
	[PIECE_CLIMB] = "⚪ ",
};

static bool title_index_to_canonical_pos_row(unsigned int tile_index, struct canonical_position_t *canonical_pos, int row_width) {
	canonical_pos->row_width = row_width;
	for (int i = 0; i < row_width; i++) {
		canonical_pos->col_number = i;
		if (tile_index == canonical_pos->tile_index) {
			return true;
		}
		canonical_pos->tile_index++;
	}
	canonical_pos->row_number++;
	return false;
}

static int sort_uint(const void *v_uint1ptr, const void *v_uint2ptr) {
	const unsigned int *uint1 = (const unsigned int*)v_uint1ptr;
	const unsigned int *uint2 = (const unsigned int*)v_uint2ptr;
	if (*uint1 < *uint2) {
		return -1;
	} else if (*uint1 == *uint2) {
		return 0;
	} else {
		return 1;
	}
}

void tile_index_to_canonical_pos(unsigned int tile_index, uint8_t n, struct canonical_position_t *canonical_pos) {
	memset(canonical_pos, 0, sizeof(struct canonical_position_t));
	canonical_pos->tile_index = 0;
	canonical_pos->n = n;
	bool found = false;
	for (int row_width = canonical_pos->n; row_width < 2 * canonical_pos->n; row_width++) {
		found = title_index_to_canonical_pos_row(tile_index, canonical_pos, row_width);
		if (found) {
			break;
		}
	}
	if (!found) {
		for (int row_width = 2 * canonical_pos->n - 2; row_width >= canonical_pos->n; row_width--) {
			found = title_index_to_canonical_pos_row(tile_index, canonical_pos, row_width);
			if (found) {
				break;
			}
		}
	}

	/* Now complete positional flag computation */
	if (canonical_pos->row_number == 0) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_TOP;
	} else if (canonical_pos->row_number == (2 * n) - 2) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_BOTTOM;
	}

	if (canonical_pos->col_number == 0) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_LEFT;
	} else if (canonical_pos->col_number == canonical_pos->row_width - 1) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_RIGHT;
	}

	if (canonical_pos->row_number < n - 1) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_NORTH;
	} else if (canonical_pos->row_number > n - 1) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_SOUTH;
	} else {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_EQUATOR;
	}

	/* Determine teleport pieces */
	if ((canonical_pos->loc_flags & (CANONICAL_LOCFLAG_LEFT | CANONICAL_LOCFLAG_RIGHT)) &&
		(canonical_pos->loc_flags & (CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_EQUATOR | CANONICAL_LOCFLAG_BOTTOM))) {
		canonical_pos->loc_flags |= CANONICAL_LOCFLAG_TELEPORT;
	}

	/* Then infer the adjacent pieces. Start with a mask that contains all and
	 * then take away specifics for different pieces. */
	canonical_pos->adj_flags = ADJACENT_MASK_ALL;

	if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_LEFT) {
		canonical_pos->adj_flags &= ~ADJACENT_LEFT;
		if (canonical_pos->loc_flags & (CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_EQUATOR)) {
			canonical_pos->adj_flags &= ~ADJACENT_TOP_LEFT;
		}
		if (canonical_pos->loc_flags & (CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_EQUATOR)) {
			canonical_pos->adj_flags &= ~ADJACENT_BOTTOM_LEFT;
		}
	} else if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_RIGHT) {
		canonical_pos->adj_flags &= ~ADJACENT_RIGHT;
		if (canonical_pos->loc_flags & (CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_EQUATOR)) {
			canonical_pos->adj_flags &= ~ADJACENT_TOP_RIGHT;
		}
		if (canonical_pos->loc_flags & (CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_EQUATOR)) {
			canonical_pos->adj_flags &= ~ADJACENT_BOTTOM_RIGHT;
		}
	}

	if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_TOP) {
		canonical_pos->adj_flags &= ~(ADJACENT_TOP_LEFT | ADJACENT_TOP_RIGHT);
	}  else if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_BOTTOM) {
		canonical_pos->adj_flags &= ~(ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT);
	}

	/* Lastly, determine adjacency array */
	if (canonical_pos->adj_flags & ADJACENT_TOP_LEFT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index - canonical_pos->row_width + (canonical_pos->loc_flags & CANONICAL_LOCFLAG_SOUTH ? -1 : 0);
	}
	if (canonical_pos->adj_flags & ADJACENT_TOP_RIGHT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index - canonical_pos->row_width + (canonical_pos->loc_flags & CANONICAL_LOCFLAG_SOUTH ? 0 : 1);
	}
	if (canonical_pos->adj_flags & ADJACENT_LEFT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index - 1;
	}
	if (canonical_pos->adj_flags & ADJACENT_RIGHT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index + 1;
	}
	if (canonical_pos->adj_flags & ADJACENT_BOTTOM_LEFT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index + canonical_pos->row_width + (canonical_pos->loc_flags & CANONICAL_LOCFLAG_NORTH ? 0 : -1);
	}
	if (canonical_pos->adj_flags & ADJACENT_BOTTOM_RIGHT) {
		canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index + canonical_pos->row_width + (canonical_pos->loc_flags & CANONICAL_LOCFLAG_NORTH ? 1 : 0);
	}
	if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_TELEPORT) {
		if (canonical_pos->loc_flags & CANONICAL_LOCFLAG_LEFT) {
			canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index + canonical_pos->row_width - 1;
		} else {
			canonical_pos->adjacent_tiles[canonical_pos->adjacent_count++] = canonical_pos->tile_index - canonical_pos->row_width + 1;
		}
	}

	/* Because of teleport fields, order of tiles may be messed up. For having
	 * consistent test cases, sort them. */
	qsort(canonical_pos->adjacent_tiles, canonical_pos->adjacent_count, sizeof(canonical_pos->adjacent_tiles[0]), sort_uint);
}

void dump_canonical_pos(const struct canonical_position_t *canonical_pos) {
	printf("%2d: %2d %2d (%d) ", canonical_pos->tile_index, canonical_pos->row_number, canonical_pos->col_number, canonical_pos->row_width);

	int len = 0;
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_TOP) ? "T" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_BOTTOM) ? "B" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_LEFT) ? "L" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_RIGHT) ? "R" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_NORTH) ? "N" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_EQUATOR) ? "E" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_SOUTH) ? "S" : "");
	len += printf("%s", (canonical_pos->loc_flags & CANONICAL_LOCFLAG_TELEPORT) ? "*" : "");
	for (int i = 0; i < 4 - len; i++) {
		printf(" ");
	}
	printf("   ");

	printf("%s", (canonical_pos->adj_flags & ADJACENT_TOP_LEFT) ? "↖ " : "  ");
	printf("%s", (canonical_pos->adj_flags & ADJACENT_TOP_RIGHT) ? "↗ " : "  ");
	printf("%s", (canonical_pos->adj_flags & ADJACENT_LEFT) ? "← " : "  ");
	printf("%s", (canonical_pos->adj_flags & ADJACENT_RIGHT) ? "→ " : "  ");
	printf("%s", (canonical_pos->adj_flags & ADJACENT_BOTTOM_LEFT) ? "↙ " : "  ");
	printf("%s", (canonical_pos->adj_flags & ADJACENT_BOTTOM_RIGHT) ? "↘ " : "  ");
	printf("   ");

	printf(" [ ");
	for (int i = 0; i < canonical_pos->adjacent_count; i++) {
		printf("%2u ", canonical_pos->adjacent_tiles[i]);
	}
	printf("]");
	printf("\n");
}

void dump_canonical_board(uint8_t n) {
	for (int i = 0; i < NUMBER_TILES(n); i++) {
		struct canonical_position_t cpos;
		tile_index_to_canonical_pos(i, n, &cpos);
		dump_canonical_pos(&cpos);
	}
}

static void row_dump(const struct board_t *board, int tile_index, int row_width) {
	int missing_chars = (2 * board->n) - 1 - row_width;
	for (int i = 0; i < missing_chars / 2; i++) {
		printf("   ");
	}
	if ((missing_chars % 2) == 1) {
		printf(" ");
	}
	for (int i = 0; i < row_width; i++) {
		uint8_t tile = board->tiles[tile_index + i];
		const char *tile_char = dump_tile_char[tile];
		printf("%s", tile_char);
	}
	printf("\n");
}

void board_dump(const struct board_t *board) {
	int tile_index = 0;
	for (int row_width = board->n; row_width < 2 * board->n; row_width++) {
		row_dump(board, tile_index, row_width);
		tile_index += row_width;
	}
	for (int row_width = 2 * board->n - 2; row_width >= board->n; row_width--) {
		row_dump(board, tile_index, row_width);
		tile_index += row_width;
	}
}

struct board_t *board_init(uint8_t n) {
	struct board_t *result = calloc(1, BOARD_SIZE_BYTES(n));
	result->n = n;
	const unsigned int tile_max_index = NUMBER_TILES(n) - 1;
	for (int i = 0; i < n; i++) {
		result->tiles[i] = PIECE_CLIMB;
		result->tiles[tile_max_index - i] = PIECE_TRENCH;
	}
	return result;
}

void board_free(struct board_t *board) {
	free(board);
}
