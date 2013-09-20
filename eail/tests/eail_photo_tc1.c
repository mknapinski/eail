/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailPhoto
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailPhoto
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define DEBUG 1

INIT_TEST("EailPhoto")

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));

   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE);

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   g_assert(!atk_state_set_is_empty(state_set));
   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

static void
_init_photo(Evas_Object *win)
{
   Evas_Object *bx, *photo;
   const char * const filename = "data/th-whiterabbit01.jpg";

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_TRUE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_homogeneous_set(bx, EINA_TRUE);
   elm_win_resize_object_add(win, bx);

   photo = elm_photo_add(win);
   g_assert(elm_photo_file_set(photo, filename));
   elm_photo_size_set(photo, 100);
   elm_photo_aspect_fixed_set(photo, EINA_FALSE);
   evas_object_size_hint_weight_set(photo, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_photo_fill_inside_set(photo, EINA_TRUE);
   evas_object_size_hint_align_set(photo, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, photo);
   evas_object_show(photo);

   evas_object_show(bx);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   g_assert(win);
   _init_photo(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
