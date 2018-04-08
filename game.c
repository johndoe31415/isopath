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
#include "game.h"

struct first_move_ctx {
	void (*action_callback)(struct game_t *game, const struct action_t *action, void *vctx);
	void *action_ctx;
};

struct second_move_ctx {
	struct action_t action;
	struct first_move_ctx *first;
};

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

static bool tile_surrounded(const struct game_t *game, unsigned int index, uint8_t by_tile) {
	/* Now look at all adjacent tiles and see if there's a player on there.
	 * We need at least two.  */
	int surrounded_by = 0;
	for (int i = 0; i < game->canpos[index].adjacent_count; i++) {
		unsigned int adjacent_tile_index = game->canpos[index].adjacent_tiles[i];
		if (game->board->tiles[adjacent_tile_index] == by_tile) {
			surrounded_by++;
			if (surrounded_by == 2) {
				return true;
			}
		}
	}
	return false;
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
		 * We need at least two. */
		if (!tile_surrounded(game, move->dst_tile, player_piece)) {
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

void game_perform_action(struct game_t *game, const struct action_t *action) {
	apply_move(game, &action->moves[0]);
	apply_move(game, &action->moves[1]);
	game->side_turn = (game->side_turn == TRENCH) ? CLIMB : TRENCH;
}

static void enumerate_valid_moves(struct game_t *game, bool allow_capture, bool allow_build, bool allow_move, void (*enumeration_callback)(struct game_t *game, const struct move_t *move, void *ctx), void *ctx) {
	/* First determine if there's pieces that can be captured */
	if (allow_capture) {
		uint8_t enemy_piece = (game->side_turn == TRENCH) ? PIECE_CLIMB : PIECE_TRENCH;
		uint8_t player_piece = (game->side_turn == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		for (int i = 0; i < NUMBER_TILES(game->n); i++) {
			if ((game->board->tiles[i] == enemy_piece) && tile_surrounded(game, i, player_piece)) {
				/* Yes! Capture is possible. */
				struct move_t move = {
					.type = CAPTURE,
					.dst_tile = i,
				};
				enumeration_callback(game, &move, ctx);
			}
		}
	}

	/* Then enumerate all build moves */
	if (allow_build) {
		for (int src = 0; src < NUMBER_TILES(game->n); src++) {
			if ((game->board->tiles[src] == EMPTY_NEUTRAL) || (game->board->tiles[src] == EMPTY_CLIMB)) {
				for (int dst = 0; dst < NUMBER_TILES(game->n); dst++) {
					if (((game->board->tiles[dst] == EMPTY_NEUTRAL) || (game->board->tiles[dst] == EMPTY_TRENCH)) && (src != dst)) {
						/* Have a valid build move. */
						struct move_t move = {
							.type = BUILD,
							.src_tile = src,
							.dst_tile = dst,
						};
						enumeration_callback(game, &move, ctx);
					}
				}
			}
		}
	}

	/* Finally, enumerate all legal movement moves */
	// TODO: Not in home base!
	if (allow_move) {
		uint8_t player_piece = (game->side_turn == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
		uint8_t player_empty_piece = (game->side_turn == TRENCH) ? EMPTY_TRENCH : EMPTY_CLIMB;
		for (int src = 0; src < NUMBER_TILES(game->n); src++) {
			if (game->board->tiles[src] == player_piece) {
				for (int i = 0; i < game->canpos[src].adjacent_count; i++) {
					unsigned int adjacent_tile_index = game->canpos[src].adjacent_tiles[i];
					if (game->board->tiles[adjacent_tile_index] == player_empty_piece) {
						/* Found a valid movement move. */
						struct move_t move = {
							.type = MOVE,
							.src_tile = src,
							.dst_tile = adjacent_tile_index,
						};
						enumeration_callback(game, &move, ctx);
					}
				}
			}
		}
	}
}

static void second_move_callback(struct game_t *game, const struct move_t *move, void *vctx) {
	struct second_move_ctx *ctx = (struct second_move_ctx*)vctx;
	ctx->action.moves[1] = *move;
	apply_move(game, move);
	ctx->first->action_callback(game, &ctx->action, ctx->first->action_ctx);
	revert_move(game, move);
}

static void first_move_callback(struct game_t *game, const struct move_t *move, void *vctx) {
	struct first_move_ctx *ctx = (struct first_move_ctx*)vctx;

	/* We have just enumerated all possible first moves */
	struct second_move_ctx second_ctx = {
		.first = ctx,
	};
	second_ctx.action.moves[0] = *move;
	apply_move(game, move);
	if (move->type == BUILD) {
		/* If first was a build move, second must be movement move. */
		enumerate_valid_moves(game, false, false, true, second_move_callback, &second_ctx);
	} else if (move->type == CAPTURE) {
		/* If first was a build move, second can be either build or movement
		 * move. */
		enumerate_valid_moves(game, false, true, true, second_move_callback, &second_ctx);
	}
	revert_move(game, move);
}

void enumerate_valid_actions(struct game_t *game, void (*enumeration_callback)(struct game_t *game, const struct action_t *action, void *vctx), void *vctx) {
	struct first_move_ctx ctx = {
		.action_callback = enumeration_callback,
		.action_ctx = vctx,
	};
	enumerate_valid_moves(game, true, true, false, first_move_callback, &ctx);
}

bool game_won_by(struct game_t *game, enum side_t player) {
	uint8_t enemy_piece = (player == TRENCH) ? PIECE_CLIMB : PIECE_TRENCH;
	uint8_t player_piece = (player == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
	uint8_t enemy_base = (player == TRENCH) ? CANONICAL_LOCFLAG_CLIMB_BASE : CANONICAL_LOCFLAG_TRENCH_BASE;
	unsigned int enemy_count = 0;
	for (int i = 0; i < NUMBER_TILES(game->n); i++) {
		if (game->board->tiles[i] == player_piece) {
			if (game->canpos[i].loc_flags & enemy_base) {
				/* First winning condition: Our piece in the enemy base */
				return true;
			}
		} else if (game->board->tiles[i] == enemy_piece) {
			enemy_count++;
		}
	}

	/* Second possible winning condition: All enemies captured */
	return enemy_count == 0;
}

struct game_t* game_init(uint8_t n) {
	struct game_t *result = calloc(1, sizeof(struct game_t));
	if (!result) {
		return NULL;
	}
	result->n = n;
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
