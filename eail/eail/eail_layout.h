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
 * @file eail_layout.h
 *
 * @brief Header for EailLayout implementation
 */

#ifndef EAIL_LAYOUT_H
#define EAIL_LAYOUT_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailLayout class
 */
#define EAIL_TYPE_LAYOUT              (eail_layout_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailLayout
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_LAYOUT, EailLayout))
/**
 * @brief Macro upcasts a subclass (klass) to the EailLayout class
 *
 * @param klass subclass object
 */
#define EAIL_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_LAYOUT, EailLayoutClass))
/**
 * @brief Tests whether object (obj) is an instance of EailLayout class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_LAYOUT))
/**
 * @brief Tests whether given klass is a subclass of EailLayout
 *
 * @param klass klass object
 */
#define EAIL_IS_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_LAYOUT))
/**
 * @brief Gets EailLayout class structure from an obj (class instance)
 *
 * @param obj object instance to get EailLayout class from
 */
#define EAIL_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_LAYOUT, EailLayoutClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailLayout*/
typedef struct _EailLayout      EailLayout;
/** @brief Definition of object class for Atk EailLayout*/
typedef struct _EailLayoutClass EailLayoutClass;

/** @brief Definition of object structure for Atk EailLayout*/
struct _EailLayout
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
   gint child_count_last;/**< @brief last reported count of children */
};

/** @brief Definition of object class for Atk EailLayout*/
struct _EailLayoutClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailLayout GType
 * @returns GType for EailLayout implementation*/
GType eail_layout_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
