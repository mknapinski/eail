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
 * @file eail_label.h
 *
 * @brief Header for EailLabel implementation
 */

#ifndef EAIL_LABEL_H
#define EAIL_LABEL_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailLabel class
 */
#define EAIL_TYPE_LABEL              (eail_label_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailLabel
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_LABEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_LABEL, EailLabel))
/**
 * @brief Macro upcasts a subclass (klass) to the EailLabel class
 *
 * @param klass subclass object
 */
#define EAIL_LABEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_LABEL, EailLabelClass))
/**
 * @brief Tests whether object (obj) is an instance of EailLabel class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_LABEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_LABEL))
/**
 * @brief Tests whether given klass is a subclass of EailLabel
 *
 * @param klass klass object
 */
#define EAIL_IS_LABEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_LABEL))
/**
 * @brief Gets EailLabel class structure from an obj (class instance)
 *
 * @param obj object instance to get EailLabel class from
 */
#define EAIL_LABEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_LABEL, EailLabelClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailLabel*/
typedef struct _EailLabel      EailLabel;
/** @brief Definition of class structure for Atk EailLabel*/
typedef struct _EailLabelClass EailLabelClass;

/** @brief Definition of object structure for Atk EailLabel*/
struct _EailLabel
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailLabel*/
struct _EailLabelClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailLabel GType
 * @returns GType for EailLabel implementation*/
GType eail_label_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
