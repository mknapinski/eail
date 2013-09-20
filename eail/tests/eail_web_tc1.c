/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailWeb
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailWeb
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailWeb")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   child_count =  atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 0);

   eailu_test_atk_focus(obj, TRUE);

   _printf("DONE. All WEB tests passed successfully \n");
   eailu_test_code_called = 1;
}

static void
_default_web_content_set(Evas_Object *web)
{
#ifdef HAVE_ELEMENTARY_WEB
   Evas_Object *view, *frame;
   const char contents[] = ""
       "<html>\n"
       "  <head>\n"
       "    <title>Nothing to see here, move along</title>\n"
       "  </head>\n"
       "  <body>\n"
       "    <a href=\"http://www.enlightenment.org\" target=\"_blank\">E</a>\n"
       "    <br />\n"
       "    <a href=\"http://www.google.com\" target=\"_blank\">Google</a>\n"
       "    <br />\n"
       "  </body>\n"
       "</html>\n";

   view = elm_web_webkit_view_get(web);
   frame = ewk_view_frame_main_get(view);
   ewk_frame_contents_set(frame, contents, sizeof(contents) - 1, "text/html",
                          "UTF-8", NULL);
#else
   _printf("Webview could not be initialized - NO WEBKIT SUPPORT ON PLATFORM\n");
   _printf("However still raw web widget implementation CAN be tested \n");
   (void) web;
#endif
}

static void
_init_webview(Evas_Object *win)
{
   Evas_Object *web;//, *box;

   //   box = elm_box_add(win);
   //   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   //   elm_win_resize_object_add(win, box);
   //   evas_object_show(box);

   web = elm_web_add(win);
   evas_object_size_hint_weight_set(web, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(web, EVAS_HINT_FILL, 0.5);
   _default_web_content_set(web);

   //elm_box_pack_end(box, web);

   evas_object_show(web);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   _init_webview(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
