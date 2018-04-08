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
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_RIGHT,
	},
	{
		.tile_index = 1,
		.n = 4,
		.row_number = 0,
		.col_number = 1,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT | ADJACENT_RIGHT,
	},
	{
		.tile_index = 2,
		.n = 4,
		.row_number = 0,
		.col_number = 2,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT | ADJACENT_RIGHT,
	},
	{
		.tile_index = 3,
		.n = 4,
		.row_number = 0,
		.col_number = 3,
		.row_width = 4,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_TOP | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_LEFT,
	},
	{
		.tile_index = 4,
		.n = 4,
		.row_number = 1,
		.col_number = 0,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT | ADJACENT_RIGHT | ADJACENT_TOP_RIGHT,
	},
	{
		.tile_index = 5,
		.n = 4,
		.row_number = 1,
		.col_number = 1,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 6,
		.n = 4,
		.row_number = 1,
		.col_number = 2,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 7,
		.n = 4,
		.row_number = 1,
		.col_number = 3,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 8,
		.n = 4,
		.row_number = 1,
		.col_number = 4,
		.row_width = 5,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
	},
	{
		.tile_index = 9,
		.n = 4,
		.row_number = 2,
		.col_number = 0,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
	},
	{
		.tile_index = 10,
		.n = 4,
		.row_number = 2,
		.col_number = 1,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 11,
		.n = 4,
		.row_number = 2,
		.col_number = 2,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 12,
		.n = 4,
		.row_number = 2,
		.col_number = 3,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 13,
		.n = 4,
		.row_number = 2,
		.col_number = 4,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 14,
		.n = 4,
		.row_number = 2,
		.col_number = 5,
		.row_width = 6,
		.loc_flags = CANONICAL_LOCFLAG_NORTH | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT | ADJACENT_BOTTOM_RIGHT,
	},
	{
		.tile_index = 15,
		.n = 4,
		.row_number = 3,
		.col_number = 0,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR | CANONICAL_LOCFLAG_LEFT,
		.adj_flags = ADJACENT_TOP_RIGHT | ADJACENT_RIGHT | ADJACENT_BOTTOM_RIGHT,
	},
	{
		.tile_index = 16,
		.n = 4,
		.row_number = 3,
		.col_number = 1,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 17,
		.n = 4,
		.row_number = 3,
		.col_number = 2,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 18,
		.n = 4,
		.row_number = 3,
		.col_number = 3,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 19,
		.n = 4,
		.row_number = 3,
		.col_number = 4,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 20,
		.n = 4,
		.row_number = 3,
		.col_number = 5,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR,
		.adj_flags = ADJACENT_MASK_ALL,
	},
	{
		.tile_index = 21,
		.n = 4,
		.row_number = 3,
		.col_number = 6,
		.row_width = 7,
		.loc_flags = CANONICAL_LOCFLAG_EQUATOR | CANONICAL_LOCFLAG_RIGHT,
		.adj_flags = ADJACENT_TOP_LEFT | ADJACENT_LEFT | ADJACENT_BOTTOM_LEFT,
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

