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
#include "game.h"
#include "strategy.h"

int main(int argc, char **argv) {
	struct game_t *game = game_init(4);
	board_dump(game->board);
	struct strategy_t strategy = {
		.winning_coefficient = 1000,
		.threat_coefficient = 100,
		.min_distance_coefficient = 10,
		.sum_distance_coefficient = 3,
	};
	strategy_play_out(game, &strategy, &strategy);
	game_free(game);
	return 0;
}
