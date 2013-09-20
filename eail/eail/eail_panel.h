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

#ifndef EAIL_PANEL_H
#define EAIL_PANEL_H

/**
 * @file eail_panel.h
 *
 * @brief Header for EailPanel implementation
 */

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailPanel class
 */
#define EAIL_TYPE_PANEL             (eail_panel_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPanel
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PANEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_PANEL, EailPanel))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPanel class
 *
 * @param klass subclass object
 */
#define EAIL_PANEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_PANEL, EailPanelClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPanel class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PANEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_PANEL))
/**
 * @brief Tests whether given klass is a subclass of EailPanel
 *
 * @param klass klass object
 */
#define EAIL_IS_PANEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_PANEL))
/**
 * @brief Gets EailPanel class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPanel class from
 */
#define EAIL_PANEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_PANEL, EailPanelClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPanel*/
typedef struct _EailPanel EailPanel;
/** @brief Definition of object class for Atk EailPanel*/
typedef struct _EailPanelClass EailPanelClass;

/** @brief Definition of object structure for Atk EailPanel*/
struct _EailPanel
{
   EailActionWidget parent;/**< @brief widget that is being extended*/
   char *toggle_description;/**< @brief description for 'toggle' action */
};

/** @brief Definition of object class for Atk EailPanel*/
struct _EailPanelClass
{
   EailActionWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for panel GType
 *
 * @returns GType for EailPanel implementation
 */
GType eail_panel_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
