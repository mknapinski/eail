#include <atk/atk.h>
#include <Elementary.h>
#include <string.h>

#include "eail_test_utils.h"

#define EAILU_WINDOW_TEST_TITLE "Eail test window"
#define EAILU_TEST_WINDOW_WIDTH 420
#define EAILU_TEST_WINDOW_HEIGHT 420

gboolean
eailu_is_object_with_type(AtkObject *obj, gchar *type_name)
{
   const char *obj_type = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   if ((!type_name) || (!obj)) return FALSE;

   return !(strcmp(obj_type, type_name));
}

gboolean
eailu_is_object_with_name(AtkObject *obj, gchar *name)
{
   const char *obj_name = NULL;

   if (!ATK_IS_OBJECT(obj))
     return FALSE;

   obj_name = atk_object_get_name(obj);
   if ((!obj_name) || (!name))
     return FALSE;

   return !(strcmp(obj_name, name));
}

gboolean
eailu_is_object_with_role(AtkObject *obj, AtkRole role)
{
   AtkRole obj_role = ATK_ROLE_INVALID;

   if (!ATK_IS_OBJECT(obj))
     return FALSE;

   obj_role = atk_object_get_role(obj);

   return (obj_role == role);
}

AtkObject *
eailu_find_child_with_name(AtkObject *obj, gchar *name)
{
   int child_count = 0, i = 0;

   if (eailu_is_object_with_name(obj, name))
     return obj;

   child_count = atk_object_get_n_accessible_children(obj);
   for (i = 0; i < child_count; i++)
     {
        AtkObject *found_obj = NULL;
        AtkObject *child = atk_object_ref_accessible_child(obj, i);

        found_obj = eailu_find_child_with_name(child, name);
        if (found_obj != NULL)
          return found_obj;

        g_object_unref(child);
     }

   return NULL;
}

void
eailu_traverse_children_for_type_call_cb(AtkObject *obj,
                                         gchar *type_name,
                                         Eailu_Test_Func_Cb test_cb)
{
   int child_count = atk_object_get_n_accessible_children(obj);
   int i;
   for (i = 0; i < child_count; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        if (ATK_IS_OBJECT(child))
          {
             if (type_name == NULL ||
                      eailu_is_object_with_type(child, type_name))
               test_cb(child);
          }

        eailu_traverse_children_for_type_call_cb(child, type_name, test_cb);

        g_object_unref(child);
     }
}

void
eailu_traverse_children_for_role_call_cb(AtkObject *obj,
                                         AtkRole role,
                                         Eailu_Test_Func_Cb test_cb)
{
   gint number_of_children = atk_object_get_n_accessible_children(obj);
   gint children_count;

   for (children_count = 0; number_of_children > children_count; children_count++)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, children_count);

        if (eailu_is_object_with_role(child, role))
          test_cb(child);

        eailu_traverse_children_for_role_call_cb(child, role, test_cb);
        g_object_unref(child);
     }
}

Evas_Object *
eailu_create_test_window_with_glib_init(Evas_Smart_Cb on_done,
                                        Evas_Smart_Cb on_focus_in)
{
   Evas_Object *win;

   if (!ecore_main_loop_glib_integrate())
     {
        fprintf(stderr, "Cannot integrate with glib main loop");
        return NULL;
     }

#if !GLIB_CHECK_VERSION(2,35,0)
   g_type_init();
#endif

   win = elm_win_add(NULL, EAILU_WINDOW_TEST_TITLE, ELM_WIN_BASIC);
   elm_win_title_set(win, EAILU_WINDOW_TEST_TITLE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   evas_object_smart_callback_add(win, "delete,request", on_done, NULL);
   elm_win_autodel_set(win, EINA_TRUE);

   evas_object_smart_callback_add(win, "focus,in", on_focus_in, NULL);
   evas_object_resize(win, EAILU_TEST_WINDOW_WIDTH, EAILU_TEST_WINDOW_HEIGHT);

   return win;
}

void
eailu_test_atk_focus(AtkObject *obj, gboolean focusable)
{
   AtkStateSet *state_set = NULL;

   if (focusable)
     {
        /* checking is setting focus works properly */
        state_set = atk_object_ref_state_set(obj);

        g_assert(atk_state_set_contains_state(state_set, ATK_STATE_FOCUSABLE));
        g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));

        /* grabbing focus */
        g_assert(ATK_IS_COMPONENT(obj));

        atk_component_grab_focus(ATK_COMPONENT(obj));
        state_set = atk_object_ref_state_set(obj);
        g_assert(
            atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));
     }
    else
     {
        state_set = atk_object_ref_state_set(obj);
        g_assert(
             !atk_state_set_contains_state(state_set, ATK_STATE_FOCUSABLE));
        g_assert(
             !atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));
     }
}

