/*
	isopath - Iso-path game analytics
	Copyright (C) 2018-2018 Johannes Bauer

	This file is part of isopath.

	isopath is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; this program is ONLY licensed under
	version 3 of the License, later versions are explicitly excluded.

	isopath is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	Johannes Bauer <JohannesBauer@gmx.de>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

static const char* dump_tile_char[] = {
	[EMPTY_NEUTRAL] = "-",
	[EMPTY_TRENCH] = "_",
	[EMPTY_CLIMB] = "^",
	[PIECE_TRENCH] = ".",
	[PIECE_CLIMB] = "°",
};

void tile_index_to_canonical_pos(unsigned int tile_index, uint8_t n, struct canonical_position_t *canonical_pos) {
	memset(canonical_pos, 0, sizeof(struct canonical_position_t));
	canonical_pos->tile_index = 0;
	canonical_pos->n = n;
	for (int row_width = canonical_pos->n; row_width < 2 * canonical_pos->n; row_width++) {
		canonical_pos->row_width = row_width;
		for (int i = 0; i < row_width; i++) {
			canonical_pos->col_number = i;
			if (tile_index == canonical_pos->tile_index) {
				return;
			}
			canonical_pos->tile_index++;
		}
		canonical_pos->row_number++;
	}
	for (int row_width = 2 * canonical_pos->n - 2; row_width >= canonical_pos->n; row_width--) {
		canonical_pos->row_width = row_width;
		for (int i = 0; i < row_width; i++) {
			canonical_pos->col_number = i;
			if (tile_index == canonical_pos->tile_index) {
				return;
			}
			canonical_pos->tile_index++;
		}
		canonical_pos->row_number++;
	}
}

void dump_canonical_pos(const struct canonical_position_t *canonical_pos) {
	printf("%2d: %2d %2d (%d) ", canonical_pos->tile_index, canonical_pos->row_number, canonical_pos->col_number, canonical_pos->row_width);
//	if (
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
		printf("%s  ", tile_char);
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
