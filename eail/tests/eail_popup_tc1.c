/*
 * Tested interface: AtkText
 *
 * Tested AtkObject: EailPopup
 *
 * Description: Test AtkPopup interface
 *
 * Test input: accessible object representing EailPopup
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_POPUP "EailPopup"
#define EAIL_TEST_LABEL_TITTLE "Test popup obj"
#define EAIL_TEST_CONTENT_LABEL_TEXT "Test content label text"
#define EAIL_TEST_CONTENT_TEXT "seseeseese"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_popup_object_with_text(AtkObject *obj);

static void
_test_popup_object_with_label(AtkObject *obj);

static Evas_Object *popup = NULL;

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
_set_label_content_on_popup(Evas_Object *popup)
{
   Evas_Object *label_content;

   label_content = elm_label_add(popup);
   elm_object_text_set(label_content, EAIL_TEST_CONTENT_LABEL_TEXT);

   elm_object_content_set(popup, label_content);
}

static void
on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   /* testing popup with text content */
   eailu_traverse_children_for_type_call_cb
                  (aobj, EAIL_TYPE_FOR_POPUP, _test_popup_object_with_text);

   _set_label_content_on_popup(popup);
   /* testing popup with label content */
   eailu_traverse_children_for_type_call_cb
                  (aobj, EAIL_TYPE_FOR_POPUP, _test_popup_object_with_label);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_popup_object_with_text(AtkObject *obj)
{
   AtkText *text_obj = NULL;
   char *text = NULL;
   int child_amount = 0;

   atk_object_set_name(obj, EAIL_TEST_CONTENT_TEXT);
   g_assert_cmpstr(atk_object_get_name(obj), ==, EAIL_TEST_CONTENT_TEXT);
   _printf("Testing popup instance with text....\n");
   g_assert(ATK_IS_TEXT(obj));
   text_obj =  ATK_TEXT(obj);
   text = atk_text_get_text(text_obj, 0, -1);
   g_assert(text);
   g_assert(strcmp(text, EAIL_TEST_CONTENT_TEXT) == 0);
   g_assert((atk_text_get_character_count(text_obj)) ==
                                 (g_utf8_strlen(EAIL_TEST_CONTENT_TEXT, -1)));

   /* only 2 buttons should be listed as children when text content is set */
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(1 == child_amount);

   _printf("Tests passed successfully\n");
   eail_test_code_called = TRUE;
}

static void
_test_popup_object_with_label(AtkObject *obj)
{
   AtkText *text_obj = NULL;
   char *text = NULL;
   int child_amount = 0;
   AtkObject *nested_obj;
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   _printf("Testing popup instance with label....\n");
   g_assert(ATK_IS_TEXT(obj));
   text_obj =  ATK_TEXT(obj);
   text = atk_text_get_text(text_obj, 0, -1);
   g_assert(NULL == text);
   g_assert((atk_text_get_character_count(text_obj)) == 0);

   /* only 2 buttons + one widget with content should be listed as childrens */
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(2 == child_amount);

   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, "EailLabel"));
   g_assert(eailu_is_object_with_name
            (nested_obj, EAIL_TEST_CONTENT_LABEL_TEXT));

   _printf("Tests passed successfully\n");
}

static void
_response_cb(void *data, Evas_Object *obj,
             void *event_info)
{
   evas_object_hide(data);
}

static void
_initialize_and_show_popup(Evas_Object *win)
{
   Evas_Object *btn1, *btn2, *icon1;
   char buf[256];

   popup = elm_popup_add(win);

   icon1 = elm_icon_add(popup);
   snprintf(buf, sizeof(buf), "%s/images/logo_small.png",
            elm_app_data_dir_get());
   elm_image_file_set(icon1, buf, NULL );

   /* Seting popup title-text */
   elm_object_part_text_set(popup, "title,text", EAIL_TEST_LABEL_TITTLE);

   elm_object_text_set(popup,EAIL_TEST_CONTENT_TEXT);
   /* NOTE: RJ,list content of popup is not supported,
    * so not adding list items. It is because there is no possibility to
    * fetch those values from popup via popup API */

   /* Creating the first action button */
   btn1 = elm_button_add(popup);
   elm_object_text_set(btn1, "OK");

   /* NOTE: cannot add due to bug with popup inserting content for button3 */
   /*elm_object_part_content_set(popup, "button3", btn1);
     evas_object_smart_callback_add(btn1, "clicked", _response_cb, popup);
     */

   /* Creating the second action button */
   btn2 = elm_button_add(popup);
   elm_object_text_set(btn2, "Cancel");
   evas_object_smart_callback_add(btn2, "clicked", _response_cb, popup);

   /* Appending the second action button */
   elm_object_part_content_set(popup, "button1", btn2);

   /* Display the popup object */
   evas_object_show(popup);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_popup(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
