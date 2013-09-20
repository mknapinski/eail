/*
 * Tested interface: AtkImage
 *
 * Tested AtkObject: EailThumb
 *
 * Description: Test AtkImage interface
 *
 * Test input: accessible object representing EailThumb
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

INIT_TEST("EailThumb")

static void
_init_thumb(Evas_Object *win)
{
   const char *filename = "./data/06.jpg";
   Evas_Object *bg, *thumb;

   elm_need_ethumb();

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   thumb = elm_thumb_add(win);
   elm_thumb_editable_set(thumb, EINA_FALSE);
   elm_thumb_file_set(thumb, filename, NULL);
   elm_thumb_reload(thumb);
   evas_object_size_hint_weight_set(thumb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, thumb);
   evas_object_show(thumb);
}

static void
_do_test(AtkObject *obj)
{
   const char * const desc_test = "top secret";
   const char *desc;
   int height = 0, width = 0;
   int x = -1, y = -1;

   g_assert(ATK_IS_IMAGE(obj));

   atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_SCREEN);

   g_assert(NULL == atk_image_get_image_description(ATK_IMAGE(obj)));
   g_assert(TRUE == atk_image_set_image_description(ATK_IMAGE(obj), desc_test));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   g_assert(NULL != desc);
   g_assert_cmpstr(desc_test, ==, desc);

   atk_image_get_image_size(ATK_IMAGE(obj), &height, &width);
   g_assert(height == 128 && width == 128);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_thumb(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
