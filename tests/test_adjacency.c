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

#include "testbed.h"
#include <board.h>

static const struct canonical_position_t expected_pos[] = {
	{
		.tile_index = 0,
		.n = 4,
		.row_number = 0,
		.col_number = 0,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_LEFT | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 1, 3, 4, 5 },
	},
	{
		.tile_index = 1,
		.n = 4,
		.row_number = 0,
		.col_number = 1,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 0, 2, 5, 6 },
	},
	{
		.tile_index = 2,
		.n = 4,
		.row_number = 0,
		.col_number = 2,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 1, 3, 6, 7 },
	},
	{
		.tile_index = 3,
		.n = 4,
		.row_number = 0,
		.col_number = 3,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_RIGHT | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT,
		.adjacent_count = 4,
		.adjacent_tiles = { 0, 2, 7, 8 },
	},
	{
		.tile_index = 4,
		.n = 4,
		.row_number = 1,
		.col_number = 0,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_RIGHT | ADJACENT_TOP_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 0, 5, 9, 10 },
	},
	{
		.tile_index = 5,
		.n = 4,
		.row_number = 1,
		.col_number = 1,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 0, 1, 4, 6, 10, 11 },
	},
	{
		.tile_index = 6,
		.n = 4,
		.row_number = 1,
		.col_number = 2,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 1, 2, 5, 7, 11, 12 },
	},
	{
		.tile_index = 7,
		.n = 4,
		.row_number = 1,
		.col_number = 3,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 2, 3, 6, 8, 12, 13 },
	},
	{
		.tile_index = 8,
		.n = 4,
		.row_number = 1,
		.col_number = 4,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 3, 7, 13, 14 },
	},
	{
		.tile_index = 9,
		.n = 4,
		.row_number = 2,
		.col_number = 0,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 4, 10, 15, 16 },
	},
	{
		.tile_index = 10,
		.n = 4,
		.row_number = 2,
		.col_number = 1,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 4, 5, 9, 11, 16, 17 },
	},
	{
		.tile_index = 11,
		.n = 4,
		.row_number = 2,
		.col_number = 2,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 5, 6, 10, 12, 17, 18 },
	},
	{
		.tile_index = 12,
		.n = 4,
		.row_number = 2,
		.col_number = 3,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 6, 7, 11, 13, 18, 19 },
	},
	{
		.tile_index = 13,
		.n = 4,
		.row_number = 2,
		.col_number = 4,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 7, 8, 12, 14, 19, 20 },
	},
	{
		.tile_index = 14,
		.n = 4,
		.row_number = 2,
		.col_number = 5,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 8, 13, 20, 21 },
	},
	{
		.tile_index = 15,
		.n = 4,
		.row_number = 3,
		.col_number = 0,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR | CANONICAL_LOCFLAG_LEFT | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 9, 16, 21, 22 },
	},
	{
		.tile_index = 16,
		.n = 4,
		.row_number = 3,
		.col_number = 1,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 9, 10, 15, 17, 22, 23 },
	},
	{
		.tile_index = 17,
		.n = 4,
		.row_number = 3,
		.col_number = 2,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 10, 11, 16, 18, 23, 24 },
	},
	{
		.tile_index = 18,
		.n = 4,
		.row_number = 3,
		.col_number = 3,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 11, 12, 17, 19, 24, 25 },
	},
	{
		.tile_index = 19,
		.n = 4,
		.row_number = 3,
		.col_number = 4,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 12, 13, 18, 20, 25, 26 },
	},
	{
		.tile_index = 20,
		.n = 4,
		.row_number = 3,
		.col_number = 5,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 13, 14, 19, 21, 26, 27 },
	},
	{
		.tile_index = 21,
		.n = 4,
		.row_number = 3,
		.col_number = 6,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR | CANONICAL_LOCFLAG_RIGHT | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT,
		.adjacent_count = 4,
		.adjacent_tiles = { 14, 15, 20, 27 },
	},
	{
		.tile_index = 22,
		.n = 4,
		.row_number = 4,
		.col_number = 0,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 15, 16, 23, 28 },
	},
	{
		.tile_index = 23,
		.n = 4,
		.row_number = 4,
		.col_number = 1,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 16, 17, 22, 24, 28, 29 },
	},
	{
		.tile_index = 24,
		.n = 4,
		.row_number = 4,
		.col_number = 2,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 17, 18, 23, 25, 29, 30 },
	},
	{
		.tile_index = 25,
		.n = 4,
		.row_number = 4,
		.col_number = 3,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 18, 19, 24, 26, 30, 31 },
	},
	{
		.tile_index = 26,
		.n = 4,
		.row_number = 4,
		.col_number = 4,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 19, 20, 25, 27, 31, 32 },
	},
	{
		.tile_index = 27,
		.n = 4,
		.row_number = 4,
		.col_number = 5,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT,
		.adjacent_count = 4,
		.adjacent_tiles = { 20, 21, 26, 32 },
	},
	{
		.tile_index = 28,
		.n = 4,
		.row_number = 5,
		.col_number = 0,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 22, 23, 29, 33 },
	},
	{
		.tile_index = 29,
		.n = 4,
		.row_number = 5,
		.col_number = 1,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 23, 24, 28, 30, 33, 34 },
	},
	{
		.tile_index = 30,
		.n = 4,
		.row_number = 5,
		.col_number = 2,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 24, 25, 29, 31, 34, 35 },
	},
	{
		.tile_index = 31,
		.n = 4,
		.row_number = 5,
		.col_number = 3,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH,
		.adj_flags = ADJACENT_MASK_ALL,
		.adjacent_count = 6,
		.adjacent_tiles = { 25, 26, 30, 32, 35, 36 },
	},
	{
		.tile_index = 32,
		.n = 4,
		.row_number = 5,
		.col_number = 4,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT,
		.adjacent_count = 4,
		.adjacent_tiles = { 26, 27, 31, 36 },
	},
	{
		.tile_index = 33,
		.n = 4,
		.row_number = 6,
		.col_number = 0,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_LEFT | CANONICAL_LOCFLAG_BOTTOM | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 28, 29, 34, 36 },
	},
	{
		.tile_index = 34,
		.n = 4,
		.row_number = 6,
		.col_number = 1,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_BOTTOM,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 29, 30, 33, 35 },
	},
	{
		.tile_index = 35,
		.n = 4,
		.row_number = 6,
		.col_number = 2,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_BOTTOM,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_RIGHT,
		.adjacent_count = 4,
		.adjacent_tiles = { 30, 31, 34, 36 },
	},
	{
		.tile_index = 36,
		.n = 4,
		.row_number = 6,
		.col_number = 3,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_SOUTH | CANONICAL_LOCFLAG_RIGHT | CANONICAL_LOCFLAG_BOTTOM | CANONICAL_LOCFLAG_TELEPORT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_TOP_LEFT | ADJACENT_LEFT,
		.adjacent_count = 4,
		.adjacent_tiles = { 31, 32, 33, 35 },
	},
};


