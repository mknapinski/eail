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
 * @file eail_photocam.h
 *
 * @brief Header for EailPhotocam implementation
 */

#ifndef EAIL_PHOTOCAM_H
#define EAIL_PHOTOCAM_H

#include "eail_scrollable_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailPhotocam class
 */
#define EAIL_TYPE_PHOTOCAM              (eail_photocam_get_type ())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPhotocam
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PHOTOCAM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                         EAIL_TYPE_PHOTOCAM, EailPhotocam))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPhotocam class
 *
 * @param klass subclass object
 */
#define EAIL_PHOTOCAM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                         EAIL_TYPE_PHOTOCAM, EailPhotocamClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPhotocam class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PHOTOCAM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                         EAIL_TYPE_PHOTOCAM))
/**
 * @brief Tests whether given klass is a subclass of EailPhotocam
 *
 * @param klass klass object
 */
#define EAIL_IS_PHOTOCAM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                         EAIL_TYPE_PHOTOCAM))
/**
 * @brief Gets EailPhotocam class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPhotocam class from
 */
#define EAIL_PHOTOCAM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                         EAIL_TYPE_PHOTOCAM, EailPhotocamClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPhotocam*/
typedef struct _EailPhotocam       EailPhotocam;
/** @brief Definition of object class for Atk EailPhotocam*/
typedef struct _EailPhotocamClass  EailPhotocamClass;

/** @brief Definition of object structure for Atk EailPhotocam*/
struct _EailPhotocam
{
   EailScrollableWidget parent;/**< @brief widget that is being extended*/
   char *description;/**< @brief 'description' string property*/
};

/** @brief Definition of object class for Atk EailPhotocam*/
struct _EailPhotocamClass
{
   EailScrollableWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailPhotocam GType
 *
 * @returns GType for EailPhotocam object
 */
GType eail_photocam_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
