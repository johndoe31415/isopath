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

#ifndef __GAME_H__
#define __GAME_H__

#include <stdint.h>
#include <stdbool.h>
#include "board.h"

enum movetype_t {
	BUILD,
	MOVE,
	CAPTURE,
};

struct move_t {
	enum movetype_t type;
	unsigned int src_tile;
	unsigned int dst_tile;
};

struct action_t {
	struct move_t moves[2];
};

enum side_t {
	TRENCH,
	CLIMB,
};

struct game_t {
	uint8_t n;
	enum side_t side_turn;
	struct canonical_position_t *canpos;
	struct board_t *board;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool is_action_legal(struct game_t *game, const struct action_t *action);
bool perform_action(struct game_t *game, const struct action_t *action);
struct game_t* game_init(uint8_t n);
void game_free(struct game_t *game);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
