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
 * @file eail_check.c
 * @brief EailChceck implementation
 */

#include <Elementary.h>

#include "eail_check.h"
#include "eail_utils.h"
#include "eail_factory.h"

static void atk_image_interface_init(AtkImageIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief Define EailCheck GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailCheck,
                        eail_check,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Implementation of get_image_position from AtkImage interface
 *
 * @param image Eailcheck instance
 * @param [out] x horizontal coordinate
 * @param [out] y vertical coordinate
 * @param coord_type coord type
 */
static void
eail_check_get_image_position(AtkImage     *image,
                               gint         *x,
                               gint         *y,
                               AtkCoordType  coord_type)
{
   Evas_Object *check_image;
   Evas_Object *widget;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *x = G_MININT;
        *y = G_MININT;
        return;
     }

   check_image = elm_object_part_content_get(widget, "icon");
   if (check_image)
     {
        obj = eail_factory_get_accessible(check_image);
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
 * @param image Eailcheck instance
 * @param [out] width width image
 * @param [out] height height image
 */
static void
eail_check_get_image_size(AtkImage *image,
                           gint     *width,
                           gint     *height)
{
   Evas_Object *widget;
   Evas_Object *check_image;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }

   check_image = elm_object_part_content_get(widget, "icon");
   if (check_image)
     {
        obj = eail_factory_get_accessible(check_image);
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
   iface->get_image_position = eail_check_get_image_position;
   iface->get_image_size     = eail_check_get_image_size;
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string
 *
 * @param text an AtkText
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 * @return string containing text from start_offset up to, but not including
 * end_offset
 */
static gchar*
eail_check_get_text(AtkText   *text,
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
 * @return character at offset
 */
static gunichar
eail_check_get_character_at_offset(AtkText    *text,
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
 * @return text length
 */
static gint
eail_check_get_character_count(AtkText *text)
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
 * Function called upon instance creation. It initializes AtkText interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface an AtkTextIface
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_check_get_text;
   iface->get_character_at_offset = eail_check_get_character_at_offset;
   iface->get_character_count = eail_check_get_character_count;
}


/**
 * @brief Callback for on_changed event
 * This will be called to notify AtkObject about object state change
 *
 * @param data callback data
 * @param obj object source
 * @param event_info event info
 */
static void
eail_check_atk_notify_change(void *data, Evas_Object *obj, void *event_info)
{
   Eina_Bool state = elm_check_state_get(obj);
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_CHECKED,
                                  state == EINA_TRUE ? TRUE : FALSE);
}


/**
 * @brief Action handler for 'click'
 *
 * @param action an AtkAction object (EailCheck)
 * @param data additional data (unused here)
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_check_action_click_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;
   Eina_Bool state;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   state = elm_check_state_get(widget);
   elm_check_state_set(widget, !state);

   evas_object_smart_callback_call(widget, "changed", NULL);
   eail_check_atk_notify_change(action, widget, NULL);

   return TRUE;
}

/**
 * @brief Initializer for actions defined in EailCheck
 *
 * @param action_widget an EailActionWidget implementation to be filled
 */
static void
eail_check_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget, "click", NULL,
                                    eail_check_action_click_cb);
}

/**
 * @brief Implementation of ref_state_set from AtkObject
 *
 * @param obj EailCheck instance
 *
 * @returns AtkStateSet for EailCheck instance
 */
static AtkStateSet*
eail_check_ref_state_set(AtkObject *obj)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   AtkStateSet *state_set =
       ATK_OBJECT_CLASS(eail_check_parent_class)->ref_state_set(obj);

   if (NULL == widget)
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
     }
   if (elm_check_state_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_CHECKED);
     }

   return state_set;
}

/**
 * @brief Gets obj accessible name
 *
 * @param obj an AtkObject
 *
 * @returns obj name
 */
static const gchar*
eail_check_get_name(AtkObject *obj)
{
   Evas_Object *widget;
   const char *atk_name;

   g_return_val_if_fail(EAIL_IS_CHECK(obj), NULL);

   atk_name = ATK_OBJECT_CLASS(eail_check_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   return elm_object_text_get(widget);
}

/**
 * @brief Gets list of child widget
 *
 * @param widget EailWidget object
 *
 * @return list of children, NULL if no children
 */
static Eina_List *
eail_check_get_widget_children(EailWidget *widget)
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
 * @brief EailCheck instance initialization
 *
 * @param check Eailheck instance
 */
static void
eail_check_init(EailCheck *check)
{
}

/**
 * @brief EailCheck object initializiation
 *
 * @param obj EailCheck object
 * @param data user set addtional initailization data
 */
static void
eail_check_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_check_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_CHECK_BOX;

   evas_object_smart_callback_add((Evas_Object*) data, "changed",
                                  eail_check_atk_notify_change,
                                  obj);

   eail_check_actions_init(EAIL_ACTION_WIDGET(obj));
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailCheck class
 */
static void
eail_check_class_init(EailCheckClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_check_initialize;
   class->ref_state_set = eail_check_ref_state_set;
   class->get_name = eail_check_get_name;
   widget_class->get_widget_children = eail_check_get_widget_children;
}
