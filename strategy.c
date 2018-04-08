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
#include <string.h>
#include "strategy.h"

struct evaluated_action_t {
	struct action_t action;
	float goodness;
};

struct action_callback_ctx_t {
	const struct strategy_t *strategy;
	unsigned int action_cnt;
	struct evaluated_action_t *actions;
};

struct piece_distances_t {
	int min_distance;
	int sum_distance;
};

static void game_evaluate_distances(struct piece_distances_t *distances, struct game_t *game, int target_row, enum side_t side) {
	uint8_t player_piece = (side == TRENCH) ? PIECE_TRENCH : PIECE_CLIMB;
	distances->sum_distance = 0;
	distances->min_distance = 1000;
	for (int i = 0; i < NUMBER_TILES(game->n); i++) {
		if (game->board->tiles[i] == player_piece) {
			int distance = target_row - game->canpos[i].row_number;
			if (distance < 0) {
				distance = -distance;
			}
			if (distance < distances->min_distance) {
				distances->min_distance = distance;
			}
			distances->sum_distance += distance;
		}
	}
}

static float evaluate_board_side(struct game_t *game, const struct strategy_t *strategy, enum side_t side) {
	float result = 0;
	if (game_won_by(game, side)) {
		result += strategy->winning_coefficient;
	}

	struct piece_distances_t distances;
	int target_row = (side == TRENCH) ? 0 : ((game->n * 2) - 2);
	game_evaluate_distances(&distances, game, target_row, side);
	result -= strategy->min_distance_coefficient * distances.min_distance;
	result -= strategy->sum_distance_coefficient * distances.sum_distance;

	/*
	if (piece_threatened(game, side)) {
		result += strategy->threat_coefficient;
	}
	*/
	return result;
}

static float evaluate_board(struct game_t *game, const struct strategy_t *strategy) {
	float our_goodness = evaluate_board_side(game, strategy, game->side_turn);
	float enemy_goodness = evaluate_board_side(game, strategy, (game->side_turn == TRENCH) ? CLIMB : TRENCH);
	return our_goodness - enemy_goodness;
}

static void enumeration_callback(struct game_t *game, const struct action_t *action, void *vctx) {
	struct action_callback_ctx_t *ctx = (struct action_callback_ctx_t *)vctx;
	ctx->actions = realloc(ctx->actions, sizeof(struct evaluated_action_t) * (ctx->action_cnt + 1));
	memcpy(&ctx->actions[ctx->action_cnt].action, action, sizeof(struct action_t));
	ctx->actions[ctx->action_cnt].goodness = evaluate_board(game, ctx->strategy);
	ctx->action_cnt += 1;
}

void strategy_perform_move(struct game_t *game, const struct strategy_t *strategy) {
	struct action_callback_ctx_t ctx = {
		.strategy = strategy,
		.action_cnt = 0,
		.actions = NULL,
	};
	enumerate_valid_actions(game, enumeration_callback, &ctx);

	if (ctx.action_cnt == 0) {
		fprintf(stderr, "fatal: no valid actions enumeratable!\n");
		abort();
	}

	float max_goodness = ctx.actions[0].goodness;
	unsigned int preferred_option = 0;
//	printf("%u options\n", ctx.action_cnt);
	for (int i = 1; i < ctx.action_cnt; i++) {
		if (ctx.actions[i].goodness > max_goodness) {
			max_goodness = ctx.actions[i].goodness;
			preferred_option = i;
		}
//		printf("option %d: %f\n", i, ctx.actions[i].goodness);
	}
//	dump_action(&ctx.actions[preferred_option]);
	game_perform_action(game, &ctx.actions[preferred_option].action);
	board_dump(game->board);
	printf("\n");
	free(ctx.actions);
}

bool strategy_play_out(struct game_t *game, const struct strategy_t *our_strategy, const struct strategy_t *their_strategy) {
	enum side_t us = game->side_turn;
	enum side_t them = (us == TRENCH) ? CLIMB : TRENCH;
	while (true) {
		/* Our turn */
		strategy_perform_move(game, our_strategy);
		if (game_won_by(game, us)) {
			return true;
		}

		/* Their turn */
		strategy_perform_move(game, their_strategy);
		if (game_won_by(game, them)) {
			return false;
		}
	}
}
