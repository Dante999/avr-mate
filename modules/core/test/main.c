#include <unity_fixture.h>

static void run_all_tests()
{
	RUN_TEST_GROUP(graphx);
	RUN_TEST_GROUP(protocol);
	RUN_TEST_GROUP(handheld_cartridge);
}

int main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, run_all_tests);
}
