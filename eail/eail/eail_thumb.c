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
 * @file eail_thumb.c
 * @brief EailThumb Implementation
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_thumb.h"
#include "eail_widget.h"

static void atk_image_iface_init(AtkImageIface *iface);

/**
 * @brief EailThumb type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailThumb,
                        eail_thumb,
                        EAIL_TYPE_IMAGE,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_iface_init));

/**
 * @brief EailThumb initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_thumb_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_thumb_parent_class)->initialize(obj, data);
}

/**
 * @brief EailThumb instance initializer
 *
 * @param thumb an EailThumb
 */
static void
eail_thumb_init(EailThumb *thumb)
{
}

/**
 * @brief Gets obj state set
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet*
eail_thumb_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;

   g_return_val_if_fail(EAIL_IS_THUMB(obj), NULL);

   state_set = ATK_OBJECT_CLASS(eail_thumb_parent_class)->ref_state_set(obj);
   if (!state_set)
     atk_state_set_add_state(state_set, ATK_STATE_ANIMATED);

   return state_set;
}

/**
 * @brief EailThumb class initializer
 *
 * @param klass an EailThumbClass
 */
static void
eail_thumb_class_init(EailThumbClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   class->initialize = eail_thumb_initialize;
   class->ref_state_set = eail_thumb_ref_state_set;
}

/**
 * @brief Gets thumb current size
 *
 * @param image an AtkImage
 * @param [out] width thumb current width
 * @param [out] height thumb current height
 */
static void
eail_thumb_size_get(AtkImage *image, gint *width, gint *height)
{
#ifdef ELM_ETHUMB
   Ethumb_Client *client;
#endif
   Evas_Object *widget;

   g_return_if_fail(EAIL_IS_THUMB(image));
#ifdef ELM_ETHUMB
   client = elm_thumb_ethumb_client_get();
   ethumb_client_size_get(client, width, height);
   return;
#endif
   widget = eail_widget_get_widget(EAIL_WIDGET(image));

   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }

   evas_object_geometry_get(widget, NULL, NULL, width, height);
}

/**
 * @brief AtkImage interface initializer
 *
 * @param iface an AtkImageIface
 */
static void
atk_image_iface_init(AtkImageIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_image_size = eail_thumb_size_get;
}
