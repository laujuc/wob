#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cmocka.h>

#include "src/config.h"

void
test_default_section_is_supported(void **state)
{
	(void) state;

	char tmp_path[] = "/tmp/wob-config-test-XXXXXX";
	int fd = mkstemp(tmp_path);
	assert_true(fd >= 0);

	const char *config_content =
		"[default]\n"
		"max = 42\n"
		"width = 9\n"
		"height = 1\n"
		"border_offset = 0\n"
		"border_size = 0\n"
		"bar_padding = 0\n";
	ssize_t written = write(fd, config_content, strlen(config_content));
	assert_int_equal(written, (ssize_t) strlen(config_content));
	close(fd);

	struct wob_config *config = wob_config_create();
	assert_non_null(config);

	bool loaded = wob_config_load(config, tmp_path);
	assert_true(loaded);
	assert_int_equal((int) config->max, 42);
	assert_int_equal((int) config->dimensions.width, 9);
	assert_int_equal((int) config->dimensions.height, 1);

	wob_config_destroy(config);
	unlink(tmp_path);
}

int
main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_default_section_is_supported),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
