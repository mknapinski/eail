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
 * @file eail_slideshow.h
 *
 * @brief Header for EailSlideshow implementation
 */

#ifndef EAIL_SLIDESHOW_H
#define EAIL_SLIDESHOW_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailSlideshow class
 */
#define EAIL_TYPE_SLIDESHOW             (eail_slideshow_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailSlideshow
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SLIDESHOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_SLIDESHOW, EailSlideshow))

/**
 * @brief Macro upcasts a subclass (klass) to the EailSlideshow class
 *
 * @param klass subclass object
 */
#define EAIL_SLIDESHOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                         EAIL_TYPE_SLIDESHOW, \
                                         EailSlideshowClass))

/**
 * @brief Tests whether object (obj) is an instance of EailSlideshow class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SLIDESHOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_SLIDESHOW))

/**
 * @brief Tests whether given klass is a subclass of EailSlideshow
 *
 * @param klass klass object
 */
#define EAIL_IS_SLIDESHOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                         EAIL_TYPE_SLIDESHOW))

/**
 * @brief Gets EailSlideshow class structure from an obj (class instance)
 *
 * @param obj object instance to get EailSlideshow class from
 */
#define EAIL_SLIDESHOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                         EAIL_TYPE_SLIDESHOW, \
                                         EailSlideshowClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailSlideshow*/
typedef struct _EailSlideshow EailSlideshow;
/** @brief Definition of object class for Atk EailSlideshow*/
typedef struct _EailSlideshowClass EailSlideshowClass;

/** @brief Definition of object structure for Atk EailSlideshow*/
struct _EailSlideshow
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
   gchar *next_description;/*!< @brief "next" action description*/
   gchar *previous_description;/*!< @brief "previous" action description*/
   gchar *stop_description;/*!< @brief "stop" action description*/
   gchar *start_description;/*!< @brief "start" action description*/
};

/** @brief Definition of object class for Atk EailSlideshow*/
struct _EailSlideshowClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailSlideshow GType
 *
 * @returns GType for EailSlideshow implementation*/
GType eail_slideshow_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
