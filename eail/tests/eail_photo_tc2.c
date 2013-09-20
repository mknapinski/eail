/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailPhoto
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailPhoto
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

/* used for checking if photo was really clicked*/
static gboolean glob_test_photo_clicked = FALSE;

INIT_TEST("EailPhoto")

static void
_clicked_cb(void *data , Evas_Object *obj,
            void *event_info)
{
   glob_test_photo_clicked = TRUE;
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_action_description_all(ATK_ACTION(obj));

   g_assert(glob_test_photo_clicked);

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
   evas_object_smart_callback_add(photo, "clicked", _clicked_cb, NULL);
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

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_photo(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
