/*
 * Tested interface: AtkImage
 *
 * Tested AtkObject: EailBackground
 *
 * Description: Test AtkImage interface
 *
 * Test input: accessible object representing EailBackground
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <Ecore_X.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define DEBUG

#define TEST_STR "bg-test"
#define IMG_PATH "./data/08.jpg"
#define X 50
#define Y 60
#define WIDTH 320
#define HEIGHT 280

INIT_TEST("EailBackground")

static void
_do_test(AtkObject *obj)
{
   const char *desc;
   int w, h;

   g_assert(ATK_IS_IMAGE(obj));

   g_assert(atk_image_set_image_description(ATK_IMAGE(obj), TEST_STR));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   g_assert(!g_strcmp0(desc, TEST_STR));

   atk_image_get_image_size(ATK_IMAGE(obj), &w, &h);
   g_assert(w == WIDTH);
   g_assert(h == HEIGHT);

   atk_image_get_image_position(ATK_IMAGE(obj), &w, &h, ATK_XY_WINDOW);
   g_assert(w == 0);
   g_assert(h == 0);

   eailu_test_code_called = 1;
}

static void
_init_background(Evas_Object *win)
{
   Evas_Object *bg;
   char buf[PATH_MAX];

   bg = elm_bg_add(win);
   elm_bg_load_size_set(bg, 20, 20);
   elm_bg_option_set(bg, ELM_BG_OPTION_STRETCH);
   g_snprintf(buf, sizeof(buf), IMG_PATH);
   elm_bg_file_set(bg, buf, NULL);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   evas_object_resize(win, WIDTH, HEIGHT);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_background(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
