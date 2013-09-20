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
 * @file eail_dayselector.c
 * @brief Implementation of eail representation of dayselector elementary widget
 */

#include <Elementary.h>

#include "eail_dayselector.h"
#include "eail_check.h"

/** @brief Number of days displayed in dayselector widget */
#define EAIL_DAYSELECTOR_NUM_OF_DAYS 7
/** @brief Content part name definition for dayselector widget */
#define EAIL_CONTENT_PART_FORMAT "day%d"

/**
 * @brief GObject definition for EailDayselector widget
 *
 * It extends EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE(EailDayselector, eail_dayselector, EAIL_TYPE_WIDGET);

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj an AtkObject
 * @param data initialization data (Evas_Object*)
 */
static void
eail_dayselector_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_dayselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_PANEL;
}

/**
 * @brief GObject-initializer for EailDayselector
 *
 * @param button an EailDayselector
 */
static void
eail_dayselector_init(EailDayselector *button)
{
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_dayselector_get_n_children(AtkObject *obj)
{
   return EAIL_DAYSELECTOR_NUM_OF_DAYS;
}

/**
 * @brief Helper func for generating content string name for given day number
 * @param day_num number of day in week
 *
 * @returns an Eina_Strbuf* object filled with content day-name
 */
static Eina_Strbuf *
_eail_dayselector_gen_day_str(int day_num)
{
   Eina_Strbuf *str_buf = NULL;

   str_buf = eina_strbuf_new();
   eina_strbuf_append_printf(str_buf, EAIL_CONTENT_PART_FORMAT, day_num);

   return str_buf;
}

/**
 * @brief Helper func for creating dayselector child content
 *
 * @param day_check_widget check widget that represents a day in dayselector
 * widget
 *
 * @returns an AtkObject representation of given day in dayselector. Unref
 * it when it is no longer needed.
 */
static AtkObject *
_eail_create_dayselector_child(Evas_Object *day_check_widget)
{
   AtkObject *child = NULL;
   child = g_object_new(EAIL_TYPE_CHECK, NULL);
   atk_object_initialize(child, day_check_widget);
   g_object_ref(child);

   return child;
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i child index
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_dayselector_ref_child(AtkObject *obj, gint i)
{
   AtkObject *child = NULL;
   Eina_Strbuf *part_name = NULL;
   Evas_Object *ds_widget = NULL, *day_check = NULL;

   g_return_val_if_fail (EAIL_IS_WIDGET(obj), NULL);

   ds_widget = eail_widget_get_widget(EAIL_WIDGET(obj) );
   part_name = _eail_dayselector_gen_day_str(i);
   day_check = elm_object_part_content_get
                  (ds_widget, eina_strbuf_string_get(part_name));

   /* dayselector consists of objects with type CHECK-widget */
   child = _eail_create_dayselector_child(day_check);

   eina_strbuf_free(part_name);
   return child;
}

/**
 * @brief Destructor for GObject Dayselector implementation
 *
 * @param object GObject object to be finalized
 */
static void
eail_dayselector_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_dayselector_parent_class)->finalize(object);
}

/**
 * @brief Initializer for GObject EailDayselectorClass class (defines callbacks
 * for base AtkObject)
 *
 * @param klass an EailDayselectorClass
 */
static void
eail_dayselector_class_init(EailDayselectorClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_dayselector_initialize;
   class->get_n_children = eail_dayselector_get_n_children;
   class->ref_child = eail_dayselector_ref_child;
   gobject_class->finalize = eail_dayselector_finalize;
}
