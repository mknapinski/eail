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
 * @file eail.h
 *
 * @brief Header for Eail initialization part
 */

/**
 * @mainpage Manual SDK
 *
 * @section Description
 * EAIL (Enlightenment Accessibility Implementation Library) is implementation
 * of ATK (Accessibility Toolkit) - it allows to receive accessible content
 * representation for Elementary widgets.
 * EAIL is meant to be a bridge Elementary and ATK, thus various ATs can query for
 * accessible information using AT-SPI2.
 * ATK is an accessibility toolkit conforming to the Section 508 Standards.
 * EAIL is developed as an Elementary module and loaded whenever Elementary
 * ELM_MODULES environmental variable is set  ("eail>eail/api" is appended).
 *
 * @section Setup
 * <p>In order to load properly, EAIL has to be installed in directory that is
 * used to store the rest of Elementary modules (directory path is architecture
 * dependent). Passing correct directory name is available via ./configure
 * option '--with-elementary-module'.</p>
 * <p>Below is sample configuration for x86_64
 * architecture:
 * @code
 * ./configure --with-elementary-modules=/usr/local/lib/elementary/modules/eail/linux-gnu-x86_64-1.7.99/
 * @endcode
 * </p>
 * <p>When EAIL module is installed, then Elementary library has to be notified
 * about new module - it needs to load EAIL module during startup.
 * This can be done using 'ELM_MODULES' environmental variable.</p>
 * <p>Below is a code snippet, that shows how to set EAIL module load for current
 * session:
 * @code
 * export ELM_MODULES="eail>eail/api"
 * @endcode
 *
 * NOTE: ELM_MODULES can also be set for every session using elementary configuration
 * files. Details on how to do that can be found in Elementary documentation.
 *
 * <p>This is all needed configuration - now elementary application can be
 * launched and accessible content for target application should be visible
 * when using accessibility clients (eg. ORCA, TADEK, accerciser...)</p>
 * </p>
 *
 * @section Architecture
 * <p>EAIL is implemented as module of Enlightenment (it is located in Elementary
 * modules directory). When elementary-application is being
 * launched and ELM_MODULES is set correctly, then EAIL module is also loaded.
 * EAIL module registers itself to ATK bridge, so accessibility client
 * application (eg. ORCA, TADEK) can obtain information (in form of accessible
 * ATK object) about every visible element in application UI.</p>
 * <p>
 * See diagram of architecture below:
 * @image html eail_component_diagram.png
 * </p>
 *
 * @section Dataflow Data Flow
 * <p>When accessibility client wants to get accessible content of application
 * it needs to get 'root' ATK object for application. To do that, client has to
 * call atk_get_root() on ATK/AT-SPI brigde. Request is propagated to EAIL which
 * creates new instance of EailApp which is ATK representation of elementary
 * application.</p>
 * <p>
 * Below is diagram that shows what is happening during that operation:
 * @image html eail_get_root_flow.png
 * </p>
 *
 * <p>When accessibility client holds reference to ATK root object (EailApp), it
 * can use AtkObject interface to use objects accessible functionality (eg.
 * get name, set/get description or, the most important one, get reference to
 * children)). To get widget children, client has to call
 * <b>atk_object_ref_accessible_child.</b>
 * </p>
 * <p>
 * Below is diagram that shows how app child (EailWindow) is being created. For
 * other kinds of object data flow is very similar:
 * @image html eail_ref_window_child_flow.png
 * </p>
 *
 * @section Events
 * Following ATK event types are supported:
 * -# "focus"
 * -# "state-change"
 * -# "visible-data-changed"
 * -# "property-change"
 * <p>
 * NOTE: To receive events from EAIL widget, ATK-client application needs to
 * hold reference to target widget (it can be achieved by eg. listing whole
 * widget hierarchy tree). Without that event listener for widget won't be
 * registered and ATK events won't be propagated.
 * \n
 * To be sure that client always
 * has correct widget tree, it needs to refresh widget descendants every time
 * it receives "visible-data-changed" event from ATK.
 * </p>
 * Example how to register for events using python pyatspi binding (
 * as mentioned above all objects that we're interesed in have to be listed at
 * first, below is a code spippet only for event registration for pyatspi part):
 * @code
 * pyatspi.Registry.registerEventListener(test_cb, "focus")
 * pyatspi.Registry.registerEventListener(test_cb, "object:state-changed")
 * pyatspi.Registry.registerEventListener(test_cb, "object:visible-data-changed")
 * pyatspi.Registry.registerEventListener(test_cb, "object:property-changed")
 *
 * pyatspi.Registry.start()
 *
 * @endcode
 *
 * @section Examples
 * <p>Below are some examples on how to use C API for ATK with EAIL.
 * </p>
 * <p>
 * How to get root ATK object for current application:
 * @code
 * AtkObject *atk_obj = atk_get_root();
 * @endcode
 * </p>
 *
 * <p>
 * How to count children of given ATK object:
 * @code
 * gint n = atk_object_get_n_accessible_children(atk_obj);
 * @endcode
 * </p>
 *
 * <p>
 * How to obtain reference to first child of ATK object:
 * @code
 * AtkObject *atk_obj = atk_object_ref_accessible_child(atk_obj, 0);
 * @endcode
 * </p>
 *
 * <p>
 * How to launch first action of given ATK object that supports ATK_ACTION
 * interface:
 * @code
 * gboolean success = atk_action_do_action(ATK_ACTION(atk_obj), 0);
 * @endcode
 * </p>
 *
 * <p>
 * How to get name of action with given index ('1'):
 * @code
 * const gchar *action_name = atk_action_get_name(ATK_ACTION(atk_obj), 1);
 * @endcode
 * </p>
 *
 * Many more ready-to-go examples can be found in EAIL test code which can
 * be found in <b>tests/</b> directory.
 */
#ifndef EAIL_H
#define EAIL_H

#include "Elementary.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief ATK initializer for accessibility API */
EAPI int elm_modapi_init(void *m);

/** @brief ATK desctructor for accessibility API */
EAPI int elm_modapi_shutdown(void *m);

#ifdef __cplusplus
}
#endif

#endif
