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
 * @file eail_spinner.h
 *
 * @brief Header for EailSpinner implementation
 */

#ifndef EAIL_SPINNER_H
#define EAIL_SPINNER_H

#include "eail_widget.h"


/**
 * @brief Returns a value corresponding to the type of EailSpinner class
 */
#define EAIL_TYPE_SPINNER             (eail_spinner_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailSpinner
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SPINNER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_SPINNER, EailSpinner))

/**
 * @brief Macro upcasts a subclass (klass) to the EailSpinner class
 *
 * @param klass subclass object
 */
#define EAIL_SPINNER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_SPINNER, EailSpinnerClass))

/**
 * @brief Tests whether object (obj) is an instance of EailSpinner class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SPINNER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_SPINNER))

/**
 * @brief Tests whether given klass is a subclass of EailSpinner
 *
 * @param klass klass object
 */
#define EAIL_IS_SPINNER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_SPINNER))


/**
 * @brief Gets EailSpinner class structure from an obj (class instance)
 *
 * @param obj object instance to get EailSpinner class from
 */
#define EAIL_SPINNER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_SPINNER, EailSpinnerClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailSpinner*/
typedef struct _EailSpinner      EailSpinner;
/** @brief Definition of object class for Atk EailSpinner*/
typedef struct _EailSpinnerClass EailSpinnerClass;

/** @brief Definition of object structure for Atk EailSpinner*/
struct _EailSpinner
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailSpinner*/
struct _EailSpinnerClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailSpinner GType
 *
 * @returns GType for EailSpinner implementation*/
GType eail_spinner_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
