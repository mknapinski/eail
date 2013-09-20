#ifndef EAIL_TEST_UTILS_H_
#define EAIL_TEST_UTILS_H_

#include <atk/atk.h>
#include <Elementary.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INIT_TEST(type_name) \
    static int eailu_test_code_called; \
    static void _on_done(void *data, Evas_Object *obj, void *event_info) \
       { \
          elm_exit(); \
       } \
     static void _do_test(AtkObject *obj); \
     static void _on_focus_in(void *data, Evas_Object *obj, void *event_info) \
       { \
            AtkObject *aobj = atk_get_root(); \
            g_assert(aobj); \
            eailu_traverse_children_for_type_call_cb( \
                aobj, type_name, _do_test); \
            g_assert(eailu_test_code_called); \
            _on_done(NULL, NULL, NULL); \
       }

typedef void(* Eailu_Test_Func_Cb)(AtkObject *obj);

/**
 * Checks is AktObject has given type name
 */
gboolean eailu_is_object_with_type(AtkObject *obj, gchar *type_name);

/**
 * Checks if AtkObject has given object name
 */
gboolean eailu_is_object_with_name(AtkObject *obj, gchar *name);

/**
 * Checks if AtkObject has given atk role (AtkRole enum)
 */
gboolean eailu_is_object_with_role(AtkObject *obj, AtkRole role);

/**
 * Goes for every child in objects hierarchy looking for given type_name.
 * If given type has been found, then test_cb callback is called on it.
 *
 * @param obj Object to iterate on
 * @param type_Name string representing type or NULL if all objects should
 * be processed
 * @param test_cb Callback to test function which will be performed on every
 * matched object
 */
void eailu_traverse_children_for_type_call_cb(AtkObject *obj,
                                              gchar *type_name,
                                              Eailu_Test_Func_Cb test_cb);

/**
 * Goes for every child in objects hierarchy looking for given atk role.
 * If given type has been found, then test_cb callback is called on it.
 *
 * @param obj Object to iterate on
 * @param role representing atk role (AtkRole enum)
 * @param test_cb Callback to test function which will be performed on every
 * matched object
 */
void eailu_traverse_children_for_role_call_cb(AtkObject *obj,
                                              AtkRole role,
                                              Eailu_Test_Func_Cb test_cb);

/**
 * Gets object with given name from child hierarchy or NULL if not found
 */
AtkObject * eailu_find_child_with_name(AtkObject *obj, gchar *name);

/**
 * Initializes glib, gobject and creates EFL test window
 *
 */
Evas_Object * eailu_create_test_window_with_glib_init(Evas_Smart_Cb on_done,
                                                      Evas_Smart_Cb on_focus_in);

void eailu_print_atk_object_info(AtkObject *obj);

/**
 * Gets action number for given action name in AtkAction object
 *
 * @return returns action index number or -1 if action with given name has
 * not been found
 */
int eailu_get_action_number(AtkAction *action_obj,const gchar *action_name);

/* Executes test routing for action with given name. If something will be wrong
 * with given action, then assertion will fail */
void eailu_test_action_activate(AtkAction *action_obj,
                                    const gchar *action_name);

void eailu_test_action_description_all(AtkAction *action_obj);

/**
 * Tests "focus" functionality
 * @oaram obj: object to test
 * @param focusable: TRUE if focusable shoulbe be handled by component or
 * false otherwise
 */
void eailu_test_atk_focus(AtkObject *obj, gboolean focusable);

/**
 * Printf for debug
 */
inline static void _printf(const char *format, ...)
{
#if DEBUG
   va_list args;

   va_start(args, format);
   vfprintf(stdout, format, args);
   va_end(args);
#endif
}


#ifdef __cplusplus
}
#endif

#endif /* TEST_UTILS_H_ */
