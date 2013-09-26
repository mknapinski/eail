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

#ifndef EAIL_MENU_H
#define EAIL_MENU_H

/**
 * @file eail_menu.h
 *
 * @brief Header for EailMenu implementation
 */

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailMenu class
 */
#define EAIL_TYPE_MENU             (eail_menu_get_type ())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailMenu
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_MENU(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                    EAIL_TYPE_MENU, EailMenu))
/**
 * @brief Macro upcasts a subclass (klass) to the EailMenu class
 *
 * @param klass subclass object
 */
#define EAIL_MENU_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    EAIL_TYPE_MENU, EailMenuClass))
/**
 * @brief Tests whether object (obj) is an instance of EailMenu class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_MENU(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                    EAIL_TYPE_MENU))
/**
 * @brief Tests whether given klass is a subclass of EailMenu
 *
 * @param klass klass object
 */
#define EAIL_IS_MENU_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    EAIL_TYPE_MENU))
/**
 * @brief Gets EailMenu class structure from an obj (class instance)
 *
 * @param obj object instance to get EailMenu class from
 */
#define EAIL_MENU_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                    EAIL_TYPE_MENU, EailMenuClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailMenu*/
typedef struct _EailMenu      EailMenu;
/** @brief Definition of object class for Atk EailMenu*/
typedef struct _EailMenuClass EailMenuClass;

/** @brief Definition of object structure for Atk EailMenu*/
struct _EailMenu
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailMenu*/
struct _EailMenuClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailMenu GType
 * @returns GType for EailMenu implementation*/
GType eail_menu_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
