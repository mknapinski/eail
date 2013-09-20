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
 * @file eail_calendar.c
 * @brief EailCalendar implementation
 */

#include <Elementary.h>
#include "eail_calendar.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailCalendar GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailCalendar,
                        eail_calendar,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
 * @brief EailCalendar object initialization
 *
 * Main task is to set role for AtkObject.
 *
 * @param  obj an AtkObject
 * @param  data initialization data
 */
static void eail_calendar_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_calendar_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_CALENDAR;
}

/**
 * @brief EailCalendar instance initialization
 *
 * EailCalendar does not have any fields to initialize in struct EailCalendar
 * thus this function is emp but still it has to be defined.
 *
 * @param  calendar EailCalendar instance
 */
static void
eail_calendar_init(EailCalendar *calendar)
{
}

/**
 * @brief EailCalendar class initialization function
 *
 * Purpose of this function is to initialize AtkObjectClass method pointers
 * with class's implementations.
 *
 * @param  klass EailCalendar class
 */
static void
eail_calendar_class_init(EailCalendarClass *klass)
{
   AtkObjectClass *class;

   class = ATK_OBJECT_CLASS(klass);
   class->initialize = eail_calendar_initialize;
}

/**
 * @brief Gets obj current value
 *
 * Gets callendar date
 *
 * Example:
 * @code
 * AtkObject *obj; // AtkObject representing EailCalendar
 * GValue value = G_VALUE_INIT;
 * eail_calendar_get_current_value(ATK_VALUE(obj), &value);
 * @endcode
 *
 * @param obj an AtkValue
 * @param [out] value obj current value for date
 */
static void
eail_calendar_get_current_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm current;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_selected_time_get(widget, &current);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&current));
}

/**
 * @brief Gets obj maxiumum value
 *
 * Gets maximum date that can be set for callendar.
 *
 * Example:
 * @code
 * AtkObject *obj; //AtkObject representing EailCalendar
 * GValue value = G_VALUE_INIT;
 * eail_calendar_get_maximum_value(ATK_VALUE(obj), &value);
 * @endcode
 *
 * @param  obj an AtkValue
 * @param [out] value obj maxiumum value
 */
static void
eail_calendar_get_maximum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm maximum;
   int min, max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&maximum, 0, sizeof(maximum));

   if (min < max)
     {
        maximum.tm_year = max - 1900;
        maximum.tm_mon = 11;
        maximum.tm_mday = 31;
        maximum.tm_hour = 23;
        maximum.tm_min = 59;
        maximum.tm_sec = 59;
     }

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&maximum));
}

/**
 * @brief Gets obj minimum value
 *
 * Gets minimum date than can be set for callendar
 *
 * Example:
 * @code
 * AtkObject *obj; //AtkObject representing EailCalendar
 * GValue value = G_VALUE_INIT;
 * eail_calendar_get_minimum_value(ATK_VALUE(obj), &value);
 * @endcode
 *
 * @param obj an AtkObject
 * @param [out] value obj minimum value
 */
static void
eail_calendar_get_minimum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm minimum;
   int min, max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&minimum, 0, sizeof(minimum));

   if (min < max)
     {
        minimum.tm_year = min - 1900;
        minimum.tm_mon = 0;
        minimum.tm_mday = 1;
        minimum.tm_hour = 0;
        minimum.tm_min = 0;
        minimum.tm_sec = 0;
     }

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&minimum));
}

/**
 * @brief Sets obj value
 *
 * Sets calendar date.
 *
 * Example:
 * @code
 * AtkObject *obj; // AtkObject representing EailCalendar
 * GValue value = G_VALUE_INIT;
 * struct tm time;
 *
 * memset(&tm, 0, sizeof(time));
 * time.tm_year = 120;  // years since 1900
 * time.tm_mon = 2;
 * time.tm_mday = 15;
 * time.tm_hour = 7;
 * time.tm_min = 21;
 * time.tm_sec = 33;
 *
 * g_value_set_int(&value, mktime(&time));
 *
 * eail_calendar_set_current_value(ATK_VALUE(obj), &value);
 * @endcode
 *
 * @param  obj an AtkValue
 * @param  value obj new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_calendar_set_current_value(AtkValue     *obj,
                                const GValue *value)
{
   Evas_Object *widget;
   struct tm minimum, maximum, current_set;
   int min, max;
   time_t time;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&minimum, 0, sizeof(minimum));
   minimum.tm_year = min - 1900;
   minimum.tm_mon = 0;
   minimum.tm_mday = 1;
   minimum.tm_hour = 0;
   minimum.tm_min = 0;
   minimum.tm_sec = 0;

   memset(&maximum, 0, sizeof(maximum));
   maximum.tm_year = max - 1900;
   maximum.tm_mon = 11;
   maximum.tm_mday = 31;
   maximum.tm_hour = 23;
   maximum.tm_min = 59;
   maximum.tm_sec = 59;

   time = g_value_get_int(value);

   if ((min < max) && (mktime(&minimum) > time || mktime(&maximum) < time))
     return FALSE;

   localtime_r(&time, &current_set);
   elm_calendar_selected_time_set(widget, &current_set);

   return TRUE;
}

/**
 * @brief Gets obj minimum increment
 *
 * Gets minimum calendar increment i.e one second.
 *
 * Example:
 *
 * @code
 * AtkObject *obj;
 * GValue value = G_VALUE_INIT;
 * eail_calendar_get_minimum_increment(ATK_VALUE(obj), &value);
 * @endcode
 *
 * @param obj an AtkValue
 * @param [out] value obj minimum increment
 */
static void
eail_calendar_get_minimum_increment(AtkValue *obj,
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
 * @brief Initialization of AtkValue interface
 *
 * Function called upon instance creation. It initializes AtkValue interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface an AtkObject than implements AtkValueInterface
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   iface->get_current_value     = eail_calendar_get_current_value;
   iface->get_maximum_value     = eail_calendar_get_maximum_value;
   iface->get_minimum_value     = eail_calendar_get_minimum_value;
   iface->set_current_value     = eail_calendar_set_current_value;
   iface->get_minimum_increment = eail_calendar_get_minimum_increment;
}
