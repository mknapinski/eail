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
 * @file eail_fileselector.c
 * @brief Fileselector implementation
 */

#include <Elementary.h>

#include "eail_fileselector.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

/**
 * @brief Definition of EailFileselector as GObject
 *
 * EailFileselector is extended EAIL_TYPE_LAYOUT
 */
G_DEFINE_TYPE(EailFileselector, eail_fileselector, EAIL_TYPE_LAYOUT);

/**
 * @brief Initializer for Fileselector AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_fileselector_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_fileselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_FILE_CHOOSER;
}

/**
 * @brief Gets obj's accessible name
 *
 * @param obj AtkObject instance
 *
 * @returns string representing obj's name
 */
static const gchar*
eail_fileselector_get_name(AtkObject *obj)
{
   Evas_Object *widget;
   const char *atk_name;

   g_return_val_if_fail(EAIL_IS_FILESELECTOR(obj), NULL);

   atk_name = ATK_OBJECT_CLASS(eail_fileselector_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   return elm_object_text_get(widget);
}

/**
 * @brief EailFileselector GObject instance initializer
 *
 * @param fileselector EailFileselector instance
 */
static void
eail_fileselector_init(EailFileselector *fileselector)
{
}

/**
 * @brief Destructor for EailFileselector
 *
 * @param object GObject object to be finalized
 */
static void
eail_fileselector_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_fileselector_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailFileselector GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailFileselectorClass instance
 */
static void
eail_fileselector_class_init(EailFileselectorClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   atk_class->initialize = eail_fileselector_initialize;
   atk_class->get_name = eail_fileselector_get_name;
   gobject_class->finalize = eail_fileselector_finalize;
}
