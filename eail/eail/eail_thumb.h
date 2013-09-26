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
 * @file eail_thumb.h
 *
 * @brief Header for EailThumb implementation
 */

#ifndef EAIL_THUMB_H
#define EAIL_THUMB_H

#include "eail_image.h"


/**
 * @brief Returns a value corresponding to the type of EailThumb class
 */
#define EAIL_TYPE_THUMB             (eail_thumb_get_type ())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailThumb
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_THUMB(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     EAIL_TYPE_THUMB, EailThumb))

/**
 * @brief Macro upcasts a subclass (klass) to the EailThumb class
 *
 * @param klass subclass object
 */
#define EAIL_THUMB_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                     EAIL_TYPE_THUMB, EailThumbClass))

/**
 * @brief Tests whether object (obj) is an instance of EailThumb class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_THUMB(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     EAIL_TYPE_THUMB))

/**
 * @brief Tests whether given klass is a subclass of EailThumb
 *
 * @param klass klass object
 */
#define EAIL_IS_THUMB_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                     EAIL_TYPE_THUMB))

/**
 * @brief Gets EailThumb class structure from an obj (class instance)
 *
 * @param obj object instance to get EailThumb class from
 */
#define EAIL_THUMB_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                     EAIL_TYPE_THUMB, EailThumbClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailThumb*/
typedef struct _EailThumb      EailThumb;
/** @brief Definition of object class for Atk EailThumb*/
typedef struct _EailThumbClass EailThumbClass;

/** @brief Definition of object structure for Atk EailThumb*/
struct _EailThumb
{
   /** @brief Parent widget whose functionality is being extended*/
   EailImage parent;
};

/** @brief Definition of object class for Atk EailThumb*/
struct _EailThumbClass
{
   EailImageClass parent_class;/*!< @brief class whose functionality is being extended*/
};

/** @brief Getter for EailThumb GType
 *
 * @returns GType for EailThumb implementation*/
GType eail_thumb_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
