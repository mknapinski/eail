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
 * @file eail_button.h
 *
 * @brief Header for EailButton implementation
 */

#ifndef EAIL_BUTTON_H
#define EAIL_BUTTON_H

#include "eail_action_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailButton class
 */
#define EAIL_TYPE_BUTTON             (eail_button_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailButton
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_BUTTON, EailButton))

/**
 * @brief Macro upcasts a subclass (klass) to the EailButton class
 *
 * @param klass subclass object
 */
#define EAIL_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_BUTTON, EailButtonClass))

/**
 * @brief Tests whether object (obj) is an instance of EailButton class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_BUTTON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_BUTTON))

/**
 * @brief Tests whether given klass is a subclass of EailButton
 *
 * @param klass klass object
 */
#define EAIL_IS_BUTTON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_BUTTON))
/**
 * @brief Gets EailButton class structure from an obj (class instance)
 *
 * @param obj object instance to get EailButton class from
 */
#define EAIL_BUTTON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_BUTTON, EailButtonClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailButton*/
typedef struct _EailButton      EailButton;

/** @brief Definition of class structure for Atk EailButton*/
typedef struct _EailButtonClass EailButtonClass;

/** @brief Definition of object structure for Atk EailButton*/
struct _EailButton
{
   /** @brief parent AtkObject that functionality is being extended */
   EailActionWidget parent;
};

/** @brief Definition of class structure for Atk EailButton*/
struct _EailButtonClass
{
   /** @brief parent class whose functionality is being extended */
   EailActionWidgetClass parent_class;
};

/**
 * @brief Getter for EailButton GType
 * @return GType for EailButton implementation
 */
GType eail_button_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
