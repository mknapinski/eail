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
 * @file eail_box.h
 *
 * @brief Header for EailBox implementation
 */

#ifndef EAIL_BOX_H
#define EAIL_BOX_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailBox class
 */
#define EAIL_TYPE_BOX              (eail_box_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailBox
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_BOX(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_BOX, EailBox))
/**
 * @brief Macro upcasts a subclass (klass) to the EailBox class
 *
 * @param klass subclass object
 */
#define EAIL_BOX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_BOX, EailBoxClass))
/**
 * @brief Tests whether object (obj) is an instance of EailBox class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_BOX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_BOX))
/**
 * @brief Tests whether given klass is a subclass of EailBox
 *
 * @param klass klass object
 */
#define EAIL_IS_BOX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_BOX))
/**
 * @brief Gets EailBox class structure from an obj (class instance)
 *
 * @param obj object instance to get EailBox class from
 */
#define EAIL_BOX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_BOX, EailBoxClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailBox*/
typedef struct _EailBox      EailBox;
/** @brief Definition of class structure for Atk EailBox*/
typedef struct _EailBoxClass EailBoxClass;

/** @brief Definition of object structure for Atk EailBox*/
struct _EailBox
{
   /** @brief Parent widget whose functionality is being extended */
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailBox*/
struct _EailBoxClass
{
   /** @brief parent class whose functionality is being extended */
   EailWidgetClass parent_class;
};

/** @brief Getter for EailBox GType
 *
 * @returns GType for EailBox implementation
 * */
GType eail_box_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
