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
 * @file eail_background.c
 * @brief EailBackground implementation
 */

#include <Elementary.h>

#include "eail_background.h"

static void atk_image_interface_init(AtkImageIface *iface);

/**
 * @brief EailBackground type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailBackground,
                        eail_background,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_interface_init));

/* AtkObject */
static void eail_background_initialize(AtkObject *obj, gpointer data);
static void eail_background_finalize(GObject *object);
static AtkAttributeSet *eail_background_get_attributes(AtkObject *obj);

/* AtkImage */
static void eail_background_get_image_position (AtkImage *image,
                                                gint *x,
                                                gint *y,
                                                AtkCoordType coord_type);
static const gchar *eail_background_get_image_description(AtkImage *image);
static gboolean eail_background_set_image_description(AtkImage *image,
                                                      const gchar *description);
static void eail_background_get_image_size(AtkImage *image,
                                           gint *width,
                                           gint *height);
const gchar *eail_background_get_image_locale(AtkImage *image);


/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailBackground class initializer
 *
 * @param klass EailBackgroundClass instance
 */
static void
eail_background_class_init(EailBackgroundClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *object_class = G_OBJECT_CLASS(klass);

   atk_class->initialize = eail_background_initialize;
   atk_class->get_attributes = eail_background_get_attributes;

   object_class->finalize = eail_background_finalize;
}

/**
 * @brief EailBackground initializer
 *
 * @param obj AtkObject (EailBackground) instance to be initialized
 * @param data additional data
 */
static void
eail_background_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_background_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_IMAGE;
}

/**
 * @brief EailBackground finalizer
 *
 * Destroys the object and allocated resources.
 *
 * @param object EailBackground instance to be finalized
 */
static void
eail_background_finalize(GObject *object)
{
   EailBackground *bg = EAIL_BACKGROUND(object);

   if (bg->description) free(bg->description);

   G_OBJECT_CLASS(eail_background_parent_class)->finalize(object);
}

/**
 * @brief EailBackground instance initializer
 *
 * @param background EailBackground instance
 */
static void
eail_background_init(EailBackground *background)
{
   background->description = NULL;
}

/**
 * @brief Gets obj's attributes set
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @return AtkAttributeSet containing obj's attributes
 */
static AtkAttributeSet *
eail_background_get_attributes(AtkObject *obj)
{
   AtkAttribute *attr;
   AtkAttributeSet *attributes;
   Evas_Object *widget;
   int r, g, b;
   gchar color[8];
   const char *mode, *file;

   attributes = ATK_OBJECT_CLASS(eail_background_parent_class)->get_attributes(obj);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return attributes;

   elm_bg_file_get(widget, &file, NULL);
   if (!file)
     {
        elm_bg_color_get(widget, &r, &g, &b);
        g_snprintf(color, sizeof(color), "#%02x%02x%02x", r, g, b);
        attr = g_new(AtkAttribute, 1);
        attr->name = g_strdup("color");
        attr->value = g_strdup(color);
        attributes = g_slist_append(attributes, attr);
     }
   else
     {
        attr = g_new(AtkAttribute, 1);
        attr->name = g_strdup("file");
        attr->value = g_strdup(file);
        attributes = g_slist_append(attributes, attr);
     }

   switch (elm_bg_option_get(widget))
     {
       case ELM_BG_OPTION_CENTER:
           mode = "center";
           break;
       case ELM_BG_OPTION_SCALE:
           mode = "scale";
           break;
       case ELM_BG_OPTION_STRETCH:
           mode = "stretch";
           break;
       case ELM_BG_OPTION_TILE:
           mode = "tile";
           break;
       default:
           mode = "";
           break;
     }

   attr = g_new(AtkAttribute, 1);
   attr->name = g_strdup("display mode");
   attr->value = g_strdup(mode);
   attributes = g_slist_append(attributes, attr);

   return attributes;
}


/*
 * AtkImage interface implementation
 */

/**
 * @brief AtkImage interface initializer
 *
 * @param iface AtkImageIface instance
 */
static void
atk_image_interface_init(AtkImageIface *iface)
{
   g_return_if_fail(iface);

   iface->get_image_description = eail_background_get_image_description;
   iface->set_image_description = eail_background_set_image_description;
   iface->get_image_size = eail_background_get_image_size;
   iface->get_image_position = eail_background_get_image_position;
   iface->get_image_locale = eail_background_get_image_locale;
}

/**
 * @brief Gets background's image position.
 *
 * Position is in the form of a point specifying background top-left corner.
 *
 * @param image AtkImage instance
 * @param [out] x x coordinate or -1 if value cannot be obtained
 * @param [out] y y coordinate or -1 if value cannot be obtained
 * @param coord_type specifies whether the coordinates are relative to the screen or to the component's top level window
 */
static void
eail_background_get_image_position(AtkImage *image,
                                   gint *x,
                                   gint *y,
                                   AtkCoordType coord_type)
{
   atk_component_get_position(ATK_COMPONENT(image), x, y, coord_type);
}

/**
 * @brief Gets background image's description
 *
 * @param image AtkImage instance
 * @return string representing image's description
 */
static const gchar *
eail_background_get_image_description(AtkImage *image)
{
   EailBackground *bg;
   const gchar *desc;

   g_return_val_if_fail(EAIL_IS_BACKGROUND(image), NULL);

   bg = EAIL_BACKGROUND(image);
   if (!bg) return NULL;

   desc = bg->description;

   return desc;
}

/**
 * @brief Sets background image's description
 *
 * @param image AtkImage instance
 * @param description new image description
 * @return TRUE if description was set successfully, FALSE otherwise
 */
static gboolean
eail_background_set_image_description(AtkImage *image,
                                      const gchar *description)
{
   EailBackground *bg;

   g_return_val_if_fail(EAIL_IS_BACKGROUND(image), FALSE);

   bg = EAIL_BACKGROUND(image);
   if (!bg) return FALSE;

   if (bg->description)
     free(bg->description);

   bg->description = g_strdup(description);
   if (!bg->description) return FALSE;

   return TRUE;
}

/**
 * @brief Gets background's widget size
 *
 * @param image AtkImage instance
 * @param [out] width background's width or -1 if value cannot be obtained
 * @param [out] height background's height or -1 if value cannot be obtained
 */
static void
eail_background_get_image_size(AtkImage *image,
                               gint *width,
                               gint *height)
{
   EailBackground *bg;
   Evas_Object *widget;

   bg = EAIL_BACKGROUND(image);
   if (!bg)
     *width = *height = -1;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     return;

   evas_object_geometry_get(widget, NULL, NULL, width, height);
}

/**
 * @brief Gets background's locale (LC_MESSAGES variable)
 *
 * @param image AtkImage instance
 * @return string corresponding to the POSIX LC_MESSAGES
 * locale used by the image description, or NULL if
 * the image does not specify a locale
 *
 */
const gchar *
eail_background_get_image_locale(AtkImage *image)
{
    return setlocale(LC_MESSAGES, NULL);
}
