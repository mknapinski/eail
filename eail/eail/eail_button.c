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
 * @file eail_button.c
 * @brief EailButton implementation
 */

#include <Elementary.h>
#include "eail_button.h"
#include "eail_factory.h"
#include "eail_priv.h"
#include "eail_utils.h"

static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief Define EailButton GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailButton,
                        eail_button,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 *
 * @return string containing the text from start_offset up to,
 * but not including end_offset
 */
static gchar*
eail_button_get_text(AtkText   *text,
                     gint       start_offset,
                     gint       end_offset)
{
   gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     string = (gchar *)elm_object_text_get(widget);

   return eail_get_substring(string, start_offset, end_offset);
}

/**
 * @brief Gets the character at offset
 *
 * @param text AtkText instance
 * @param offset character offset
 * @returns char representing the character at offset
 */
static gunichar
eail_button_get_character_at_offset(AtkText    *text,
                                    gint        offset)
{
   gunichar character = '\0';
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     character = g_utf8_get_char(
         g_utf8_offset_to_pointer(elm_object_text_get(widget), offset));

   return character;
}

/**
 * @brief Gets the text length
 *
 * @param text AtkText instance
 * @returns integer representing the text length
 */
static gint
eail_button_get_character_count(AtkText *text)
{
   gint count = 0;
   const gchar *string_text = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return count;

   string_text = elm_object_text_get(widget);
   if (!string_text) return count;

   count = g_utf8_strlen(string_text, -1);

   return count;
}

/**
 * @brief AtkText interface initializer
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_button_get_text;
   iface->get_character_at_offset = eail_button_get_character_at_offset;
   iface->get_character_count = eail_button_get_character_count;
}

/**
 * @brief Notifies ATK about button press event
 *
 * @param data data passed to event
 * @param obj Evas_Object instance that received event
 * @param event_info additional event info
 */
static void
eail_button_atk_notify_press(void *data, Evas_Object *obj, void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_PRESSED, TRUE);
}

/**
 * @brief Notifies ATK about button release event
 *
 * @param data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
static void
eail_button_atk_notify_release(void *data, Evas_Object *obj, void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_PRESSED, FALSE);
   eail_notify_child_focus_changes();
}

/**
 * @brief 'click' action callback
 *
 * @param action AtkAction instance
 * @param data user data passed to callback
 *
 * @returns TRUE if action was successfull, FALSE otherwise
 */
static gboolean
eail_button_action_click_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   evas_object_smart_callback_call(widget, "clicked", NULL);
   eail_button_atk_notify_press(action, NULL, NULL);

   return TRUE;
}

/**
 * @brief 'press' action callback
 *
 * @param action AtkAction instance
 * @param data user data passed to callback
 *
 * @returns TRUE if action was successfull, FALSE otherwise
 */
static gboolean
eail_button_action_press_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   evas_object_smart_callback_call(widget, "pressed", NULL);
   eail_button_atk_notify_press(action, NULL, NULL);

   return TRUE;
}

/**
 * @brief 'release' action callback
 *
 * @param action AtkAction instance
 * @param data user data passed to callback
 *
 * @returns TRUE if action was successfull, FALSE otherwise
 */
static gboolean
eail_button_action_release_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   evas_object_smart_callback_call(widget, "unpressed", NULL);
   eail_button_atk_notify_release(action, NULL, NULL);

   return TRUE;
}

/**
 * @brief Sets EailButton actions
 *
 * @param action_widget EailActionWidget instance
 */
static void
eail_button_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    "click", NULL,
                                    eail_button_action_click_cb);
   eail_action_widget_action_append(action_widget,
                                    "press", NULL,
                                    eail_button_action_press_cb);
   eail_action_widget_action_append(action_widget,
                                    "release", NULL,
                                    eail_button_action_release_cb);
}

/**
 * @brief EailButton object initialization
 *
 * @param obj AtkObject instance
 * @param data user set additional initialization data
 */
static void
eail_button_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_button_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_PUSH_BUTTON;

   evas_object_smart_callback_add((Evas_Object*) data, "pressed",
                                  eail_button_atk_notify_press, obj);
   evas_object_smart_callback_add((Evas_Object*) data, "unpressed",
                                  eail_button_atk_notify_release, obj);

   eail_button_actions_init(EAIL_ACTION_WIDGET(obj));
}

/**
 * @brief Gets the list of widget's children
 *
 * @param widget EailWidget instance
 *
 * @returns Eina_list representing the list of children
 * or NULL if widget has no children
 */
static Eina_List *
eail_button_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));
   if (obj)
     {
        child = elm_object_part_content_get(obj, "icon");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);
     }

   return list;
}

/**
 * @brief Gets the accessible name of obj
 *
 * @param obj AtkObject instance
 *
 * @returns string representing the accessible name of obj
 */
static const gchar*
eail_button_get_name(AtkObject *obj)
{
   Evas_Object *widget;
   const char *atk_name;

   g_return_val_if_fail(EAIL_IS_BUTTON(obj), NULL);

   atk_name = ATK_OBJECT_CLASS(eail_button_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   return elm_object_text_get(widget);
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailButtonClass instance
 */
static void
eail_button_class_init(EailButtonClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   atk_class->initialize = eail_button_initialize;
   atk_class->get_name = eail_button_get_name;
   widget_class->get_widget_children = eail_button_get_widget_children;
}

/**
 * @brief EailButton instance initializer
 *
 * @param button EailButton instance
 */
static void
eail_button_init(EailButton *button)
{
}
