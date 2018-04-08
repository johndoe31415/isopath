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

static bool are_tiles_adjacent(const struct game_t *game, unsigned int index1, unsigned int index2) {
	for (int i = 0; i < game->canpos[index1].adjacent_count; i++) {
		if (game->canpos[index1].adjacent_tiles[i] == index2) {
			return true;
		}
	}
	return false;
}

static void revert_move(struct game_t *game, const struct move_t *move) {
	struct board_t *board = game->board;
	if (move->type == BUILD) {
		board->tiles[move->src_tile]++;
		board->tiles[move->dst_tile]--;
	} else if (move->type == MOVE) {
		uint8_t player_piece = (game->side_turn == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		uint8_t empty_piece = (game->side_turn == TRENCH) ? EMPTY_TRENCH : EMPTY_CLIMB;
		board->tiles[move->dst_tile] = empty_piece;
		board->tiles[move->src_tile] = player_piece;
	} else if (move->type == CAPTURE) {
		uint8_t enemy_piece = (game->side_turn == TRENCH) ? PIECE_CLIMB : PIECE_TRENCH;
		board->tiles[move->dst_tile] = enemy_piece;
	}
}

static void apply_move(struct game_t *game, const struct move_t *move) {
	struct board_t *board = game->board;
	if (move->type == BUILD) {
		board->tiles[move->src_tile]--;
		board->tiles[move->dst_tile]++;
	} else if (move->type == MOVE) {
		uint8_t player_piece = (game->side_turn == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		uint8_t empty_piece = (game->side_turn == TRENCH) ? EMPTY_TRENCH : EMPTY_CLIMB;
		board->tiles[move->dst_tile] = player_piece;
		board->tiles[move->src_tile] = empty_piece;
	} else if (move->type == CAPTURE) {
		uint8_t empty_enemy_piece = (game->side_turn == TRENCH) ? EMPTY_CLIMB : EMPTY_TRENCH;
		board->tiles[move->dst_tile] = empty_enemy_piece;
	}
}

static bool is_move_legal(struct game_t *game, const struct move_t *move) {
	struct board_t *board = game->board;
	enum side_t player = game->side_turn;
	if (move->type == BUILD) {
		if ((board->tiles[move->src_tile] != EMPTY_NEUTRAL) && (board->tiles[move->src_tile] != EMPTY_CLIMB)) {
			/* Can only move when there's something there (without a piece on
			 * it) */
			return false;
		}
		if ((board->tiles[move->dst_tile] != EMPTY_TRENCH) && (board->tiles[move->src_tile] != EMPTY_NEUTRAL)) {
			/* Can only move to a tile where's either nothing or neutral (and
			 * no piece on it).  */
			return false;
		}
	} else if (move->type == MOVE) {
		uint8_t player_piece = (player == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		uint8_t empty_piece = (player == TRENCH) ? EMPTY_TRENCH : EMPTY_CLIMB;
		if (board->tiles[move->src_tile] != player_piece) {
			/* Player not allowed to move this (either not own piece or no
			 * piece on source tile) */
			return false;
		}
		if (board->tiles[move->dst_tile] != empty_piece) {
			/* Player not allowed to move to here (either not right elecation
			 * or already piece on that tile) */
			return false;
		}
		if (!are_tiles_adjacent(game, move->src_tile, move->dst_tile)) {
			/* Can only move between adjacent tiles */
			return false;
		}
	} else if (move->type == CAPTURE) {
		uint8_t enemy_piece = (player == TRENCH) ? PIECE_CLIMB : PIECE_TRENCH;
		uint8_t player_piece = (player == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		if (board->tiles[move->dst_tile] != enemy_piece) {
			/* There's no enemy on the piece we're trying to capture */
			return false;
		}

		/* Now look at all adjacent tiles and see if there's a player on there.
		 * We need at least two.  */
		int surrounded_by = 0;
		for (int i = 0; i < game->canpos[move->dst_tile].adjacent_count; i++) {
			unsigned int adjacent_tile_index = game->canpos[move->dst_tile].adjacent_tiles[i];
			if (board->tiles[adjacent_tile_index] == player_piece) {
				surrounded_by++;
				if (surrounded_by == 2) {
					break;
				}
			}
		}
		if (surrounded_by < 2) {
			/* Cannot capture, not surrounded by enough player pieces */
			return false;
		}
	} else {
		return false;
	}
	return true;
}


bool is_action_legal(struct game_t *game, const struct action_t *action) {
	bool is_legal = is_move_legal(game, &action->moves[0]);
	if (is_legal) {
		apply_move(game, &action->moves[0]);
		is_legal = is_move_legal(game, &action->moves[1]);
		revert_move(game, &action->moves[0]);
	}
	return is_legal;
}

void perform_action(struct game_t *game, const struct action_t *action) {
	apply_move(game, &action->moves[1]);
	apply_move(game, &action->moves[2]);
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
	result->side_turn = CLIMB;
	return result;
}

void game_free(struct game_t *game) {
	board_free(game->board);
	free(game->canpos);
	free(game);
}
