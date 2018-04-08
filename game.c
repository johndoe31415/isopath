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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "game.h"

static bool are_tile_adjacent(const struct game_t *game, unsigned int index1, unsigned int index2) {
	for (int i = 0; i < game->canpos[index1].adjacent_count; i++) {
		if (game->canpos[index1].adjacent_tiles[i] == index2) {
			return true;
		}
	}
	return false;
}

static bool apply_move(struct board_t *board, const struct move_t *move) {
	return false;
}

bool is_action_legal(struct game_t *game, const struct action_t *action) {
	return true;
}

void perform_action(struct game_t *game, const struct action_t *action) {
}

struct game_t* game_init(uint8_t n) {
	struct game_t *result = calloc(1, sizeof(struct game_t));
	if (!result) {
		return NULL;
	}
	result->canpos = malloc(sizeof(struct canonical_position_t) * NUMBER_TILES(n));
	if (!result->canpos) {
		free(result);
		return NULL;
	}
	result->board = board_init(n);
	if (!result->board) {
		free(result->canpos);
		free(result);
		return NULL;
	}
	for (int i = 0; i < NUMBER_TILES(n); i++) {
		tile_index_to_canonical_pos(i, n, &result->canpos[i]);
	}
	return result;
}

void game_free(struct game_t *game) {
	board_free(game->board);
	free(game->canpos);
	free(game);
}
