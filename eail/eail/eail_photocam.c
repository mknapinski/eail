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
 * @file eail_photocam.c
 * @brief EailPhotocam implementation
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_photocam.h"

/**
 * @brief ZoomIn action name
 */
#define EAIL_PHOTOCAM_ACTION_ZOOM_IN "zoom_in"

/**
 * @brief ZoomOut action name
 */
#define EAIL_PHOTOCAM_ACTION_ZOOM_OUT "zoom_out"

/**
 * @brief Click action name
 */
#define EAIL_PHOTOCAM_ACTION_CLICK "click"

/**
 * @brief Zoom increment
 */
#define EAIL_PHOTOCAM_ZOOM_STEP 0.0625

static void atk_image_iface_init(AtkImageIface *iface);

/**
 * @brief EailPhotocam type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailPhotocam, eail_photocam,
                        EAIL_TYPE_SCROLLABLE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_iface_init));

/**
 * @brief ZoomIn action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_photocam_action_zoom_in(AtkAction *action, void *data)
{
   Evas_Object *widget;
   double zoom_level, set_zoom;

   g_return_val_if_fail(EAIL_IS_PHOTOCAM(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   zoom_level = elm_photocam_zoom_get(widget);
   set_zoom = zoom_level - EAIL_PHOTOCAM_ZOOM_STEP;
   if (set_zoom < 0.0) return FALSE;

   elm_photocam_zoom_set(widget, set_zoom);
   return TRUE;
}

/**
 * @brief ZoomOut action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_photocam_action_zoom_out(AtkAction *action, void *data)
{
   Evas_Object *widget;
   double zoom_level;

   g_return_val_if_fail(EAIL_IS_PHOTOCAM(action), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   zoom_level = elm_photocam_zoom_get(widget) + EAIL_PHOTOCAM_ZOOM_STEP;
   elm_photocam_zoom_set(widget, zoom_level);

   return TRUE;
}

/**
 * @brief Click action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_photocam_action_click(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_PHOTOCAM(action), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   evas_object_smart_callback_call(widget, "clicked", NULL);
   return TRUE;
}

/**
 * @brief EailPhotocam initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_photocam_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_photocam_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_IMAGE;

   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_PHOTOCAM_ACTION_ZOOM_IN,
                                    NULL,
                                    _eail_photocam_action_zoom_in);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_PHOTOCAM_ACTION_ZOOM_OUT,
                                    NULL,
                                    _eail_photocam_action_zoom_out);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_PHOTOCAM_ACTION_CLICK,
                                    NULL,
                                    _eail_photocam_action_click);
}

/**
 * @brief Gets obj state set
 *
 * The caller must unreference it when it is no longer needed
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet*
eail_photocam_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;

   state_set = ATK_OBJECT_CLASS(eail_photocam_parent_class)->ref_state_set(obj);
   return state_set;
}

/**
 * @brief EailPhotocam finalizer
 *
 * Frees allocated resources
 *
 * @param object a GObject
 */
static void
eail_photocam_finalize(GObject *object)
{
   EailPhotocam *photocam = EAIL_PHOTOCAM(object);
   if (photocam->description)
     free(photocam->description);

   G_OBJECT_CLASS(eail_photocam_parent_class)->finalize(object);
}

/**
 * @brief EailPhotocam class initializer
 *
 * @param klass an EailPhotocamClass
 */
static void
eail_photocam_class_init(EailPhotocamClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_photocam_initialize;
   class->ref_state_set = eail_photocam_ref_state_set;

   g_object_class->finalize = eail_photocam_finalize;
}

/**
 * @brief EailPhotocam instance intializer
 *
 * @param photocam an EailPhotocam
 */
static void eail_photocam_init(EailPhotocam *photocam)
{
   photocam->description = NULL;
}

/**
 * @brief Gets image size
 *
 * @param image an AtkImage
 * @param [out] width image width
 * @param [out] height image height
 */
static void
eail_photocam_image_size_get(AtkImage *image, gint *width, gint *height)
{
   g_return_if_fail(ATK_IS_IMAGE(image));
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }
   elm_photocam_image_size_get(widget, width, height);
}

/**
 * @brief Gets image description
 *
 * @param image an AtkImage
 * @return description
 */
static const char*
eail_photocam_image_description_get(AtkImage* image)
{
   EailPhotocam *photocam;

   g_return_val_if_fail(EAIL_IS_PHOTOCAM(image), NULL);
   photocam = EAIL_PHOTOCAM(image);

   return photocam->description;
}

/**
 * @brief Sets image description
 *
 * @param image an AtkImage
 * @param description new image description
 * @return TRUE if description is set successfully, FALSE otherwise
 */
static gboolean
eail_photocam_image_description_set(AtkImage *image, const char *description)
{
   EailPhotocam *photocam;

   g_return_val_if_fail(EAIL_IS_PHOTOCAM(image), FALSE);
   photocam = EAIL_PHOTOCAM(image);

   if (photocam->description) free(photocam->description);
   photocam->description = g_strdup(description);

   return TRUE;
}

/**
 * @brief Gets image position
 *
 * Position is in the form of a point specifying background top-left corner
 *
 * @param image an AtkImage
 * @param [out] x x coordinate
 * @param [out] y y coordinate
 * @param coord_type xy coordinate interpretation
 */
static void
eail_photocam_image_position_get(AtkImage *image,
                                 gint *x,
                                 gint *y,
                                 AtkCoordType coord_type)
{
   atk_component_get_position(ATK_COMPONENT(image), x, y, coord_type);
}

/**
 * @brief AktImage interface initializer
 *
 * @param iface an AtkImage interface
 */
static void
atk_image_iface_init(AtkImageIface *iface)
{
   if (!iface) return;

   iface->get_image_size = eail_photocam_image_size_get;
   iface->get_image_description = eail_photocam_image_description_get;
   iface->set_image_description = eail_photocam_image_description_set;
   iface->get_image_position = eail_photocam_image_position_get;
}
