/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * @file eail_fileselector_entry.c
 * @brief EailFileselectorEntry implementation
 */

#include <Elementary.h>

#include "eail_fileselector_entry.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_text_interface_init(AtkTextIface *iface);
static void eail_fileselector_entry_actions_init(EailActionWidget *widget);

#define ICON_CONTENT_NAME "button icon" /**< @brief content name for icon */
#define EAIL_FSE_ACTION_CLICK "click"/**< @brief 'click' action name */
#define EAIL_FSE_ACTION_LONGPRESS "longpress" /**< @brief 'longpress' action name */
#define EAIL_FSE_ACTION_PRESS "press" /**< @brief 'press' action name */
#define EAIL_FSE_ACTION_RELEASE "release" /**< @brief 'release' action name */

/**
 * @brief Definition of EailFileselectorEntry as GObject
 *
 * EailFileselectorEntry is extended EAIL_TYPE_ACTION_WIDGET with ATK_TYPE_TEXT
 * interface implemented.
 */
G_DEFINE_TYPE_WITH_CODE(EailFileselectorEntry,
                        eail_fileselector_entry,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Initializer for AtkObject
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_fileselector_entry_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_fileselector_entry_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_GROUPING;

   EailActionWidget *action_widget = EAIL_ACTION_WIDGET(obj);

   /* Initializing possible widget actions table*/
   eail_fileselector_entry_actions_init(action_widget);
}

/**
 * @brief Initializer for EailEntry GObject implementation
 *
 * @param fileselector_entry EailFileselectorEntry instance
 */
static void
eail_fileselector_entry_init(EailFileselectorEntry *fileselector_entry)
{
}

/**
 * @brief Gets the accessible name of FileselectorEntry
 *
 * Name is the label-text on the button.
 *
 * Implementation of get_name from AtkObject interface.
 *
 * @param obj AtkObject instance
 *
 * @returns character string representing the accessible name of
 * the accessible
 */
static const gchar *
eail_fileselector_entry_get_name(AtkObject *obj)
{
   Evas_Object *fse_widget = NULL;
   const gchar *atk_name = NULL;

   /* returning name from ATK default implementation if available */
   atk_name = ATK_OBJECT_CLASS(eail_fileselector_entry_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   return elm_object_part_text_get(fse_widget, "default");
}

/**
 * @brief Gets nested button from FileSelectorEntry
 *
 * @param obj AtkObject instance
 * @returns Evas_Object representing the file selector button
 * or NULL if one was not found
 */
static Evas_Object *
_eail_fileselector_entry_get_nested_button(AtkObject *obj)
{
   Evas_Object *button_widget = NULL, *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   button_widget = elm_object_part_content_get(fse_widget, "elm.swallow.button");
   if (!button_widget)
     DBG("Button widget NOT found!");

   return button_widget;
}

/**
 * @brief Helper function for returning list of nested widgets in fileselector entry
 *
 * @param obj AtkObject instance
 *
 * @return Eina_list representing the list of Evas_Object* objects with nested widgets
 */
static Eina_List *
_eail_fileselector_entry_get_items(AtkObject *obj)
{
   Eina_List *items = NULL;
   Evas_Object *icon_widget = NULL, *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   icon_widget = elm_object_part_content_get(fse_widget, ICON_CONTENT_NAME);
   if (!icon_widget) return NULL;

   items = eina_list_append(items, icon_widget);

   return items;
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * Implementation of get_n_children from AtkObject interface.
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_fileselector_entry_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = _eail_fileselector_entry_get_items(obj);
   n_items = eina_list_count(items);

   eina_list_free(items);

   return n_items;
}

/**
 * @brief Gets a reference to the specified accessible child of the object
 *
 * The accessible children are 0-based so the first accessible child
 * is at index 0, the second at index 1 and so on.
 *
 * Implementation of get_n_children from AtkObject interface.
 *
 * @param obj AtkObject instance
 * @param i index of the child
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_fileselector_entry_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items = NULL;
   AtkObject *child = NULL;

   items = _eail_fileselector_entry_get_items(obj);
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_accessible(eina_list_nth(items, i));
        if (child)  g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}

/**
 * @brief Destructor of FileselectorEntry class
 *
 * @param object GObject instance to be finalized
 */
static void
eail_fileselector_entry_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_fileselector_entry_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailFileselectorEntry GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailFileselectorEntryClass instance
 */
static void
eail_fileselector_entry_class_init(EailFileselectorEntryClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   atk_class->initialize = eail_fileselector_entry_initialize;
   atk_class->get_name = eail_fileselector_entry_get_name;
   atk_class->get_n_children = eail_fileselector_entry_get_n_children;
   atk_class->ref_child = eail_fileselector_entry_ref_child;
   gobject_class->finalize = eail_fileselector_entry_finalize;
}

/*
 * Implementation of the *AtkText* interface
 */

/**
 * @brief Gets text content from item
 *
 * Implementation of get_text from AtkTextIface.
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string
 *
 * @returns newly allocated string containing the text from start_offset
 * up to, but not including end_offset
 */
static gchar*
eail_fileselector_entry_get_text(AtkText  *text,
                                 gint      start_offset,
                                 gint      end_offset)
{
   gchar *ret_str = NULL;
   Evas_Object *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!fse_widget) return NULL;

   ret_str = eail_get_substring
                (elm_fileselector_entry_path_get(fse_widget),
                 start_offset, end_offset);

   return ret_str;
}

