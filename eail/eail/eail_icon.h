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
 * @file eail_icon.h
 *
 * @brief Header for EailIcon implementation
 */

#ifndef EAIL_ICON_H
#define EAIL_ICON_H

#include "eail_image.h"
/**
 * @brief Returns a value corresponding to the type of EailIcon class
 */
#define EAIL_TYPE_ICON             (eail_icon_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailIcon
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_ICON(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_ICON, EailIcon))
/**
 * @brief Macro upcasts a subclass (klass) to the EailIcon class
 *
 * @param klass subclass object
 */
#define EAIL_ICON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_ICON, EailIconClass))
/**
 * @brief Tests whether object (obj) is an instance of EailIcon class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_ICON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_ICON))
/**
 * @brief Tests whether given klass is a subclass of EailIcon
 *
 * @param klass klass object
 */
#define EAIL_IS_ICON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_ICON))
/**
 * @brief Gets EailIcon class structure from an obj (class instance)
 *
 * @brief Header for EailIcon implementation
 */
#define EAIL_ICON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_ICON, EailIconClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailIcon*/
typedef struct _EailIcon      EailIcon;
/** @brief Definition of class structure for Atk EailIcon*/
typedef struct _EailIconClass EailIconClass;

/** @brief Definition of object structure for Atk EailIcon*/
struct _EailIcon
{
   /** @brief Parent widget whose functionality is being extended*/
   EailImage parent;
};

/** @brief Definition of class structure for Atk EailIcon*/
struct _EailIconClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailImageClass parent_class;
};

/** @brief Getter for EailIcon GType
 * @returns GType for EailIcon implementation*/
GType eail_icon_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
