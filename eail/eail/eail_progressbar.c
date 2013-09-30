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
 * @file eail_progressbar.c
 * @brief EailProgressbar implementation
 */

#include <Elementary.h>

#include "eail_progressbar.h"
#include "eail_utils.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailProgressbar GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailProgressbar,
                        eail_progressbar,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Handler for changed event
 *
 * @param data data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
void
_eail_progressbar_handle_changed_event(void *data,
                                       Evas_Object *obj,
                                       void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief EailProgressbar object initialization
 *
 * @param obj AtkObject instance
 * @param data user set additional initialization data
 */
static void
eail_progressbar_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;

   ATK_OBJECT_CLASS(eail_progressbar_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PROGRESS_BAR;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   evas_object_smart_callback_add(nested_widget, "changed",
                                  _eail_progressbar_handle_changed_event, obj);

}

/**
 * @brief Gets the list of child widgets
 *
 * @param widget EailWidget object
 *
 * @return Eina_List representing the list of widget's children
 * or NULL if widget has no children
 */
static Eina_List *
eail_progressbar_get_widget_children(EailWidget *widget)
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
 * @brief Gets the slider's name
 *
 * @param obj AtkObject instance
 * @returns string representing the accessible name of the slider
 */
static const gchar*
eail_progressbar_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget;

   name = ATK_OBJECT_CLASS(eail_progressbar_parent_class)->get_name(obj);
   if (name) return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailProgressbar instance initialization
 *
 * @param progressbar EailProgressbar instance
 */
static void
eail_progressbar_init(EailProgressbar *progressbar)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailProgressbarClass instance
 */
static void
eail_progressbar_class_init(EailProgressbarClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   atk_class->initialize = eail_progressbar_initialize;
   atk_class->get_name = eail_progressbar_get_name;
   widget_class->get_widget_children = eail_progressbar_get_widget_children;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets the current value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's current value
 */
static void
eail_progressbar_get_current_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, elm_progressbar_value_get(widget));
}

/**
 * @brief Gets the maxiumum value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's maxiumum value
 */
static void
eail_progressbar_get_maximum_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 1.0);
}

/**
 * @brief Gets the minimum value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum value
 */
static void
eail_progressbar_get_minimum_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 0.0);
}

/**
 * @brief Sets the value of obj
 *
 * @param obj AtkValue instance
 * @param value obj's new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_progressbar_set_current_value(AtkValue *obj,
                                   const GValue *value)
{
   Evas_Object *widget;
   double val;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   val = g_value_get_double(value);

   if (0.0 > val || 1.0 < val)
     return FALSE;

   elm_progressbar_value_set(widget, val);

   return TRUE;
}

/**
 * @brief Gets the minimum increment of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum increment
 */
static void
eail_progressbar_get_minimum_increment(AtkValue *obj,
                                       GValue *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, G_MINDOUBLE);
}

/**
 * @brief AtkValue interface initializer
 *
 * @param iface AtkValueIface instance
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value     = eail_progressbar_get_current_value;
   iface->get_maximum_value     = eail_progressbar_get_maximum_value;
   iface->get_minimum_value     = eail_progressbar_get_minimum_value;
   iface->set_current_value     = eail_progressbar_set_current_value;
   iface->get_minimum_increment = eail_progressbar_get_minimum_increment;
}
