/*
 * Tested interface: AtkImage
 *
 * Tested AtkObject: EailPhotocam
 *
 * Description: Test AtkImage interface
 *
 * Test input: accessible object representing EailPhotocam
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailPhotocam")

static void
_do_test(AtkObject *obj)
{
   const char * const desc_test = "top secret";
   const char *desc;
   int height = 0, width  = 0;
   int x = -1, y = -1;

   g_assert(ATK_IS_IMAGE(obj));

   atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_SCREEN);
   _printf("atk_image_get_image_position on screen: x: %d y %d\n", x, y);

   g_assert(NULL == atk_image_get_image_description(ATK_IMAGE(obj)));
   g_assert(TRUE == atk_image_set_image_description(ATK_IMAGE(obj), desc_test));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   _printf("atk_image_get_image_description: %s\n", desc ? desc : "NULL");
   g_assert(NULL != desc);
   g_assert_cmpstr(desc_test, ==, desc);

   atk_image_get_image_size(ATK_IMAGE(obj), &height, &width);
   _printf("atk_image_get_image_size: height %d width %d\n", height, width);

   eailu_test_code_called = 1;
}

static void
_init_photocam(Evas_Object *win)
{
   Evas_Object *bg, *photocam;
   const char * const filename = "./data/whiterabbit01.jpg";

   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 255, 255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   photocam = elm_photocam_add(win);
   g_assert(EVAS_LOAD_ERROR_NONE == elm_photocam_file_set(photocam, filename));
   elm_scroller_bounce_set(photocam, EINA_FALSE, EINA_TRUE);
   evas_object_resize(photocam, 300, 300);
   evas_object_show(photocam);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_photocam(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
