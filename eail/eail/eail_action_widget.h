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
 * @file eail_action_widget.h
 *
 * @brief Header for EailActionWidget implementation
 */

#ifndef EAIL_ACTION_WIDGET_H
#define EAIL_ACTION_WIDGET_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailActionWidget class
 */
#define EAIL_TYPE_ACTION_WIDGET             (eail_action_widget_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailActionWidget
 * type
 *
 * @param obj AtkObject instance
 */

#define EAIL_ACTION_WIDGET(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidget))
/**
 * @brief Macro upcasts a subclass (klass) to the EailActionWidget class
 *
 * @param klass subclass object
 */

#define EAIL_ACTION_WIDGET_CLASS(klass)     (G_TYPE_ACTION_WIDGET_CLASS_CAST((klass), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidgetClass))
/**
 * @brief Tests whether object (obj) is an instance of EailActionWidget class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_ACTION_WIDGET(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                             EAIL_TYPE_ACTION_WIDGET))
/**
 * @brief Tests whether given klass is a subclass of EailActionWidget
 *
 * @param klass klass object
 */

#define EAIL_IS_ACTION_WIDGET_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                             EAIL_TYPE_ACTION_WIDGET))
/**
 * @brief Gets EailActionWidget class structure from an obj (class instance)
 *
 * @param obj object instance to get EailActionWidget class from
 */
#define EAIL_ACTION_WIDGET_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidgetClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailActionWidget*/
typedef struct _EailActionWidget      EailActionWidget;
/** @brief Definition of class structure for Atk EailActionWidget*/
typedef struct _EailActionWidgetClass EailActionWidgetClass;

/*! \brief Callback used for performing work-action by given ActionObject
 */
typedef gboolean (* Eail_Do_Action_Cb) (AtkAction *obj, void *data);

/*! \brief Appends given action to action list used by EailActionWidget
 */
void eail_action_widget_action_append(EailActionWidget *action_widget,
                                      const gchar *action_name,
                                      const gchar *keybinding,
                                      Eail_Do_Action_Cb action_cb);

/** @brief Definition of object structure for Atk EailActionWidget
 * */
struct _EailActionWidget
{
   /** @brief Parent widget whose functionality is being extended */
   EailWidget parent;
   /** @brief List of action objects (EailActionObj*) */
   Eina_List *action_objs;
};

/** @brief Definition of class structure for Atk EailActionWidget
 * */
struct _EailActionWidgetClass
{
   EailWidgetClass parent_class;/**< class whose functionality is being extended*/
};

/** @brief Getter for EailActionWidget GType
 * @returns GType for EailActionWidget implementation
 * */
GType eail_action_widget_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
