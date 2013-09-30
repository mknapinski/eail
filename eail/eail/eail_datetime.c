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
 * @file eail_datetime.c
 *
 * @brief EailDatetime implementation
 */

#include <Elementary.h>

#include "eail_datetime.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailDatetime GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailDatetime,
                        eail_datetime,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
 * @brief EailDatetime object initialization
 *
 * @param obj EailDatetime instance
 * @param data additional initialization data
 */
static void
eail_datetime_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_datetime_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_DATE_EDITOR;
}

/**
 * @brief EailDatetime instance initialization
 *
 * @param datetime EailDatetime instance
 */
static void
eail_datetime_init(EailDatetime *datetime)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailDatetimeClass instance
 */
static void
eail_datetime_class_init(EailDatetimeClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_datetime_initialize;
}

/**
 * @brief Gets obj's current value
 *
 * Example:
 * @code
 * GValue value;
 * struct tm *t;
 * time_t time;
 *
 * //get current datetime value
 * atk_value_get_minimum_value(obj, &value);
 *
 * //get time
 * time = g_value_get_int(&value);
 *
 * //convert time to time structure (struct tm)
 * t = localtime(&time);
 * @endcode
 *
 * @param obj EailDatetime instance
 * @param [out] value obj's current value
 */
static void
eail_datetime_get_current_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm datetime;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_datetime_value_get(widget, &datetime);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&datetime));
}

/**
 * @brief Gets obj's maxiumum value
 *
 * @param obj EailDatetime instance
 * @param [out] value obj's maximum value
 */
static void
eail_datetime_get_maximum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm datetime;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_datetime_value_max_get(widget, &datetime);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&datetime));
}

/**
 * @brief Gets obj's minimum value
 *
 * @param obj EailDatetime instance
 * @param [out] value obj's minimum value
 */
static void
eail_datetime_get_minimum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm datetime;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_datetime_value_min_get(widget, &datetime);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&datetime));
}

/**
 * @brief Sets obj's value
 *
 * Example
 * @code
 * struct tm t;
 * time_t time;
 * GValue value;
 *
 * //Fill time structure with desired values(struct tm)
 * memset(&t, 0, sizeof(t));
 * t.tm_sec = 30;
 * t.tm_min = 20;
 * t.tm_hour = 10;
 *
 * //convert to int representation
 * time = mktime(&t);
 *
 * //copy time to value
 * memset(&value, 0, sizeof(value));
 * g_value_init(&value, G_TYPE_INT);
 * g_value_set_int(&value, time);
 *
 * //set time using atk
 * atk_value_set_current_value(obj, &value);
 * @endcode
 *
 * @param obj EailDatetime instance
 * @param value EailDatetime new value
 * @return TRUE if new value was set successfully, FALSE otherwise
 */
static gboolean
eail_datetime_set_current_value(AtkValue     *obj,
                                const GValue *value)
{
   Evas_Object *widget;
   struct tm datetime;
   time_t time;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   time = g_value_get_int(value);
   localtime_r(&time, &datetime);

   if (EINA_TRUE == elm_datetime_value_set(widget, &datetime))
     return TRUE;
   else
     return FALSE;
}

/**
 * @brief Gets obj's minimum increment
 *
 * @param obj EailDatetime instance
 * @param [out] value obj's minimum increment
 */
static void
eail_datetime_get_minimum_increment(AtkValue *obj,
                                    GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, 1);
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

   iface->get_current_value     = eail_datetime_get_current_value;
   iface->get_maximum_value     = eail_datetime_get_maximum_value;
   iface->get_minimum_value     = eail_datetime_get_minimum_value;
   iface->set_current_value     = eail_datetime_set_current_value;
   iface->get_minimum_increment = eail_datetime_get_minimum_increment;
}
