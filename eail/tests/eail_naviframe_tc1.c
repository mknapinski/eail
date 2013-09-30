/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailNaviframe
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailNaviframe
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailNaviframe")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   g_object_ref(obj);
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 0);

   g_assert(atk_object_get_role(obj) == ATK_ROLE_PAGE_TAB_LIST);
   g_object_unref(obj);

   eailu_test_code_called = 1;
}

static void
_init_naviframe(Evas_Object *win)
{
   Evas_Object *nf;

   nf = elm_naviframe_add(win);
   evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, nf);
   evas_object_show(nf);
   evas_object_resize(win, 800, 600);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_naviframe(win);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
