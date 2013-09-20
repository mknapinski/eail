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
 * @file eail_icon.c
 * @brief EailIcon implementation
 */

#include <Elementary.h>

#include "eail_icon.h"

/**
 * @brief EailIcon type definition
 */
G_DEFINE_TYPE(EailIcon, eail_icon, EAIL_TYPE_IMAGE);

/**
 * @brief EailIcon initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_icon_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_icon_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_ICON;
}

/**
 * @brief Gets obj name
 *
 * Returns obj name or obj icon file path if obj has no assigned name
 * or null if none of the previous applies
 *
 * @param obj an AtkObject
 * @returns obj name or obj icon file path
 */
static const gchar*
eail_icon_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget = NULL;

   name = ATK_OBJECT_CLASS(eail_icon_parent_class)->get_name(obj);
   if (name != NULL) return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     {
        name = (const gchar*)elm_icon_standard_get(widget);
        if (!name) elm_image_file_get(widget, &name, NULL);
     }

   return name;
}

/**
 * @brief EailIcon instance initializer
 *
 * @param icon an EailIcon
 */
static void
eail_icon_init(EailIcon *icon)
{
}

/**
 * @brief EailItem class initializer
 * @param klass an EailIcon class
 */
static void
eail_icon_class_init(EailIconClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_icon_initialize;
   class->get_name = eail_icon_get_name;
}
