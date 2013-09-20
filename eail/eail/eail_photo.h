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
 * @file eail_photo.h
 *
 * @brief Header for EailPhoto implementation
 */

#ifndef EAIL_PHOTO_H
#define EAIL_PHOTO_H

#include "eail_image.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Returns a value corresponding to the type of EailPhoto class
 */
#define EAIL_TYPE_PHOTO             (eail_photo_get_type ())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPhoto
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PHOTO(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                    EAIL_TYPE_PHOTO, EailPhoto))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPhoto class
 *
 * @param klass subclass object
 */
#define EAIL_PHOTO_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    EAIL_TYPE_PHOTO, EailPhotoClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPhoto class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PHOTO(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                    EAIL_TYPE_PHOTO))
/**
 * @brief Tests whether given klass is a subclass of EailPhoto
 *
 * @param klass klass object
 */
#define EAIL_IS_PHOTO_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    EAIL_TYPE_PHOTO))
/**
 * @brief Gets EailPhoto class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPhoto class from
 */
#define EAIL_PHOTO_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    EAIL_TYPE_PHOTO, EailPhotoClass))

/** @brief Definition of object structure for Atk EailPhoto*/
typedef struct _EailPhoto       EailPhoto;
/** @brief Definition of object class for Atk EailPhoto*/
typedef struct _EailPhotoClass  EailPhotoClass;

/** @brief Definition of object structure for Atk EailPhoto*/
struct _EailPhoto
{
   /** @brief Parent object that functionality is being extended*/
    EailImage parent;
};

/** @brief Definition of object class for Atk EailPhoto*/
struct _EailPhotoClass
{
   /** @brief Parent class whose functionality is being extended*/
    EailImageClass parent_class;
};

/**
 * @brief Getter for EailPhoto GType
 *
 * @returns GType for EailPhoto implementation
 */
GType eail_photo_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