void
_eailu_print_atk_text_obj(AtkText *atk_text)
{
   int count = 0, i = 0;
   char *text = atk_text_get_text(atk_text, 0, -1);

   printf("ATK TEXT:\n");
   printf("atk_text_get_text: %s\n", text ? text : "NULL");
   count = atk_text_get_character_count(atk_text);
   printf("atk_text_get_character_count: %d\n", count);

   printf("Text from offset single chars: ");
   for (i = 0; i < count; ++i)
     {
        printf("%c", atk_text_get_character_at_offset(atk_text, i));
     }
   printf("\n");
}

void
eailu_print_atk_object_info(AtkObject *obj)
{
   const char *name = NULL;
   const char *type_name = NULL;
   const char *role_name = NULL;
   AtkRole atk_role = ATK_ROLE_INVALID;

   name = atk_object_get_name(ATK_OBJECT(obj));
   type_name = g_type_name(G_TYPE_FROM_INSTANCE(ATK_OBJECT(obj)));
   atk_role = atk_object_get_role(ATK_OBJECT(obj));
   role_name = atk_role_get_name(atk_role);

   printf("atk_object_get_name: %s\n", name ? name : "NULL");
   printf("atk_object_get_role: %s\n", role_name ? role_name : "NULL");
   printf("atk_object_get_type_name: %s\n", type_name ? type_name : "NULL");

   if (ATK_IS_TEXT(obj))
     {
        _eailu_print_atk_text_obj(ATK_TEXT(obj));
     }

   printf("\n");
}

/**
 * @return returns action index number or -1 if action with given name has
 * not been found
 */
int
eailu_get_action_number(AtkAction *action_obj, const gchar *action_name)
{
   int i = 0;
   int action_index = -1;
   const int actions_num = atk_action_get_n_actions(action_obj);

   for (i = 0; i < actions_num; i++)
     {
        const char *local_name = atk_action_get_name(action_obj, i);
        if ((strcmp(action_name, local_name)) == 0)
          return i;
     }

   return action_index;
}

void
eailu_test_action_activate(AtkAction *action_obj, const gchar *action_name)
{
   int action_index = -1;

   action_index = eailu_get_action_number(action_obj, action_name);
   g_assert(-1 != action_index);
   g_assert(TRUE == atk_action_do_action(action_obj, action_index));
}

void
eailu_test_action_description_all(AtkAction *action_obj)
{
   int actions_num = atk_action_get_n_actions(action_obj);
   const char *desc_set = "description test";
   const char *desc_get;

   int count;
   for (count = 0; count < actions_num; ++count)
     {
        g_assert(atk_action_get_name(action_obj, count));
        g_assert(atk_action_set_description(action_obj, count, desc_set));
        desc_get = atk_action_get_description(action_obj, count);
        g_assert(desc_get);
        g_assert(!strcmp(desc_set, desc_get));
     }

   g_assert(NULL == atk_action_get_name(action_obj, actions_num));
   g_assert(FALSE == atk_action_set_description(action_obj, actions_num, desc_set));
   g_assert(NULL == atk_action_get_description(action_obj, actions_num));
}