/**
 * @brief Gets the character from popup at the given offset
 *
 * Implementation of get_character_at_offset from AtkTextIface.
 *
 * @param text AtkText instance
 * @param offset character offset
 *
 * @returns char representing the character at offset
 */
static gunichar
eail_fileselector_entry_get_character_at_offset(AtkText *text,
                                                gint     offset)
{
   gunichar character = '\0';
   gchar *entry_str = NULL;

   entry_str = eail_fileselector_entry_get_text(text, 0, -1);
   if (entry_str)
      {
         character = g_utf8_get_char
                           (g_utf8_offset_to_pointer(entry_str, offset));
         g_free(entry_str);
      }

   return character;
}

/**
 * @brief Gets the number of characters from text content in item
 *
 * Implementation of get_character_count from AtkTextIface.
 *
 * @param text AtkText instance
 *
 * @returns integer representing the number of characters in text nontent
 */
static gint
eail_fileselector_entry_get_character_count(AtkText *text)
{
   gint count = 0;
   gchar *entry_str = NULL;
   entry_str = eail_fileselector_entry_get_text(text, 0, -1);

    if (entry_str)
      {
         count = g_utf8_strlen(entry_str, -1);
         g_free(entry_str);
      }

    return count;
}

/**
 * @brief Initializer for AtkTextIface interface
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
    iface->get_text = eail_fileselector_entry_get_text;
    iface->get_character_at_offset = eail_fileselector_entry_get_character_at_offset;
    iface->get_character_count = eail_fileselector_entry_get_character_count;
}

/*
 * Implementation of the Actions
 */

/**
 * @brief Calls callback with given name
 *
 * @param action AtkAction instance
 * @param action_name action name to be called
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_fileselector_entry_do_action(AtkAction *action,
                                  const char *action_name)
{
   Evas_Object *button = NULL;

   button = _eail_fileselector_entry_get_nested_button(ATK_OBJECT(action));
   if (!button) return FALSE;

   if (action_name == NULL) return FALSE;
   /*emit signal*/
   evas_object_smart_callback_call(button, action_name, NULL);
   return TRUE;
}

/**
 * @brief Handle for 'click' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_click(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "clicked");
}

/**
 * @brief Calls action with given name for actions based on x-y coordinates
 *
 * @param action AtkAction instance
 * @param name name string of the action to be called
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_action_handle_coords_action(AtkAction *action, gchar *name)
{
   Evas_Object *button = NULL;
   int x, y;

   if (name == NULL) return FALSE;

   button = _eail_fileselector_entry_get_nested_button(ATK_OBJECT(action));
   if (!button) return FALSE;

   /* getting coordinates of center of the widget to make sure, that
    * click will be performed on active widget area */
   eail_get_coords_widget_center(button, &x, &y);

   if (0 == g_strcmp0(name, EAIL_FSE_ACTION_PRESS))
        {
          DBG("Calling 'press' on button");
          eail_mouse_press_on_coords(button, x, y);
        }
   else if (0 == g_strcmp0(name, EAIL_FSE_ACTION_RELEASE))
      {
         DBG("Calling 'release' on button");
         eail_mouse_release_on_coords(button, x, y);
      }
   else
      {
         DBG("No action by given name %s", name);
         return FALSE;
      }

   return TRUE;
}
/**
 * @brief Handle for 'longpress' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_longpress(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "longpressed");
}

/**
 * @brief Handle for 'press' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_press(AtkAction *action, void *data)
{
   /* this is being done to action press/release raise properly click event.
    * Without that for standard behavior of fileselector entry, nothing will
    * happen (that why not launching event but emulating mouse press)*/
   return _eail_action_handle_coords_action(action, EAIL_FSE_ACTION_PRESS);
}

/**
 * @brief Handle for 'release' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_release(AtkAction *action, void *data)
{
   /* this is being done to action press/release raise properly click event.
    * Without that for standard behavior of fileselector entry, nothing will
    * happen (that why not launching event but emulating mouse release)*/
   return _eail_action_handle_coords_action(action, EAIL_FSE_ACTION_RELEASE);
}

/**
 * @brief Adds fileselector actions to actions table
 *
 * @param action_widget widget that implements EailActionWidget interface
 */
static void
eail_fileselector_entry_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_CLICK, NULL,
                                    eail_action_click);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_LONGPRESS, NULL,
                                    eail_action_longpress);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_PRESS, NULL,
                                    eail_action_press);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_RELEASE, NULL,
                                    eail_action_release);
}
