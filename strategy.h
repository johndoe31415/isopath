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

#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include "game.h"

struct strategy_t {
	float winning_coefficient;
	float threat_coefficient;
	float min_distance_coefficient;
	float sum_distance_coefficient;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void strategy_perform_move(struct game_t *game, const struct strategy_t *strategy);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
