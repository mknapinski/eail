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
 * @file eail_hoversel.h
 *
 * @brief Header for EailHoversel implementation
 */

#ifndef EAIL_HOVERSEL_H
#define EAIL_HOVERSEL_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailHoversel class
 */
#define EAIL_TYPE_HOVERSEL            (eail_hoversel_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailHoversel
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_HOVERSEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_HOVERSEL, EailHoversel))
/**
 * @brief Macro upcasts a subclass (klass) to the EailHoversel class
 *
 * @param klass subclass object
 */
#define EAIL_HOVERSEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_HOVERSEL, EailHoverselClass))
/**
 * @brief Tests whether object (obj) is an instance of EailHoversel class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_HOVERSEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_HOVERSEL))
/**
 * @brief Tests whether given klass is a subclass of EailHoversel
 *
 * @param klass klass object
 */
#define EAIL_IS_HOVERSEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_HOVERSEL))
/**
 * @brief Gets EailHoversel class structure from an obj (class instance)
 *
 * @param obj object instance to get EailHoversel class from
 */
#define EAIL_HOVERSEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_HOVERSEL, EailHoverselClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailHoversel*/
typedef struct _EailHoversel      EailHoversel;
/** @brief Definition of class structure for Atk EailHoversel*/
typedef struct _EailHoverselClass EailHoverselClass;

/** @brief Definition of object structure for Atk EailHoversel*/
struct _EailHoversel
{
   EailWidget parent;/**< @brief widget that functionality is being extended*/

   char *shrink_description;/**< @brief 'shrink' action description string */
   char *expand_description;/**< @brief 'expand' action description string */
};

/** @brief Definition of class structure for Atk EailHoversel*/
struct _EailHoverselClass
{
   EailWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailHoversel GType
 * @returns GType for EailHoversel implementation
 */
GType eail_hoversel_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
