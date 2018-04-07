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
#include "board.h"

static const char* dump_tile_char[] = {
	[EMPTY_NEUTRAL] = "-",
	[EMPTY_TRENCH] = "_",
	[EMPTY_CLIMB] = "^",
	[PIECE_TRENCH] = ".",
	[PIECE_CLIMB] = "°",
};

static void row_dump(const struct board_t *board, uint8_t n, int tile_index, int row_width) {
	int missing_chars = (2 * n) - 1 - row_width;
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

void board_dump(const struct board_t *board, uint8_t n) {
	int tile_index = 0;
	for (int row_width = n; row_width < 2 * n; row_width++) {
		row_dump(board, n, tile_index, row_width);
		tile_index += row_width;
	}
	for (int row_width = 2 * n - 2; row_width >= n; row_width--) {
		row_dump(board, n, tile_index, row_width);
		tile_index += row_width;
	}
}

struct board_t *board_init(uint8_t n) {
	struct board_t *result = calloc(1, BOARD_SIZE_BYTES(n));
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
