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
 * @file eail_photo.c
 * @brief EailPhoto implementation
 */

#include <Elementary.h>

#include "eail_photo.h"
#include "eail_priv.h"

static void atk_image_iface_init(AtkImageIface *iface);

/**
 * @brief EailPhoto type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailPhoto, eail_photo, EAIL_TYPE_IMAGE,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_iface_init));


/**
 * @brief Gets the image size
 *
 * @param image AtkImage instance
 * @param [out] width photo width or -1 if value cannot be obtained
 * @param [out] height photo height or -1 if value cannot be obtained
 */
static void
eail_photo_size_get(AtkImage *image, gint *width, gint *height)
{
   g_return_if_fail(ATK_IS_IMAGE(image));

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }
   evas_object_geometry_get(widget, NULL, NULL, width, height);
}

/**
 * @brief EailPhoto initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_photo_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_photo_parent_class)->initialize(obj, data);
}

/**
 * @brief EailPhoto class initializer
 *
 * @param klass EailPhotoClass instance
 */
static void
eail_photo_class_init(EailPhotoClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   atk_class->initialize = eail_photo_initialize;
}

/**
 * @brief EailPhoto instance initializer
 *
 * @param photo EailPhoto instance
 */
static void
eail_photo_init(EailPhoto *photo)
{
}

/**
 * @brief AtkImage interface initializer
 *
 * @param iface AtkImage interface
 */
static void
atk_image_iface_init(AtkImageIface* iface)
{
   if (!iface) return;

   iface->get_image_size = eail_photo_size_get;
}

