#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <cmocka.h>

#include "src/color.h"
#include "src/image.h"

static void
test_centered_horizontal_bar_uses_actual_width(void **state)
{
	(void) state;

	const size_t width = 12;
	const size_t height = 4;
	uint32_t image_data[12 * 4];
	memset(image_data, 0, sizeof(image_data));

	struct wob_dimensions dimensions = {
		.width = width,
		.height = height,
		.border_offset = 0,
		.border_size = 0,
		.bar_padding = 0,
		.orientation = WOB_ORIENTATION_HORIZONTAL,
		.alignment = WOB_ALIGNMENT_CENTER,
	};
	struct wob_colors colors = {
		.background = {.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f},
		.border = {.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f},
		.value = {.a = 1.0f, .r = 1.0f, .g = 0.0f, .b = 0.0f},
	};

	wob_image_draw(image_data, dimensions, colors, 0.5);

	uint32_t background_color = wob_color_to_argb(wob_color_premultiply_alpha(colors.background));
	uint32_t bar_color = wob_color_to_argb(wob_color_premultiply_alpha(colors.value));

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			uint32_t expected_color = background_color;
			if (x >= 3 && x < 9) {
				expected_color = bar_color;
			}

			assert_int_equal(image_data[y * width + x], expected_color);
		}
	}
}

int
main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_centered_horizontal_bar_uses_actual_width),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
