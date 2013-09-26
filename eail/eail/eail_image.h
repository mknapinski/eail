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
 * @file eail_image.h
 *
 * @brief Header for EailImage implementation
 */

#ifndef EAIL_IMAGE_H
#define EAIL_IMAGE_H

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailImage class
 */
#define EAIL_TYPE_IMAGE             (eail_image_get_type ())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailImage
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_IMAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     EAIL_TYPE_IMAGE, EailImage))
/**
 * @brief Macro upcasts a subclass (klass) to the EailImage class
 *
 * @param klass subclass object
 */
#define EAIL_IMAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                     EAIL_TYPE_IMAGE, EailImageClass))
/**
 * @brief Tests whether object (obj) is an instance of EailImage class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_IMAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     EAIL_TYPE_IMAGE))
/**
 * @brief Tests whether given klass is a subclass of EailImage
 *
 * @param klass klass object
 */
#define EAIL_IS_IMAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                     EAIL_TYPE_IMAGE))
/**
 * @brief Gets EailImage class structure from an obj (class instance)
 *
 * @param obj object instance to get EailImage class from
 */
#define EAIL_IMAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                     EAIL_TYPE_IMAGE, EailImageClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailImage*/
typedef struct _EailImage       EailImage;
/** @brief Definition of class structure for Atk EailImage*/
typedef struct _EailImageClass  EailImageClass;

/** @brief Definition of object structure for Atk EailImage*/
struct _EailImage
{
   /** @brief Parent widget whose functionality is being extended*/
   EailActionWidget parent;

   char *description; /*!< @brief Image description*/
};

/** @brief Definition of class structure for Atk EailImage*/
struct _EailImageClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailActionWidgetClass parent_class;
};

/** @brief Getter for EailImage GType
 * @returns GType for EailImage implementation*/
GType eail_image_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
