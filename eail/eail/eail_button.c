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

static void atk_image_interface_init(AtkImageIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief Define EailButton GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailButton,
                        eail_button,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Implementation of get_image_position from AtkImage interface
 *
 * @param image EailButton instance
 * @param [out] x horizontal coordinate
 * @param [out] y vertical coordinate
 * @param coord_type coord type
 */
static void
eail_button_get_image_position(AtkImage     *image,
                               gint         *x,
                               gint         *y,
                               AtkCoordType  coord_type)
{
   Evas_Object *button_image;
   Evas_Object *widget;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *x = G_MININT;
        *y = G_MININT;
        return;
     }

   button_image = elm_object_part_content_get(widget, "icon");
   if (button_image)
     {
        obj = eail_factory_get_accessible(button_image);
        atk_image_get_image_position(ATK_IMAGE(obj), x, y, coord_type);
     }
   else
     {
        *x = G_MININT;
        *y = G_MININT;
     }
}

/**
 * @brief Implementation of get_image_size from AtkImage interface
 *
 * @param image EailButton instance
 * @param [out] width width image
 * @param [out] height height image
 */
static void
eail_button_get_image_size(AtkImage *image,
                           gint     *width,
                           gint     *height)
{
   Evas_Object *widget;
   Evas_Object *button_image;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }

   button_image = elm_object_part_content_get(widget, "icon");
   if (button_image)
     {
        obj = eail_factory_get_accessible(button_image);
        atk_image_get_image_size(ATK_IMAGE(obj), width, height);
     }
   else
     {
        *width = -1;
        *height = -1;
     }
}

/**
 * @brief AtkImage iterface initializer
 *
 * @param iface an AtkImageIface
 */
static void
atk_image_interface_init(AtkImageIface *iface)
{
   iface->get_image_position = eail_button_get_image_position;
   iface->get_image_size     = eail_button_get_image_size;
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string
 *
 * @param text an AtkText
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 *
 * @return string containing text from start_offset up to, but not including
 * end_offset
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
 * @brief Gets character at offset
 *
 * @param text an AtkText
 * @param offset character offset
 * @returns character at offset
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
 * @brief Gets text length
 *
 * @param text an AtkText
 * @returns text length
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
 * @param iface an AtkTextIface
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_button_get_text;
   iface->get_character_at_offset = eail_button_get_character_at_offset;
   iface->get_character_count = eail_button_get_character_count;
}

/**
 * @brief Notifies atk about button press event
 *
 * @param data data that was passed to event
 * @param obj an Evas_Object that received event
 * @param event_info additional event info
 */
static void
eail_button_atk_notify_press(void *data, Evas_Object *obj, void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_PRESSED, TRUE);
}

/**
 * @brief Notifies atk about button release event
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
static void
eail_button_atk_notify_release(void *data, Evas_Object *obj, void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_PRESSED, FALSE);
   eail_notify_child_focus_changes();
}

/**
 * @brief Click action callback
 *
 * @param action an AtkAction
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
 * @brief Press action callback
 *
 * @param action an AtkAction
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
 * @brief Release action callback
 *
 * @param action an AtkAction
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
 * @param action_widget an EailActionWidget
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
 * @param obj EailButton object
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
 * @brief Gets list of child widget
 *
 * @param widget EailWidget object
 *
 * @returns list of child, NULL if no children
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
 * @brief Gets obj accessible name
 *
 * @param obj an AtkObject
 *
 * @returns obj name
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
 * @param klass EailButton class
 */
static void
eail_button_class_init(EailButtonClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_button_initialize;
   class->get_name = eail_button_get_name;
   widget_class->get_widget_children = eail_button_get_widget_children;
}

/**
 * @brief EailButton instance initializer
 *
 * @param button an EailButton
 */
static void
eail_button_init(EailButton *button)
{
}
