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
 * @file eail_prefs.c
 * @brief EailPrefs widget implementation.
 */

#include <Elementary.h>

#include "eail_prefs.h"
#include "eail_factory.h"
#include "eail_priv.h"

/**
 * @brief EailPrefs GObject type definition
 *
 * EailPrefs is extended EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE(EailPrefs, eail_prefs, EAIL_TYPE_WIDGET);

/**
 * @brief EailPrefs initializer.
 *
 * @param obj EailPrefs instance
 * @param data initialization data
 */
static void
eail_prefs_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_prefs_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_REDUNDANT_OBJECT;
}

/**
 * @brief EailPrefs instance initializer
 *
 * @param prefs EailPrefs instance
 */
static void
eail_prefs_init(EailPrefs *prefs)
{
}

/**
 * @brief Gets children for prefs object
 *
 * @param parent parent widget
 * @param items empty list (used as output value)
 */
static void eail_prefs_children_get(Evas_Object *parent,
                                    Eina_List **items)
{
   Eina_List *list;
   Evas_Object *w;
   int n;

   list = evas_object_smart_members_get(parent);
   if (!list) return;

   for (n = 0; n < eina_list_count(list); ++n)
     {
        w = eina_list_nth(list, n);
        if (elm_object_widget_check(w))
          {
             *items = eina_list_append(*items, w);
          }

        eail_prefs_children_get(w, items);
     }

   eina_list_free(list);
}

/**
 * @brief Gets list of children for given widget
 *
 * @param widget an EailWidget (EailPrefs) to take children from
 *
 * @returns filled Eina_List with widget children or NULL if no children found
 */
static Eina_List *
eail_prefs_get_widget_children(EailWidget *widget)
{
    Evas_Object *w;
    Eina_List *list = NULL;

    g_return_val_if_fail(EAIL_IS_PREFS(widget), NULL);

    w = eail_widget_get_widget(widget);
    if (!widget) return NULL;

    eail_prefs_children_get(w, &list);

    return list;
}

/**
 * @brief EailPrefs class initializer.
 *
 * Here basic callbacks for AtkObject are defined
 *
 * @param klass EailPrefs class
 */
static void
eail_prefs_class_init(EailPrefsClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_prefs_initialize;
   widget_class->get_widget_children = eail_prefs_get_widget_children;
}

