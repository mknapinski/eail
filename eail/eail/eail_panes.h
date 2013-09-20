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
 * @file eail_panes.h
 *
 * @brief Header for EailPanes implementation
 */

#ifndef EAIL_PANES_H
#define EAIL_PANES_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailPanes class
 */
#define EAIL_TYPE_PANES             (eail_panes_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPanes
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PANES(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_PANES, EailPanes))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPanes class
 *
 * @param klass subclass object
 */
#define EAIL_PANES_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_PANES, EailPanesClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPanes class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PANES(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_PANES))
/**
 * @brief Tests whether given klass is a subclass of EailPanes
 *
 * @param klass klass object
 */
#define EAIL_IS_PANES_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_PANES))
/**
 * @brief Gets EailPanes class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPanes class from
 */
#define EAIL_PANES_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_PANES, EailPanesClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPanes*/
typedef struct _EailPanes      EailPanes;
/** @brief Definition of object class for Atk EailPanes*/
typedef struct _EailPanesClass EailPanesClass;

/** @brief Definition of object structure for Atk EailPanes*/
struct _EailPanes
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailPanes*/
struct _EailPanesClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Parent object whose functionality is being extended
 *
 * @returns GType for EailPanes implementation*/
GType eail_panes_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
