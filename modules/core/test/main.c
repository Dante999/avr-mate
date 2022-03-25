#include <unity_fixture.h>

static void run_all_tests()
{
	RUN_TEST_GROUP(graphx);
	RUN_TEST_GROUP(protocol);
}

int main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, run_all_tests);
}

#if 0
int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_graphx_draw_pixel);
	RUN_TEST(test_graphx_draw_hline);
	RUN_TEST(test_graphx_draw_vline);
	RUN_TEST(test_graphx_fill);
	RUN_TEST(test_graphx_draw_rect);
	RUN_TEST(test_graphx_draw_tile_4x16);
	RUN_TEST(test_graphx_draw_tile_1x64);
	RUN_TEST(test_graphx_draw_tile_128x8);
	RUN_TEST(test_graphx_draw_tile_128x64);

	return UNITY_END();
}
#endif