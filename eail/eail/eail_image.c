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
 * @file eail_image.c
 * @brief EailImage implementation
 */

#include <Elementary.h>
#include "eail_image.h"

/**
 * @brief click action name
 */
#define EAIL_IMAGE_ACTION_CLICK "click"

static void atk_image_interface_init(AtkImageIface *iface);

/**
 * @brief EailImage type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailImage,
                        eail_image,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_interface_init));

/**
 * @brief Callback for "click" action on elm_image object
 *
 * @param action EailImage instance
 * @param data user data passed to callback
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_image_action_click(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_IMAGE(action), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

    evas_object_smart_callback_call(widget, "clicked", NULL);

    return TRUE;
}

/*
 * Implementation of the *AtkObject* interface
 */

/**
 *
 * @brief Initialize EailImage instance
 *
 * @param obj EailImage instance
 * @param data user data passed to initialization
 */
static void
eail_image_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_image_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_IMAGE;

   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_IMAGE_ACTION_CLICK,
                                    NULL,
                                    _eail_image_action_click);
}

/**
 * @brief Finalize EailImage instance
 *
 * @param object EailImage instance
 */
static void
eail_image_finalize(GObject *object)
{
   EailImage *obj;

   obj = EAIL_IMAGE(object);
   if (obj->description) free(obj->description);

   G_OBJECT_CLASS(eail_image_parent_class)->finalize(object);
}

/**
 * @brief Init EailImage instance
 *
 * @param image EalImage instance
 */
static void
eail_image_init(EailImage *image)
{
   image->description = NULL;
}

/**
 * @brief Implementation on ref_state_set from AtkObject
 *
 * @param obj EailImage instance
 *
 * @returns referred state set object
 */
static AtkStateSet *
eail_image_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   Eina_Bool resizable_up, resizable_down;

   if (!widget)
     {
        return NULL;
     }

   state_set = ATK_OBJECT_CLASS(eail_image_parent_class)->ref_state_set(obj);
   if (!evas_object_smart_type_check(widget, "elm_image"))
     return state_set;

   if (elm_image_animated_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_ANIMATED);
     }

   elm_image_resizable_get(widget, &resizable_up, &resizable_down);
   if ((resizable_up) || (resizable_down))
     {
        atk_state_set_add_state(state_set, ATK_STATE_RESIZABLE);
     }

   return state_set;
}

/**
 * @brief EailImage class initialization
 *
 * @param klass EailImageClass object
 */
static void
eail_image_class_init(EailImageClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_image_initialize;
   class->ref_state_set = eail_image_ref_state_set;

   g_object_class->finalize = eail_image_finalize;
}

/*
 * Implementation of the *AtkImage* interface
 */

/**
 * @brief Implementation of get_image_postition from AtkImage interface
 *
 * @param image EailImage instance
 * @param x address of int to store x coordinate
 * @param y address of int to store y coordinate
 * @param coord_type specifies whether the coordinates are relative to the screen or to the components top level window
 */
static void
eail_image_position_get(AtkImage *image,
                        gint *x,
                        gint *y,
                        AtkCoordType coord_type)
{
   atk_component_get_position(ATK_COMPONENT(image), x, y, coord_type);
}

/**
 * @brief Implementation of get_image_size from AtkImage interface
 *
 * @param image EailImage instance
 * @param width address of int to store width
 * @param height address of int to store height
 */
static void
eail_image_size_get(AtkImage *image, gint *width, gint *height)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }

   elm_image_object_size_get(widget, width, height);
}

/**
 * @brief Implementation of get_image_description from AtkImage interface
 *
 * @param image EailImage instance
 *
 * @returns image description
 */
static const gchar *
eail_image_description_get(AtkImage *image)
{
   EailImage *obj = EAIL_IMAGE(image);
   return obj->description;
}

/**
 * @brief Implementation of set_image_description from AtkImage interface
 *
 * @param image EailImage instance
 * @param description image description to set
 *
 * @returns TRUE on success
 */
static gboolean
eail_image_description_set(AtkImage *image, const gchar *description)
{
   EailImage *obj = EAIL_IMAGE(image);

   if (obj->description)
     free(obj->description);

   obj->description = strdup(description);

   return TRUE;
}

/**
 * @brief Gets image locale (LC_MESSAGES variable)
 *
 * @param image an AtkImage
 * @return NULL if LC_MESSAGES not set, locale string otherwise
 */
const gchar *
eail_image_get_image_locale(AtkImage *image)
{
    return setlocale(LC_MESSAGES, NULL);
}

/**
 * @brief Initialize AtkImage interface
 *
 * @param iface EailImage object
 */
static void
atk_image_interface_init(AtkImageIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_image_position    = eail_image_position_get;
   iface->get_image_size        = eail_image_size_get;
   iface->get_image_description = eail_image_description_get;
   iface->set_image_description = eail_image_description_set;
   iface->get_image_locale      = eail_image_get_image_locale;
}
