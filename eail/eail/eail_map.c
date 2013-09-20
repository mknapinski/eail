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
 * @file eail_map.c
 * @brief EailMap implementation
 */

#include <Elementary.h>

#include "eail_map.h"
#include "eail_utils.h"

/**
 * @brief Press action name
 */
#define EAIL_MAP_ACTION_PRESS "press"

/**
 * @brief ScrollUp action name
 */
#define EAIL_MAP_ACTION_SCROLL_UP "scroll_up"

/**
 * @brief ScrollDown action name
 */
#define EAIL_MAP_ACTION_SCROLL_DOWN "scroll_down"

/**
 * @brief ScrollLeft action name
 */
#define EAIL_MAP_ACTION_SCROLL_LEFT "scroll_left"

/**
 * @brief ScrollRight action name
 */
#define EAIL_MAP_ACTION_SCROLL_RIGHT "scroll_right"

/**
 * @brief ZoomIn action name
 */
#define EAIL_MAP_ACTION_ZOOM_IN "zoom_in"

/**
 * @brief ZoomOut action name
 */
#define EAIL_MAP_ACTION_ZOOM_OUT "zoom_out"

/**
 * @brief Zoom increment
 */
#define EAIL_MAP_ZOOM_STEP 1

static void akt_image_interface_init(AtkImageIface *iface);

/**
 * @brief EailMap type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailMap,
                        eail_map,
                        EAIL_TYPE_IMAGE,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              akt_image_interface_init)) ;

static void eail_map_set_actions(AtkObject *obj);

/**
 * @brief EailMap initializer
 *
 * @param object an AtkObject
 * @param data initialization data
 */
static void
eail_map_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_map_parent_class)->initialize(object, data);
   object->role = ATK_ROLE_IMAGE_MAP;

   eail_map_set_actions(object);
}

/**
 * @brief EailMap finalizer
 *
 * @param object a GObject
 */
static void
eail_map_finalize(GObject *object)
{
   EailMap *map = EAIL_MAP(object);
   eina_stringshare_del(map->name);

   G_OBJECT_CLASS(eail_map_parent_class)->finalize(object);
}

/**
 * @brief Gets object name
 *
 * @param object an AtkObject
 * @return EailMap name if name is assigned, map path otherwise
 */
static const gchar*
eail_map_name_get(AtkObject *object)
{
   Evas_Object *widget;
   EailMap *map;
   const gchar *atk_name = NULL;

   /* returning name from ATK default implementation if available */
   atk_name = ATK_OBJECT_CLASS(eail_map_parent_class)->get_name(object);
   if (atk_name)
     return atk_name;

   g_return_val_if_fail(EAIL_IS_MAP(object), NULL);

   map = EAIL_MAP(object);
   if (map->name) return map->name;

   widget = eail_widget_get_widget(EAIL_WIDGET(map));
   if (!widget) return NULL;

   map->name =
           eina_stringshare_printf("Map(%s)",
                                   elm_map_source_get(widget,
                                                      ELM_MAP_SOURCE_TYPE_NAME));
   return map->name;
}

/**
 * @brief EailMap instance initializer
 *
 * @param map an EailMap
 */
static void
eail_map_init(EailMap *map)
{
   map->name = NULL;
}

/**
 * @brief EailMap class initializer
 *
 * @param klass an EailMapClass
 */
static void
eail_map_class_init(EailMapClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_map_initialize;
   class->get_name = eail_map_name_get;

   g_object_class->finalize = eail_map_finalize;
}

/**
 * @brief Gets image size
 *
 * Image size is the same as window size
 *
 * @param image an AtkImage
 * @param [out] width image width
 * @param [out] height image height
 */
static void
eail_map_image_size_get(AtkImage *image, gint *width, gint *height)
{
   atk_component_get_size(ATK_COMPONENT(image), width, height);
}

/**
 * @brief AtkImage interface initializer
 *
 * @param iface an AtkImage interface
 */
static void
akt_image_interface_init(AtkImageIface *iface)
{
   if (!iface) return;

   iface->get_image_size = eail_map_image_size_get;
}

/*
 * AtkAction implementation
 */

/**
 * @brief ScrollUp action callback
 *
 * @param action an AtkAction
 * @param data callback data
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_scroll_up(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   return eail_action_scroll_up(widget, NULL);
}

/**
 * @brief ScrollDown action callback
 *
 * @param action an AtkAction
 * @param data callback data
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_scroll_down(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   return eail_action_scroll_down(widget, NULL);
}

/**
 * @brief ScrollLeft action callback
 *
 * @param action an AtkAction
 * @param data callback data
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_scroll_left(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   return eail_action_scroll_left(widget, NULL);
}

/**
 * @brief ScrollRight action callback
 *
 * @param action an AtkAction
 * @param data callback data
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_scroll_right(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   return eail_action_scroll_right(widget, NULL);
}

/**
 * @brief ZoomIn action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_zoom_in(AtkAction *action, void *data)
{
   Evas_Object *widget;
   int new_zoom;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   new_zoom = elm_map_zoom_get(widget) + EAIL_MAP_ZOOM_STEP;
   elm_map_zoom_set(widget, new_zoom);

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
_eail_map_action_zoom_out(AtkAction *action, void *data)
{
   Evas_Object *widget;
   int new_zoom;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   new_zoom = elm_map_zoom_get(widget) - EAIL_MAP_ZOOM_STEP;

   elm_map_zoom_set(widget, new_zoom);

   return TRUE;
}

/**
 * @brief Press action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_map_action_press(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAP(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   evas_object_smart_callback_call(widget, "press", NULL);
   return TRUE;
}

/**
 * @brief Sets EailMap actions
 *
 * @param obj an AtkObject
 */
static void
eail_map_set_actions(AtkObject *obj)
{
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_PRESS,
                                    NULL,
                                    _eail_map_action_press);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_SCROLL_UP,
                                    NULL,
                                    _eail_map_action_scroll_up);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_SCROLL_DOWN,
                                    NULL,
                                    _eail_map_action_scroll_down);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_SCROLL_LEFT,
                                    NULL,
                                    _eail_map_action_scroll_left);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_SCROLL_RIGHT,
                                    NULL,
                                    _eail_map_action_scroll_right);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_ZOOM_IN,
                                    NULL,
                                    _eail_map_action_zoom_in);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(obj),
                                    EAIL_MAP_ACTION_ZOOM_OUT,
                                    NULL,
                                    _eail_map_action_zoom_out);
}