static void test_adjacency_pos(void) {
	subtest_start();
	for (int i = 0; i < sizeof(expected_pos) / sizeof(struct canonical_position_t); i++) {
		const struct canonical_position_t *expected = expected_pos + i;
		struct canonical_position_t calculated;
		debug("Tile index: %u\n", expected->tile_index);
		tile_index_to_canonical_pos(expected->tile_index, expected->n, &calculated);
		test_assert_int_eq(calculated.tile_index, expected->tile_index);
		test_assert_int_eq(calculated.n, expected->n);
		test_assert_int_eq(calculated.row_number, expected->row_number);
		test_assert_int_eq(calculated.col_number, expected->col_number);
		test_assert_int_eq(calculated.row_width, expected->row_width);
		test_assert_int_eq(calculated.loc_flags, expected->loc_flags);
		test_assert_int_eq(calculated.adj_flags, expected->adj_flags);
		test_assert_int_eq(calculated.adjacent_count, expected->adjacent_count);
		for (int j = 0; j < calculated.adjacent_count; j++) {
			test_assert_int_eq(calculated.adjacent_tiles[j], expected->adjacent_tiles[j]);
		}
		if (get_subtest_failure_count()) {
			fprintf(stderr, "Calculated:\n");
			dump_canonical_pos(&calculated);
			fprintf(stderr, "Expected:\n");
			dump_canonical_pos(expected);
		}
		abort_subtest_if_assertion_failure("Tile index %d had assertion failures.\n", i);
	}
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_adjacency_pos();
	test_finished();
	return 0;
}

