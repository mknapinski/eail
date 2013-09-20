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
 * @file eail_toolbar.h
 *
 * @brief Header for EailToolbar implementation
 */

#ifndef EAIL_TOOLBAR_H
#define EAIL_TOOLBAR_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailToolbar class
 */
#define EAIL_TYPE_TOOLBAR             (eail_toolbar_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailToolbar
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_TOOLBAR(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbar))

/**
 * @brief Macro upcasts a subclass (klass) to the EailToolbar class
 *
 * @param klass subclass object
 */
#define EAIL_TOOLBAR_CLASS(klass)     (G_TYPE_TOOLBAR_CLASS_CAST((klass), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbarClass))

/**
 * @brief Tests whether object (obj) is an instance of EailToolbar class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_TOOLBAR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                            EAIL_TYPE_TOOLBAR))

/**
 * @brief Tests whether given klass is a subclass of EailToolbar
 *
 * @param klass klass object
 */
#define EAIL_IS_TOOLBAR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                            EAIL_TYPE_TOOLBAR))

/**
 * @brief Gets EailToolbar class structure from an obj (class instance)
 *
 * @param obj object instance to get EailToolbar class from
 */
#define EAIL_TOOLBAR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbarClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailToolbar*/
typedef struct _EailToolbar EailToolbar;

/** @brief Definition of object class for Atk EailToolbar*/
typedef struct _EailToolbarClass EailToolbarClass;

/** @brief Definition of object structure for Atk EailToolbar*/
struct _EailToolbar
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailToolbar*/
struct _EailToolbarClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailToolbar GType
 *
 * @returns GType for EailToolbar implementation*/
GType eail_toolbar_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

