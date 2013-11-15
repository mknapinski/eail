/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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
 * @file eail-explorer-ui.c
 *
 * @brief EAIL Explorer UI implementation
 */

#define _GNU_SOURCE
#include "string.h"
#include "eail-explorer-ui.h"
#include "eail-explorer-tr.h"
#include "eail-browser.h"
#include <stdio.h>

/** @brief action_cb_struct Struct for action callbacks */
action_cb_struct *acs = NULL;

/**
 * @brief cleanup
 *
 * Cleans memory by free action cb struct
 */
void free_memory()
{
	if(acs)
	{
		eina_list_free(acs->list);
		free(acs);
		acs = NULL;
	}
}

/**
 * @brief Refreshes explorer layout
 *
 * Use elm_toolbar_item_selected_set() twice to refresh layout
 *
 * @param toolbar Toolbar object
 *
 * @return
 */
void refresh_layout(Evas_Object *toolbar)
{
	Elm_Object_Item *tb_item = elm_toolbar_first_item_get(toolbar);
	if(tb_item)
	{
		elm_toolbar_item_selected_set(tb_item, EINA_FALSE);
		elm_toolbar_item_selected_set(tb_item, EINA_TRUE);
	}
	else
	{
		tb_item = elm_toolbar_first_item_get(toolbar);
		elm_toolbar_item_selected_set(tb_item, EINA_TRUE);
	}
}

/**
 * @brief checks whether an object is part of EAIL-Explorer
 *
 * Check is performed by getting the role and name from AtkObject
 *
 * @param obj AtkObject which is checked
 *
 * @return true when the object is part of EAIL-Explorer, false otherwise
 */
Eina_Bool _is_part_of_eail_explorer(AtkObject *obj)
{
	if(!obj || ATK_ROLE_APPLICATION == atk_object_get_role(obj))
		return EINA_FALSE;

	AtkObject *atk_window = obj;
	while(ATK_ROLE_WINDOW != atk_object_get_role(atk_window))
		atk_window = atk_object_get_parent(atk_window);

	const char *name = atk_object_get_name(atk_window);
	if(name && !strcmp(name, EAIL_EXPLORER))
		return EINA_TRUE;

	return EINA_FALSE;
}

void _print_desc_on_entry(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *entry = eina_list_nth(acs->list, 1);
	Elm_Object_Item *it = (Elm_Object_Item*)event_info;
	int i = 0;
	const Eina_List *list = elm_list_items_get(eina_list_nth(acs->list, 0));
	const Eina_List *l;
	void *list_data;
	EINA_LIST_FOREACH(list, l, list_data)
	{
		if(list_data == it)
		{
			const char *desc = atk_action_get_description(ATK_ACTION(acs->atk_obj), i);
			elm_entry_entry_set(entry, desc);
			return;
		}
		++i;
	}
}

void _fill_action_list(void *data)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *list = eina_list_nth(acs->list, 0);
	AtkObject *obj = acs->atk_obj;
	int counter = atk_action_get_n_actions(ATK_ACTION(obj));
	elm_list_clear(list);
	int i;
	for(i = 0; i < counter; ++i)
	{
		const char *desc = atk_action_get_description(ATK_ACTION(obj), i);
		if(!desc)
		{
			elm_list_item_append(list, atk_action_get_name(ATK_ACTION(obj), i), NULL, NULL, _print_desc_on_entry, acs);
		}
		else
		{
			char *action_name_with_desc = NULL;
			if(asprintf(&action_name_with_desc, "%s [%s]", atk_action_get_name(ATK_ACTION(obj), i), desc) < 0)
			{
				EINA_LOG_ERR(ERR_MEMORY);
				elm_list_item_append(list, atk_action_get_name(ATK_ACTION(obj), i), NULL, NULL, _print_desc_on_entry, acs);
			}
			else
			{
				elm_list_item_append(list, action_name_with_desc, NULL, NULL, _print_desc_on_entry, acs);
				free(action_name_with_desc);
			}
		}
	}
	elm_list_go(list);
}

/**
 * @brief Callback function for Button click in Action Layout
 *
 * The callback is called when the Action label for button is clicked.
 * The callback performs the action selected in the given action list.
 *
 * @param data Data passed to callback, action_cb_struct
 * @param obj UNUSED
 * @param event_info UNUSED
 *
 * @return
 */
void _action_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo = eina_list_nth(acs->list, 0);
	Elm_Object_Item *it = elm_list_selected_item_get(eo);
	if(!it)
	{
		return;
	}
	int i = 0;
	const Eina_List *list = elm_list_items_get(eo);
	const Eina_List *l;
	void *list_data;
	EINA_LIST_FOREACH(list, l, list_data)
	{
		if(list_data == it)
		{
			atk_action_do_action(ATK_ACTION(acs->atk_obj), i);
			expand_browser(acs->atk_obj);
			return;
		}
		++i;
	}
}

void _set_action_desc(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo = eina_list_nth(acs->list, 0);
	Elm_Object_Item *it = elm_list_selected_item_get(eo);
	if(!it)
	{
		return;
	}
	Evas_Object *entry = eina_list_nth(acs->list, 1);
	const char *desc = elm_entry_entry_get(entry);
	if(!desc || !strcmp(desc, ""))
	{
		return;
	}
	const Eina_List *list = elm_list_items_get(eina_list_nth(acs->list, 0));
	const Eina_List *l;
	void *list_data;
	int i = 0;
	EINA_LIST_FOREACH(list, l, list_data)
	{
		if(list_data == it)
		{
			atk_action_set_description(ATK_ACTION(acs->atk_obj), i, desc);
			_fill_action_list(acs);
			return;
		}
		++i;
	}
}

/**
 * @brief Prints all avaiable actions for given AtkObject.
 *
 * Function prints all avaiable actions for given Accessible Object
 * and register callback for click on button.
 *
 * @param eo Actions Layout object
 * @param obj AtkObject which currently has focus
 * @param parent Main Layout Object
 */
void print_actions(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	char *c = NULL;
	int actions_counter = atk_action_get_n_actions(ATK_ACTION(obj));
	Evas_Object *action_list = elm_object_part_content_get(eo, "elm.editor.name.list");
	Evas_Object *entry = elm_object_part_content_get(eo, "elm.editor.name.entry");
	elm_entry_entry_set(entry, NULL);
	if (asprintf(&c,"%d", actions_counter) < 0 )
		EINA_LOG_ERR(ERR_ACTION);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), c);
	acs->atk_obj = obj;
	acs->list = NULL;
	acs->list = eina_list_append(acs->list, action_list);
	acs->list = eina_list_append(acs->list, entry);
	_fill_action_list(acs);

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _action_cb);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _action_cb, acs);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _set_action_desc);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _set_action_desc, acs);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(c);
}

/**
 * @brief prepares Action interface view
 *
 * Function prepares action Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Evas_Object wich represent Parent Layout
 *
 * @return Evas_Object representing Action Layout
 */
Evas_Object* prepare_layout_action(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);
	elm_layout_file_set(eo, EDJ_PATH_ACT, "explorer/action");

	Evas_Object *lb1, *separator, *button, *list, *entry, *button2;

	lb1 = elm_label_add(eo);
	list = elm_list_add(eo);
	elm_list_multi_select_set(list, EINA_FALSE);
	separator = elm_separator_add(eo);
	button = elm_button_add(eo);
	button2 = elm_button_add(eo);
	entry = elm_entry_add(eo);
	elm_entry_single_line_set(entry, EINA_TRUE);
	elm_entry_scrollable_set(entry, EINA_TRUE);
	elm_object_part_text_set(entry, "guide", ACTION_DESC);
	elm_object_text_set(button, DO_ACTION);
	elm_object_text_set(button2, SET);
	elm_separator_horizontal_set(separator, EINA_FALSE);
	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.list", list);
	elm_object_part_content_set(eo, "elm.editor.name.button", button);
	elm_object_part_content_set(eo, "elm.separator.1", separator);
	elm_object_part_content_set(eo, "elm.editor.name.entry", entry);
	elm_object_part_content_set(eo, "elm.editor.name.button2", button2);

	evas_object_color_set(separator, 1, 1, 1, 255);
	elm_object_part_text_set(eo, "text1", NUM_OF_ACTIONS);
	elm_object_part_text_set(eo, "text2", ACC_DESC);
	elm_object_part_text_set(eo, "text3", SET_DESC);
	elm_label_slide_duration_set(lb1, 3);
	elm_object_style_set(lb1, "slide_bounce");

	return eo;
}

/**
 * @brief callback for resize event
 *
 * Callback called when resize button on component layout is clicked,
 * resize on focused object is performed with paramenters received from
 * data parameter
 *
 * @param data action_cb_struct which has values from entries in layout
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _component_resize_cb(void *data, Evas_Object *obj, void *event_info)
{
	int width, height;
	char *size;

	action_cb_struct *acs = (action_cb_struct*)data;
	Evas_Object *eo1 = eina_list_nth(acs->list, 0);
	Evas_Object *eo2 = eina_list_nth(acs->list, 1);
	Evas_Object *lab = eina_list_nth(acs->list, 6);

	atk_component_set_size(ATK_COMPONENT(acs->atk_obj), atoi(elm_entry_entry_get(eo1)), atoi(elm_entry_entry_get(eo2)));

	atk_component_get_size(ATK_COMPONENT(acs->atk_obj), &width, &height);

	if(asprintf(&size, "%s: %d, %s: %d", WIDTH, width, HEIGHT, height) < 0)
	{
		elm_object_text_set(lab, "Error");
	}
	else
	{
		elm_object_text_set(lab, size);
	}

	free(size);
}

/**
 * @brief callback for change position event
 *
 * Callback called when change position button on component layout is clicked,
 * position change on focued object is prerformed with parameters received from
 * data parameter.
 *
 * @param data action_cb_struct which has values from entries in layout
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _component_change_position_cb(void *data, Evas_Object *obj, void *event_info)
{
	int s_x, s_y;
	int w_x, w_y;
	char *s_position;
	char *w_position;

	action_cb_struct *acs = (action_cb_struct*)data;
	Evas_Object *eo1 = eina_list_nth(acs->list, 2);
	Evas_Object *eo2 = eina_list_nth(acs->list, 3);
	Evas_Object *lab = eina_list_nth(acs->list, 4);
	Evas_Object *lab1 = eina_list_nth(acs->list, 5);

	atk_component_set_position(ATK_COMPONENT(acs->atk_obj), atoi(elm_entry_entry_get(eo1)), atoi(elm_entry_entry_get(eo2)), ATK_XY_WINDOW);

	atk_component_get_position(ATK_COMPONENT(acs->atk_obj), &s_x, &s_y, ATK_XY_SCREEN);
	atk_component_get_position(ATK_COMPONENT(acs->atk_obj), &w_x, &w_y, ATK_XY_WINDOW);

	if(asprintf(&s_position, "X: %d, Y: %d", s_x, s_y) < 0)
	{
		EINA_LOG_ERR(ERR_MEMORY);
		elm_object_text_set(lab, "-");
	}
	else
	{
		elm_object_text_set(lab, s_position);
	}
	if(asprintf(&w_position, "X: %d, Y: %d", w_x, w_y) < 0)
	{
		EINA_LOG_ERR(ERR_MEMORY);
		elm_object_text_set(lab1, "-");
	}
	else
	{
		elm_object_text_set(lab1, w_position);
	}

	free(s_position);
	free(w_position);
}

/**
 * @brief prints Component layout
 *
 * Main function to print layout component, called when click on component
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Component Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_component(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	int s_x, s_y;
	int w_x, w_y;
	int atk_mdi_zorder;
	int width, height;
	double atk_alpha;
	AtkLayer atk_layer;
	char *s_position = NULL;
	char *w_position = NULL;
	char *size = NULL;
	char *layer = NULL;
	char *mdi_zorder = NULL;
	char *alpha = NULL;
	char *width_char = WIDTH;
	char *height_char = HEIGHT;
	acs->list = NULL;

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.button"), RESIZE);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.button1"), CHANGE_POS);

	atk_component_get_position(ATK_COMPONENT(obj), &s_x, &s_y, ATK_XY_SCREEN);
	atk_component_get_position(ATK_COMPONENT(obj), &w_x, &w_y, ATK_XY_WINDOW);
	atk_component_get_size(ATK_COMPONENT(obj), &width, &height);
	atk_layer = atk_component_get_layer(ATK_COMPONENT(obj));
	atk_mdi_zorder = atk_component_get_mdi_zorder(ATK_COMPONENT(obj));
	atk_alpha = atk_component_get_alpha(ATK_COMPONENT(obj));

	if (asprintf(&s_position,"X: %d, Y: %d", s_x, s_y) < 0)
		EINA_LOG_ERR(ERR_SET_POSITION);
	if (asprintf(&w_position,"X: %d, Y: %d", w_x, w_y) < 0)
		EINA_LOG_ERR(ERR_SET_POSITION);
	if (asprintf(&size, "%s: %d, %s: %d", width_char, width, height_char, height) < 0)
		EINA_LOG_ERR(ERR_SET_POSITION);
	if (asprintf(&layer, "%d",  atk_layer) < 0)
		EINA_LOG_ERR(ERR_ATK_LAYER);
	if (asprintf(&mdi_zorder, "%i", atk_mdi_zorder) < 0)
		EINA_LOG_ERR(ERR_MDI_ZORDER);
	if (asprintf(&alpha, "%g", atk_alpha) < 0)
		EINA_LOG_ERR(ERR_ATK_ALPHA);
	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry2"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry3"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry4"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label2"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label3"));

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), s_position);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label2"), w_position);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), size);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), layer);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), mdi_zorder);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label6"), alpha);

	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry1"), NULL);
	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry2"), NULL);
	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry3"), NULL);
	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry4"), NULL);

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _component_resize_cb);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _component_change_position_cb);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _component_resize_cb, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _component_change_position_cb, acs);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(s_position);
	free(w_position);
	free(size);
	free(layer);
	free(mdi_zorder);
	free(alpha);
}

/**
 * @brief prepares Component interface view
 *
 * Function prepares Component Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing component layout
 */
Evas_Object* prepare_layout_component(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);
	elm_layout_file_set(eo, EDJ_PATH_CMP, "explorer/component");

	Evas_Object *lb1, *lb2, *lb3, *lb4, *lb5, *lb6;
	Evas_Object *e1, *e2, *e3, *e4, *separator;
	Evas_Object *button, *button1;
	Evas_Object *box1, *box2, *box3, *box4;

	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);
	lb5 = elm_label_add(eo);
	lb6 = elm_label_add(eo);

	box1 = elm_box_add(eo);
	box2 = elm_box_add(eo);
	box3 = elm_box_add(eo);
	box4 = elm_box_add(eo);

	e1 = elm_entry_add(eo);
	e2 = elm_entry_add(eo);
	e3 = elm_entry_add(eo);
	e4  = elm_entry_add(eo);

	button = elm_button_add(eo);
	button1 = elm_button_add(eo);

	separator = elm_separator_add(eo);

	elm_separator_horizontal_set(separator, EINA_FALSE);

	evas_object_size_hint_weight_set(e1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e1, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e1, EINA_TRUE);
	elm_entry_scrollable_set(e1, EINA_TRUE);
	elm_box_pack_end(box1, e1);

	evas_object_size_hint_weight_set(e2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e2, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e2, EINA_TRUE);
	elm_entry_scrollable_set(e2, EINA_TRUE);
	elm_box_pack_end(box2, e2);

	evas_object_size_hint_weight_set(e3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e3, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e3, EINA_TRUE);
	elm_entry_scrollable_set(e3, EINA_TRUE);
	elm_box_pack_end(box3, e3);

	evas_object_size_hint_weight_set(e4, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e4, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e4, EINA_TRUE);
	elm_entry_scrollable_set(e4, EINA_TRUE);
	elm_box_pack_end(box4, e4);


	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb4);
	elm_object_part_content_set(eo, "elm.editor.name.label5", lb5);
	elm_object_part_content_set(eo, "elm.editor.name.label6", lb6);

	elm_object_part_content_set(eo, "elm.editor.name.entry1", e1);
	elm_object_part_content_set(eo, "elm.editor.name.entry2", e2);
	elm_object_part_content_set(eo, "elm.editor.name.entry3", e3);
	elm_object_part_content_set(eo, "elm.editor.name.entry4", e4);

	elm_object_part_content_set(eo, "elm.editor.name.box1", box1);
	elm_object_part_content_set(eo, "elm.editor.name.box2", box2);
	elm_object_part_content_set(eo, "elm.editor.name.box3", box3);
	elm_object_part_content_set(eo, "elm.editor.name.box4", box4);

	elm_object_part_content_set(eo, "elm.editor.name.button", button);
	elm_object_part_content_set(eo, "elm.editor.name.button1", button1);

	elm_object_part_content_set(eo, "elm.separator.1", separator);

	elm_object_part_text_set(e1, "guide", ENTER_VAL);
	elm_object_part_text_set(e2, "guide", ENTER_VAL);
	elm_object_part_text_set(e3, "guide", ENTER_VAL);
	elm_object_part_text_set(e4, "guide", ENTER_VAL);

	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_object_part_text_set(eo, "text1", POS_ON_SCREEN);
	elm_object_part_text_set(eo, "text2", POS_IN_WIN);
	elm_object_part_text_set(eo, "text3", SIZE);
	elm_object_part_text_set(eo, "text4", LAYER);
	elm_object_part_text_set(eo, "text5", MDI_ZORDER);
	elm_object_part_text_set(eo, "text6", ALPHA);
	elm_object_part_text_set(eo, "text7", SET_SIZE);
	elm_object_part_text_set(eo, "text8", WIDTH);
	elm_object_part_text_set(eo, "text9", HEIGHT);
	elm_object_part_text_set(eo, "text10", CHANGE_POS);
	elm_object_part_text_set(eo, "text11", X);
	elm_object_part_text_set(eo, "text12", Y);

	return eo;
}

/**
 * @brief callback for set atk object name
 *
 * That callback function is called when "Set name" button
 * on object layout is clicked, it set Accessible Name for
 * focused object.
 *
 * @param data action_cb_struct which contains value from entry
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _set_name_obj_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *e1 = eina_list_nth(acs->list, 0);
	Evas_Object *e2 = eina_list_nth(acs->list, 1);
	const char *text = elm_entry_entry_get(e2);
	if(text == NULL)
	{
		text = "";
	}
	atk_object_set_name(acs->atk_obj, text);
	elm_object_text_set(e1, atk_object_get_name(acs->atk_obj));
	selected_item_update();
}

/**
 * @brief callback for set atk object description
 *
 * That callback function is called when "Set description" button
 * on object layout is clicked, it set Accessible Description for
 * focused object.
 *
 * @param data action_cb_struct which contains value from entry
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _set_desc_obj_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *e1 = eina_list_nth(acs->list, 2);
	Evas_Object *e2 = eina_list_nth(acs->list, 3);
	const char *text = elm_entry_entry_get(e2);
	if(text == NULL)
	{
		text = "";
	}
	atk_object_set_description(acs->atk_obj, text);
	elm_object_text_set(e1, atk_object_get_description(acs->atk_obj));
}

/**
 * @brief prints Object layout
 *
 * Main function to print object layout, called when click object label
 * in toolbar is clicked or when focus is changed to new object,
 * that function fills all entries and labels in that layout,
 * register callbacks on objects
 *
 * @param eo Object Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_object(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	AtkObject *parent_obj;
	AtkRole role;
	AtkRelationSet *relationSet;
	AtkStateSet *stateSet;
	Evas_Object *btn1, *btn2;

	parent_obj = atk_object_get_parent(obj);
	role = atk_object_get_role(obj);
	relationSet = atk_object_ref_relation_set(obj);
	stateSet = atk_object_ref_state_set(obj);

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), g_type_name(G_TYPE_FROM_INSTANCE(obj)));
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label2"), atk_object_get_name(obj));
	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry1"), NULL);

	if(parent_obj)
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), g_type_name(G_TYPE_FROM_INSTANCE(parent_obj)));
		const char *parent_name = atk_object_get_name(parent_obj);
		if(parent_name)
		{
			elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), atk_object_get_name(parent_obj));
		}
		else
		{
			elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), NO_NAME);
		}
		if(atk_object_get_index_in_parent(obj) == -1)
		{
			elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), INDEX_ERR);
		}
		else
		{
			char *c = NULL;
			if (asprintf(&c,"%d", atk_object_get_index_in_parent(parent_obj)) < 0)
				EINA_LOG_ERR(ERR_OBJECT_INDEX);
			elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label6"), c);
			free(c);
		}
	}
	else
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), NO_PAR);
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), NO_PAR);
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), NO_PAR);
	}
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label6"), atk_role_get_name(role));
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label7"), atk_object_get_description(obj));
	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry2"), NULL);
	char *c = NULL;
	if (asprintf(&c,"%d", atk_object_get_n_accessible_children(obj)) < 0)
		EINA_LOG_ERR(ERR_CHILD_COUNT);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label9"),c);
	free(c);
	if (atk_relation_set_get_n_relations(relationSet) != 0 )
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label9"), EXISTS);
	else
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label9"), NOT_EXISTS);

	if (atk_state_set_is_empty(stateSet))
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label10"), NO_STATES);
	}
	else
	{
		char *states = NULL;
		AtkStateType i;
		for (i = ATK_STATE_INVALID; i <= ATK_STATE_LAST_DEFINED; i++) {
			if (atk_state_set_contains_state(stateSet, i)){
				const char *state = atk_state_type_get_name(i);
				if(!states)
				{
					if(asprintf(&states, "%s", state) < 0)
						EINA_LOG_ERR(ERR_WRITE_STATES);
				}
				else
				{
					char *temp = NULL;
					if(asprintf(&temp, "%s, %s", states, state) < 0)
						EINA_LOG_ERR(ERR_WRITE_STATES);
					free(states);
					states = temp;
				}
			}
		}
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label10"), states);
		free(states);
	}

	g_object_unref(stateSet);

	btn1 = elm_object_part_content_get(eo, "elm.editor.name.button1");
	btn2 = elm_object_part_content_get(eo, "elm.editor.name.button2");
	acs->list = NULL;
	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label2"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label7"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry2"));

	evas_object_smart_callback_add(btn1, "clicked", _set_name_obj_cb, acs);

	evas_object_smart_callback_add(btn2, "clicked", _set_desc_obj_cb, acs);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
}

/**
 * @brief prepares Object interface view
 *
 * Function prepares Object Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing Object layout
 */
Evas_Object* prepare_layout_object(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);
	elm_layout_file_set(eo, EDJ_PATH_OBJ, "explorer/object");

	Evas_Object *lb1, *lb2, *lb3, *lb4, *lb5, *lb6, *lb7, *lb8, *lb9, *lb10;
	Evas_Object *e3, *e9;
	Evas_Object *separator, *btn1, *btn2;
	Evas_Object *box1, *box2;

	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);
	lb5 = elm_label_add(eo);
	lb6 = elm_label_add(eo);
	lb7 = elm_label_add(eo);
	lb8 = elm_label_add(eo);
	lb9 = elm_label_add(eo);
	lb10 = elm_label_add(eo);

	btn1 = elm_button_add(eo);
	btn2 = elm_button_add(eo);

	box1 = elm_box_add(eo);
	box2 = elm_box_add(eo);

	separator = elm_separator_add(eo);
	elm_separator_horizontal_set(separator, EINA_FALSE);

	e3 = elm_entry_add(eo);
	e9 = elm_entry_add(eo);

	evas_object_size_hint_weight_set(e3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e3, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e3, EINA_TRUE);
	elm_entry_scrollable_set(e3, EINA_TRUE);
	elm_box_pack_end(box1, e3);

	evas_object_size_hint_weight_set(e9, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e9, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_line_wrap_set(e9, EINA_TRUE);
	elm_entry_scrollable_set(e9, EINA_TRUE);
	elm_box_pack_end(box2, e9);

	elm_object_part_content_set(eo, "elm.editor.name.box1", box1);
	elm_object_part_content_set(eo, "elm.editor.name.box2", box2);
	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb4);
	elm_object_part_content_set(eo, "elm.editor.name.label5", lb5);
	elm_object_part_content_set(eo, "elm.editor.name.label6", lb6);
	elm_object_part_content_set(eo, "elm.editor.name.label7", lb7);
	elm_object_part_content_set(eo, "elm.editor.name.label8", lb8);
	elm_object_part_content_set(eo, "elm.editor.name.label9", lb9);
	elm_object_part_content_set(eo, "elm.editor.name.label10", lb10);

	elm_object_part_content_set(eo, "elm.editor.name.entry1", e3);
	elm_object_part_content_set(eo, "elm.editor.name.entry2", e9);

	elm_object_part_content_set(eo, "elm.editor.name.button1", btn1);
	elm_object_part_content_set(eo, "elm.editor.name.button2", btn2);

	elm_object_part_content_set(eo, "elm.separator.1", separator);

	elm_entry_editable_set(e3, EINA_TRUE);
	elm_entry_editable_set(e9, EINA_TRUE);

	elm_entry_single_line_set(e3, EINA_TRUE);
	elm_entry_single_line_set(e9, EINA_TRUE);

	elm_object_part_text_set(e3, "guide", ENTER_NAME);
	elm_object_part_text_set(e9, "guide", ENTER_DESC);

	elm_object_part_text_set(eo, "text1", ACC_TYPE);
	elm_object_part_text_set(eo, "text2", ACC_NAME);
	elm_object_part_text_set(eo, "text3", ACC_NAME_TO_SET);
	elm_object_part_text_set(eo, "text4", PAR_ACC_TYPE);
	elm_object_part_text_set(eo, "text5", PAR_ACC_NAME);
	elm_object_part_text_set(eo, "text6", INDEX_IN_PAR);
	elm_object_part_text_set(eo, "text8", ACC_DESC);
	elm_object_part_text_set(eo, "text9", ACC_DESC_TO_SET);
	elm_object_part_text_set(eo, "text7", ACC_ROLE);
	elm_object_part_text_set(eo, "text10", NUM_OF_CHILDREN);
	elm_object_part_text_set(eo, "text11", NUM_OF_RELATIONS);
	elm_object_part_text_set(eo, "text12", ACC_STATES);

	elm_label_slide_duration_set(lb1, 3);
	elm_label_slide_duration_set(lb2, 3);
	elm_label_slide_duration_set(lb3, 3);
	elm_label_slide_duration_set(lb4, 3);
	elm_label_slide_duration_set(lb5, 3);
	elm_label_slide_duration_set(lb6, 3);
	elm_label_slide_duration_set(lb7, 3);
	elm_label_slide_duration_set(lb8, 3);
	elm_label_slide_duration_set(lb9, 3);
	elm_label_slide_duration_set(lb10, 3);

	elm_object_style_set(lb1, "slide_bounce");
	elm_object_style_set(lb2, "slide_bounce");
	elm_object_style_set(lb3, "slide_bounce");
	elm_object_style_set(lb4, "slide_bounce");
	elm_object_style_set(lb5, "slide_bounce");
	elm_object_style_set(lb6, "slide_bounce");
	elm_object_style_set(lb7, "slide_bounce");
	elm_object_style_set(lb8, "slide_bounce");
	elm_object_style_set(lb9, "slide_bounce");
	elm_object_style_set(lb10, "slide_bounce");

	elm_label_line_wrap_set(lb10, ELM_WRAP_WORD);

	evas_object_color_set(separator, 1, 1, 1, 255);
	elm_object_part_text_set(btn1, "default", SET_NAME);
	elm_object_part_text_set(btn2, "default", SET_DESC);

	return eo;
}

/**
 * @brief changes a row's description in a table
 *
 * Interface table allow to set description for row,
 * that function take description from entry after
 * "Set description" and set Accessible row description
 *
 * @param data action cb struct which contains entry field from layout and atk object
 * @param obj Layout object
 * @param event_info UNUSED
 */
void _table_change_row_desc(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo = eina_list_nth(acs->list, 0);
	int int_row_n = atk_table_get_n_rows(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 0; i < int_row_n; ++i)
	{
		if(atk_table_is_row_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_set_row_description(ATK_TABLE(acs->atk_obj), i, elm_entry_entry_get(eo));
		}
	}
}

/**
 * @brief changes a column's description in a table
 *
 * Interface table allow to set description for column,
 * that function take description from entry after
 * "Set description" and set Accessible column description
 *
 * @param data action cb struct which contains entry field from layout and atk object
 * @param obj Layout object
 * @param event_info UNUSED
 */
void _table_change_col_desc(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo = eina_list_nth(acs->list, 1);
	int int_col_n = atk_table_get_n_columns(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 0; i < int_col_n; ++i)
	{
		if(atk_table_is_column_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_set_column_description(ATK_TABLE(acs->atk_obj), i, elm_entry_entry_get(eo));
		}
	}
}

/**
 * @brief selects next column in a table
 *
 * That callback is called when "Next column" button is selected in
 * table layout, it add selection to next column.
 *
 * @param data action cb struct which contains given table object
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _table_next_col(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	int int_col_n = atk_table_get_n_columns(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 0; i < int_col_n - 1; ++i)
	{
		if(atk_table_is_column_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_add_column_selection(ATK_TABLE(acs->atk_obj), i + 1);
			return;
		}
	}
	atk_table_add_column_selection(ATK_TABLE(acs->atk_obj), 0);
}

/**
 * @brief selects previous column in a table
 *
 * That callback is called when "Previous column" button is selected in
 * table layout, it add selection to previous column.
 *
 * @param data action cb struct which contains given table object
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _table_prev_col(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	int int_col_n = atk_table_get_n_columns(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 1; i < int_col_n; ++i)
	{
		if(atk_table_is_column_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_add_column_selection(ATK_TABLE(acs->atk_obj), i - 1);
			return;
		}
	}
	atk_table_add_column_selection(ATK_TABLE(acs->atk_obj), int_col_n - 1);
}

/**
 * @brief selects next row in a table
 *
 * That callback is called when "Next row" button is selected in
 * table layout, it add selection to next row.
 *
 * @param data action cb struct which contains given table object
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _table_next_row(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	int int_row_n = atk_table_get_n_rows(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 1; i < int_row_n; ++i)
	{
		if(atk_table_is_row_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_add_row_selection(ATK_TABLE(acs->atk_obj), i - 1);
			return;
		}
	}
	atk_table_add_row_selection(ATK_TABLE(acs->atk_obj), int_row_n - 1);
}

/**
 * @brief selects previous row in a table
 *
 * That callback is called when "Previous row" button is selected in
 * table layout, it add selection to previous row.
 *
 * @param data action cb struct which contains given table object
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _table_prev_row(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	int int_row_n = atk_table_get_n_rows(ATK_TABLE(acs->atk_obj));
	int i;
	for(i = 1; i < int_row_n; ++i)
	{
		if(atk_table_is_row_selected(ATK_TABLE(acs->atk_obj), i))
		{
			atk_table_add_row_selection(ATK_TABLE(acs->atk_obj), i - 1);
			return;
		}
	}
	atk_table_add_row_selection(ATK_TABLE(acs->atk_obj), int_row_n - 1);
}

/**
 * @brief prints Table layout
 *
 * Main function to print table layout, called when click on table
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Table Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_table(Evas_Object *eo, AtkObject *obj, Evas_Object *tb, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	char *rows_n = NULL;
	char *cols_n = NULL;

	int int_row_n = atk_table_get_n_rows(ATK_TABLE(obj));
	int int_col_n = atk_table_get_n_columns(ATK_TABLE(obj));

	if (asprintf(&rows_n, "%d", int_row_n) < 0)
		EINA_LOG_ERR(ERR_ROW);
	if (asprintf(&cols_n, "%d", int_col_n) < 0)
		EINA_LOG_ERR(ERR_COLUMN);

	const char *sel_col_desc = NULL;
	const char *sel_row_desc = NULL;
	acs->list = NULL;

	int i;
	for(i = 0; i < int_row_n; ++i)
	{
		if(atk_table_is_row_selected(ATK_TABLE(obj), i))
		{
			sel_row_desc = atk_table_get_row_description(ATK_TABLE(obj), i);
		}
	}
	for(i = 0; i < int_col_n; ++i)
	{
		if(atk_table_is_column_selected(ATK_TABLE(obj), i))
		{
			sel_col_desc = atk_table_get_column_description(ATK_TABLE(obj), i);
		}
	}

	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry2"));

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _table_change_row_desc);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _table_change_col_desc);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button3"), "clicked", _table_next_col);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button4"), "clicked", _table_prev_col);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button5"), "clicked", _table_next_row);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button6"), "clicked", _table_prev_row);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _table_change_row_desc, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _table_change_col_desc, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button3"), "clicked", _table_next_col, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button4"), "clicked", _table_prev_col, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button5"), "clicked", _table_next_row, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button6"), "clicked", _table_prev_row, acs);

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), cols_n);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label2"), rows_n);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), sel_row_desc);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), sel_col_desc);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(rows_n);
	free(cols_n);
}

/**
 * @brief prepares Table interface view
 *
 * Function prepares table Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing table layout
 */
Evas_Object* prepare_layout_table(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);
	elm_layout_file_set(eo, EDJ_PATH_TBL, "explorer/table");

	Evas_Object *separator;
	Evas_Object *lb1, *lb2, *lb3, *lb4, *e1, *e2;
	Evas_Object *button1, *button2, *button3, *button4, *button5, *button6;
	Evas_Object *box1, *box2;

	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);

	box1 = elm_box_add(eo);
	box2 = elm_box_add(eo);

	e1 = elm_entry_add(eo);
	e2 = elm_entry_add(eo);

	button1 = elm_button_add(eo);
	button2 = elm_button_add(eo);
	button3 = elm_button_add(eo);
	button4 = elm_button_add(eo);
	button5 = elm_button_add(eo);
	button6 = elm_button_add(eo);

	separator = elm_separator_add(eo);
	elm_separator_horizontal_set(separator, EINA_FALSE);

	evas_object_size_hint_weight_set(e1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e1, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_scrollable_set(e1, EINA_TRUE);
	elm_entry_line_wrap_set(e1, EINA_TRUE);
	elm_box_pack_end(box1, e1);

	evas_object_size_hint_weight_set(e2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e2, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_scrollable_set(e2, EINA_TRUE);
	elm_entry_line_wrap_set(e2, EINA_TRUE);
	elm_box_pack_end(box2, e2);

	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb4);

	elm_object_part_content_set(eo, "elm.editor.name.entry1", e1);
	elm_object_part_content_set(eo, "elm.editor.name.entry2", e2);

	elm_object_part_content_set(eo, "elm.editor.name.box1", box1);
	elm_object_part_content_set(eo, "elm.editor.name.box2", box2);

	elm_object_part_content_set(eo, "elm.editor.name.button1", button1);
	elm_object_part_content_set(eo, "elm.editor.name.button2", button2);
	elm_object_part_content_set(eo, "elm.editor.name.button3", button3);
	elm_object_part_content_set(eo, "elm.editor.name.button4", button4);
	elm_object_part_content_set(eo, "elm.editor.name.button5", button5);
	elm_object_part_content_set(eo, "elm.editor.name.button6", button6);

	elm_object_part_content_set(eo, "elm.separator.1", separator);
	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_label_slide_duration_set(lb1, 3);
	elm_label_slide_duration_set(lb2, 3);
	elm_label_slide_duration_set(lb3, 3);
	elm_label_slide_duration_set(lb4, 3);

	elm_object_style_set(lb1, "slide_bounce");
	elm_object_style_set(lb2, "slide_bounce");
	elm_object_style_set(lb3, "slide_bounce");
	elm_object_style_set(lb4, "slide_bounce");

	elm_entry_editable_set(e1, EINA_TRUE);
	elm_entry_editable_set(e2, EINA_TRUE);

	elm_entry_single_line_set(e1, EINA_TRUE);
	elm_entry_single_line_set(e2, EINA_TRUE);

	elm_object_part_text_set(e1, "guide", ENTER_DESC);
	elm_object_part_text_set(e2, "guide", ENTER_DESC);

	elm_object_text_set(button1, SET_DESC);
	elm_object_text_set(button2, SET_DESC);
	elm_object_text_set(button3, NEXT_COL);
	elm_object_text_set(button4, PREV_COL);
	elm_object_text_set(button5, NEXT_ROW);
	elm_object_text_set(button6, PREV_ROW);

	elm_object_part_text_set(eo, "text1", NUM_OF_ROWS);
	elm_object_part_text_set(eo, "text2", NUM_OF_COLS);
	elm_object_part_text_set(eo, "text3", SEL_ROW_DESC);
	elm_object_part_text_set(eo, "text4", SEL_COL_DESC);
	elm_object_part_text_set(eo, "text5", SET_SEL_ROW_DESC);
	elm_object_part_text_set(eo, "text6", SET_SEL_COL_DESC);
	elm_object_part_text_set(eo, "text7", SEL_NEXT_COL);
	elm_object_part_text_set(eo, "text8", SEL_PREV_COL);
	elm_object_part_text_set(eo, "text9", SEL_NEXT_ROW);
	elm_object_part_text_set(eo, "text10", SEL_PREV_ROW);

	return eo;
}

/**
 * @brief selects the first element
 *
 * Callback called when "select first child" button on Selection Layout is clicked,
 * it perform selection for the first object in object set.
 *
 * @param data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _select_first_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), 0);
}

/**
 * @brief selects the last element
 *
 * Callback called when "select last child" button on Selection Layout is clicked,
 * it perform selection for the last object in object set.
 *
 * @param data data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _select_last_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	int counter = atk_object_get_n_accessible_children(acs->atk_obj);
	atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), counter - 1);
}

/**
 * @brief selects the next element
 *
 * Callback called when "select next child" button on Selection Layout is clicked,
 * it perform selection for the next object in object set.
 *
 * @param data data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _select_next_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	int i;
	int counter = atk_object_get_n_accessible_children(acs->atk_obj);
	for(i = 0; i < counter - 1; ++i)
	{
		if(atk_selection_is_child_selected(ATK_SELECTION(acs->atk_obj), i))
		{
			atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), i + 1);
			return;
		}
	}
	atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), 0);
}

/**
 * @brief selects the previous element
 *
 * Callback called when "select previous child" button on Selection Layout is clicked,
 * it perform selection for the previous object in object set.
 *
 * @param data data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _select_prev_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	int i;
	int counter = atk_object_get_n_accessible_children(acs->atk_obj);
	for(i = 1; i < counter; ++i)
	{
		if(atk_selection_is_child_selected(ATK_SELECTION(acs->atk_obj), i))
		{
			atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), i - 1);
			return;
		}
	}
	atk_selection_add_selection(ATK_SELECTION(acs->atk_obj), counter - 1);
}

/**
 * @brief clears the selection
 *
 * Callback called when "clear selection" button on Selection Layout is clicked,
 * it perform selection clean for all objects in object set.
 *
 * @param data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _clear_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	atk_selection_clear_selection(ATK_SELECTION(acs->atk_obj));
}

/**
 * @brief selects all the elements
 *
 * Callback called when "select all children" button on Selection Layout is clicked,
 * it perform selection for the all objects in object set.
 *
 * @param data action_cb_stuct, which contains object with avaiable selection interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _all_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	atk_selection_select_all_selection(ATK_SELECTION(acs->atk_obj));
}

/**
 * @brief prints the Selection layout
 *
 * Main function to print selection layout, called when click on selection
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Selection Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_selection(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	acs->atk_obj = obj;
	acs->list = NULL;

	char *children = NULL;
	Evas_Object *lb1 = elm_object_part_content_get(eo, "elm.selection.name.label1");
	if (asprintf(&children,"%d", atk_selection_get_selection_count(ATK_SELECTION(obj))) < 0 )
		EINA_LOG_ERR(ERR_SELECTION);
	elm_object_text_set(lb1, children);

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button1"), "clicked", _select_first_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button2"), "clicked", _select_last_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button3"), "clicked", _select_next_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button4"), "clicked", _select_prev_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button5"), "clicked", _clear_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.selection.name.button6"), "clicked", _all_selection);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button1"), "clicked", _select_first_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button2"), "clicked", _select_last_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button3"), "clicked", _select_next_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button4"), "clicked", _select_prev_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button5"), "clicked", _clear_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.selection.name.button6"), "clicked", _all_selection, acs);


	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(children);
}

/**
 * @brief prepares Selection interface view
 *
 * Function prepares Selection Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing Selection layout
 */
Evas_Object* prepare_layout_selection(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);
	elm_layout_file_set(eo, EDJ_PATH_SEL, "explorer/selection");
	Evas_Object *lb1, *separator, *button1, *button2, *button3;
	Evas_Object *button4, *button5, *button6;

	lb1 = elm_label_add(eo);
	button1 = elm_button_add(eo);
	button2 = elm_button_add(eo);
	button3 = elm_button_add(eo);
	button4 = elm_button_add(eo);
	button5 = elm_button_add(eo);
	button6 = elm_button_add(eo);

	elm_object_text_set(button1, SEL);
	elm_object_text_set(button2, SEL);
	elm_object_text_set(button3, SEL);
	elm_object_text_set(button4, SEL);
	elm_object_text_set(button5, SEL);
	elm_object_text_set(button6, SEL);

	separator = elm_separator_add(eo);
	elm_separator_horizontal_set(separator, EINA_FALSE);

	elm_object_part_content_set(eo, "elm.selection.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.selection.name.button1", button1);
	elm_object_part_content_set(eo, "elm.selection.name.button2", button2);
	elm_object_part_content_set(eo, "elm.selection.name.button3", button3);
	elm_object_part_content_set(eo, "elm.selection.name.button4", button4);
	elm_object_part_content_set(eo, "elm.selection.name.button5", button5);
	elm_object_part_content_set(eo, "elm.selection.name.button6", button6);
	elm_object_part_content_set(eo, "elm.separator.1", separator);

	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_label_slide_duration_set(lb1, 3);
	elm_object_style_set(lb1, "slide_bounce");


	elm_object_part_text_set(eo, "text1", NUM_OF_SEL_CHLD);
	elm_object_part_text_set(eo, "text2", SEL_FIRST_CHILD);
	elm_object_part_text_set(eo, "text3", SEL_LAST_CHILD);
	elm_object_part_text_set(eo, "text4", SEL_NEXT_CHILD);
	elm_object_part_text_set(eo, "text5", SEL_PREV_CHILD);
	elm_object_part_text_set(eo, "text6", CLEAR_SEL);
	elm_object_part_text_set(eo, "text7", SEL_ALL_CHILDREN);

	return eo;
}

/**
 * @brief change image description
 *
 * Callback called when set image description button is clicked.
 *
 * @param data action_cb_stuct, which contains object with avaiable image interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _set_desc_img_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}

	Evas_Object *entry = eina_list_nth(acs->list, 0);
	Evas_Object *label = eina_list_nth(acs->list, 1);

	if(atk_image_set_image_description(ATK_IMAGE(acs->atk_obj), elm_entry_entry_get(entry)))
		elm_object_text_set(label, atk_image_get_image_description(ATK_IMAGE(acs->atk_obj)));
	else
		EINA_LOG_ERR(ERR_DESCRIPTION);
}

/**
 * @brief prints the Image layout
 *
 * Main function to print image layout, called when click on image
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Image Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_image(Evas_Object *eo, AtkObject *obj, Evas_Object *tb, Evas_Object *parent)
{
	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	acs->list = NULL;

	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label1"));

	const char *description = atk_image_get_image_description(ATK_IMAGE(obj));
	if(description)
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), description);
	}
	else
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), NO_DESC);
	}

	int x, y;
	atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_WINDOW);
	char *position = NULL;
	if (asprintf(&position,"X: %d Y: %d", x, y) < 0)
		EINA_LOG_ERR(ERR_GET_POSITION);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label2"), position);

	atk_image_get_image_size(ATK_IMAGE(obj), &x, &y);
	char *size = NULL;
	if (asprintf(&size, "Width: %d Height: %d", x, y) < 0)
		EINA_LOG_ERR(ERR_GET_POSITION);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), size);

	const char *locale = atk_image_get_image_locale(ATK_IMAGE(obj));
	if(!locale)
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), NO_LOCALE);
	}
	else
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), locale);
	}

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _set_desc_img_cb);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _set_desc_img_cb, acs);

	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry1"), NULL);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(position);
	free(size);
}

/**
 * @brief prepares Image interface view
 *
 * Function prepares Image Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing Image layout
 */
Evas_Object* prepare_layout_image(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);

	elm_layout_file_set(eo, EDJ_PATH_IMG, "explorer/image");
	Evas_Object *lb1, *lb2, *lb3, *lb4, *separator;
	Evas_Object *entry, *bt;

	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);

	entry = elm_entry_add(eo);

	bt = elm_button_add(eo);

	separator = elm_separator_add(eo);
	elm_separator_horizontal_set(separator, EINA_FALSE);

	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb4);
	elm_object_part_content_set(eo, "elm.separator.1", separator);
	elm_object_part_content_set(eo, "elm.editor.name.entry1", entry);
	elm_object_part_content_set(eo, "elm.editor.name.button1", bt);

	elm_label_slide_duration_set(lb1, 3);
	elm_label_slide_duration_set(lb2, 3);
	elm_label_slide_duration_set(lb3, 3);
	elm_label_slide_duration_set(lb4, 3);

	elm_object_style_set(lb1, "slide_bounce");
	elm_object_style_set(lb2, "slide_bounce");
	elm_object_style_set(lb3, "slide_bounce");
	elm_object_style_set(lb4, "slide_bounce");

	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_object_part_text_set(eo, "text1", IMG_DESC);
	elm_object_part_text_set(eo, "text2", SET_IMG_DESC);
	elm_object_part_text_set(eo, "text3", IMG_POS);
	elm_object_part_text_set(eo, "text4", IMG_SIZE);
	elm_object_part_text_set(eo, "text5", IMG_LOCALE);

	elm_entry_editable_set(entry, EINA_TRUE);

	elm_entry_single_line_set(entry, EINA_TRUE);

	elm_entry_scrollable_set(entry, EINA_TRUE);

	elm_object_part_text_set(entry, "guide", ENTER_DESC);

	elm_object_part_text_set(bt, "default", SET_DESC);

	return eo;
}

/**
 * @brief prints gvalue
 *
 * That function print string from GValue object
 *
 * @param val GValue object 
 *
 * @return GValue transformated to string
 */
char* gvalue_pprint(GValue *val)
{
	char *buffer = NULL;

	if(G_VALUE_HOLDS_UINT(val)) {
		if (asprintf(&buffer, "%d", g_value_get_uint(val)) < 0)
			EINA_LOG_ERR(ERR_PRINT_GVALUE);
	}
	else if(G_VALUE_HOLDS_INT(val)) {
		if (asprintf(&buffer, "%d", g_value_get_int(val)) < 0)
			EINA_LOG_ERR(ERR_PRINT_GVALUE);
	}
	else if(G_VALUE_HOLDS_DOUBLE(val)) {
		if (asprintf(&buffer, "%g", g_value_get_double(val)) < 0)
			EINA_LOG_ERR(ERR_PRINT_GVALUE);
	}
	return buffer;
}

/**
 * @brief callback for set new value event
 *
 * Callback which allows to set new value in object which has interface value,
 * called when "set value" button in value interface is clicked
 *
 * @param data action_cb_stuct, which contains object with avaiable value interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _value_set_new_value_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;

	if(!acs->atk_obj)
	{
		return;
	}

	char *curr_value = NULL;

	Evas_Object *entry = eina_list_nth(acs->list, 0);
	Evas_Object *label1 = eina_list_nth(acs->list, 1);

	GValue gvalue = G_VALUE_INIT;
	GValue curr_value_temp = G_VALUE_INIT;

	const char *name = g_type_name(G_TYPE_FROM_INSTANCE(acs->atk_obj));

	if(!strcmp(name, "EailActionSlider"))
	{
		g_value_init (&gvalue, G_TYPE_UINT);
		g_value_set_uint(&gvalue, atoi(elm_entry_entry_get(entry)));
	}
	else if(!strcmp(name, "EailColorselector") || !strcmp(name, "EailClock") || !strcmp(name, "EailDatetime")
	|| !strcmp(name, "EailCalendar"))
	{
		g_value_init (&gvalue, G_TYPE_INT);
		g_value_set_int(&gvalue, atoi(elm_entry_entry_get(entry)));
	}
	else
	{
		g_value_init (&gvalue, G_TYPE_DOUBLE);
		g_value_set_double(&gvalue, atof(elm_entry_entry_get(entry)));
	}

	if(atk_value_set_current_value(ATK_VALUE(acs->atk_obj), &gvalue))
	{
		atk_value_get_current_value(ATK_VALUE(acs->atk_obj), &curr_value_temp);

		curr_value = gvalue_pprint(&curr_value_temp);

		elm_object_text_set(label1, curr_value);
	}
	else
	{
		elm_object_text_set(label1, FAIL_OCC);
	}
	free(curr_value);
}

/**
 * @brief prints value layout
 *
 * Main function to print Value layout, called when click on selection
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Selection Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_value(Evas_Object *eo, AtkObject *obj, Evas_Object *toolbar, Evas_Object *parent)
{
	GValue curr_value_temp = G_VALUE_INIT;
	GValue max_value_temp = G_VALUE_INIT;
	GValue min_value_temp = G_VALUE_INIT;
	GValue min_inc_temp = G_VALUE_INIT;

	g_value_init (&curr_value_temp, G_TYPE_DOUBLE);
	g_value_init (&max_value_temp, G_TYPE_DOUBLE);
	g_value_init (&min_value_temp, G_TYPE_DOUBLE);
	g_value_init (&min_inc_temp, G_TYPE_DOUBLE);

	char *curr_value = NULL;
	char *max_value = NULL;
	char *min_value = NULL;
	char *min_inc = NULL;

	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	acs->list = NULL;

	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.entry1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label1"));

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _value_set_new_value_cb);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _value_set_new_value_cb, acs);

	atk_value_get_current_value(ATK_VALUE(obj), &curr_value_temp);
	atk_value_get_maximum_value(ATK_VALUE(obj), &max_value_temp);
	atk_value_get_minimum_value(ATK_VALUE(obj), &min_value_temp);
	atk_value_get_minimum_increment(ATK_VALUE(obj), &min_inc_temp);

	curr_value = gvalue_pprint(&curr_value_temp);
	max_value = gvalue_pprint(&max_value_temp);
	min_value = gvalue_pprint(&min_value_temp);
	min_inc = gvalue_pprint(&min_inc_temp);

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), curr_value);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label2"), max_value);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), min_value);
	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), min_inc);

	free(curr_value);
	free(max_value);
	free(min_value);
	free(min_inc);

	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.entry1"), NULL);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
}

/**
 * @brief prepares Value interface view
 *
 * Function prepares Value Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing Value layout
 */
Evas_Object* prepare_layout_value(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);

	elm_layout_file_set(eo, EDJ_PATH_VAL, "explorer/value");

	Evas_Object *lb1, *lb2, *lb3, *lb4, *e1, *button, *separator, *box1;

	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);
	box1 = elm_box_add(eo);
	e1 = elm_entry_add(eo);
	button = elm_button_add(eo);
	separator = elm_separator_add(eo);
	elm_separator_horizontal_set(separator, EINA_FALSE);

	evas_object_size_hint_weight_set(e1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e1, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_scrollable_set(e1, EINA_TRUE);
	elm_entry_line_wrap_set(e1, EINA_TRUE);
	elm_box_pack_end(box1, e1);

	elm_object_part_content_set(eo, "elm.editor.name.box1", box1);
	elm_object_part_content_set(eo, "elm.editor.name.label1", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb4);
	elm_object_part_content_set(eo, "elm.editor.name.entry1", e1);
	elm_object_part_content_set(eo, "elm.separator.1", separator);
	elm_object_part_content_set(eo, "elm.editor.name.button", button);
	elm_object_text_set(button, SET_VAL);

	elm_entry_editable_set(e1, EINA_TRUE);
	elm_entry_single_line_set(e1, EINA_TRUE);
	elm_object_part_text_set(e1, "guide", ENTER_VAL);

	elm_label_slide_duration_set(lb1, 3);
	elm_label_slide_duration_set(lb2, 3);
	elm_label_slide_duration_set(lb3, 3);
	elm_label_slide_duration_set(lb4, 3);

	elm_object_style_set(lb1, "slide_bounce");
	elm_object_style_set(lb2, "slide_bounce");
	elm_object_style_set(lb3, "slide_bounce");
	elm_object_style_set(lb4, "slide_bounce");

	elm_object_part_text_set(eo, "text1", CURR_VAL);
	elm_object_part_text_set(eo, "text2", MAX_VAL);
	elm_object_part_text_set(eo, "text3", MIN_VAL);
	elm_object_part_text_set(eo, "text4", MIN_INC);
	elm_object_part_text_set(eo, "text5", SET_VAL);

	evas_object_color_set(separator, 1, 1, 1, 255);

	return eo;
}

/**
 * @brief gets character at offset position
 *
 * That callback is called when spinner value in text layout is changed
 *
 * @param data action_cb_stuct, which contains object with avaiable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_get_char_at_offset(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo, *eo1, *eo2;
	eo1 = eina_list_nth(acs->list, 0);
	eo2 = eina_list_nth(acs->list, 1);
	eo = eina_list_nth(acs->list, 4);

	char *value = NULL;
	gchar *full_text = atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1);
	if(full_text && strlen(full_text) > (int)elm_spinner_value_get(eo2))
	{
		if (asprintf(&value,"%c", atk_text_get_character_at_offset(ATK_TEXT(acs->atk_obj), (int)elm_spinner_value_get(eo2))) < 0)
			EINA_LOG_ERR(ERR_GET_CHAR_OFFSET);
	}
	g_free(full_text);

	if(!sizeof(value))
	{
		return;
	}
	elm_object_text_set(eo1, value);
	free(value);

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)) - 1);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)) - 1);
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}
}

/**
 * @brief selects text
 *
 * Callback called when button for select text in text layout is clicked
 *
 * @param data param data action_cb_stuct, which contains object with avaiable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_add_selection(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	char *selected_regions;

	if(!acs->atk_obj)
	{
		return;
	}
	Evas_Object *eo1, *eo2, *lab, *eo;
	eo1 = eina_list_nth(acs->list, 2);
	eo2 = eina_list_nth(acs->list, 3);
	lab = eina_list_nth(acs->list, 5);
	eo = eina_list_nth(acs->list, 4);

	if(!atk_text_remove_selection(ATK_TEXT(acs->atk_obj), 0))
	{
		if(!(int)elm_spinner_value_get(eo1) && !(int)elm_spinner_value_get(eo2))
		{
			elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
			return;
		}
		else if((int)elm_spinner_value_get(eo1) > (int)elm_spinner_value_get(eo2))
		{
			elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
			return;
		}
		else if((int)elm_spinner_value_get(eo1) == (int)elm_spinner_value_get(eo2))
		{
			elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
			return;
		}
		atk_text_add_selection(ATK_TEXT(acs->atk_obj), (int)elm_spinner_value_get(eo1), (int)elm_spinner_value_get(eo2));

		if(asprintf(&selected_regions, "%d", atk_text_get_n_selections(ATK_TEXT(acs->atk_obj))) < 0)
		{
			EINA_LOG_ERR(ERR_MEMORY);
			elm_object_text_set(lab, FAIL_OCC);
		}
		else
		{
			elm_object_part_text_set(obj, NULL, UNSET);
			elm_object_text_set(lab, selected_regions);
		}
	}
	else
	{
		if(asprintf(&selected_regions, "%d", atk_text_get_n_selections(ATK_TEXT(acs->atk_obj))) < 0)
		{
			EINA_LOG_ERR(ERR_MEMORY);
		}
		else
		{
			elm_object_text_set(lab, selected_regions);
			elm_object_part_text_set(obj, NULL, SET);
		}
	}

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}

	free(selected_regions);
}

/**
 * @brief prints the Text layout
 *
 * Main function to print selection layout, called when click on text
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Text Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_text(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{

	char *caret_value = NULL;
	char *counter = NULL;
	char *selected_regions = NULL;

	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	acs->list = NULL;

	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label2"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.spinner"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.spinner1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.spinner2"));
	acs->list = eina_list_append(acs->list, eo);
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.label5"));

	if(atk_text_get_text(ATK_TEXT(obj), 0, -1))
	{
		elm_object_disabled_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner"), 0, strlen(atk_text_get_text(ATK_TEXT(obj), 0, -1)) - 1);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(obj), 0, -1)));
	}
	else
	{
		elm_object_disabled_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_TRUE);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}

	elm_spinner_value_set(elm_object_part_content_get(eo, "elm.editor.name.spinner"), 0);
	elm_spinner_value_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0);
	elm_spinner_value_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0);

	elm_entry_entry_set(elm_object_part_content_get(eo, "elm.editor.name.label1"), atk_text_get_text(ATK_TEXT(obj), 0, -1));

	if (asprintf(&caret_value, "%d", atk_text_get_caret_offset(ATK_TEXT(obj))) < 0)
		EINA_LOG_ERR(ERR_GET_CHAR_OFFSET);

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label3"), caret_value);

	if (asprintf(&counter, "%d", atk_text_get_character_count(ATK_TEXT(obj))) < 0)
		EINA_LOG_ERR(ERR_CHAR_COUNT);

	elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label4"), counter);

	if (asprintf(&selected_regions, "%d", atk_text_get_n_selections(ATK_TEXT(obj))) < 0)
		EINA_LOG_ERR(ERR_N_SELECTIONS);

	if(atk_text_get_n_selections(ATK_TEXT(obj)) == -1)
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), FAIL_OCC);
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.button"), SET);
		elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
		elm_object_disabled_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_TRUE);
	}
	else if(atk_text_get_n_selections(ATK_TEXT(obj)) > 0)
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), selected_regions);
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.button"), UNSET);
		elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_TRUE);
	}
	else
	{
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.label5"), selected_regions);
		elm_object_text_set(elm_object_part_content_get(eo, "elm.editor.name.button"), SET);
		elm_check_state_set(elm_object_part_content_get(eo, "elm.editor.name.button"), EINA_FALSE);
	}


	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button"), "changed", _text_add_selection);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.spinner"), "changed", _text_get_char_at_offset);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button"), "changed", _text_add_selection, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.spinner"), "changed", _text_get_char_at_offset, acs);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
	free(caret_value);
	free(counter);
	free(selected_regions);
}
/**
 * @brief prepares Text interface view
 *
 * Function prepares text Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing text layout
 */
Evas_Object* prepare_layout_text(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);

	elm_layout_file_set(eo, EDJ_PATH_TXT, "explorer/text");

	Evas_Object *e1, *lb1, *lb2, *lb3, *lb4, *separator, *spinner, *spinner1, *spinner2, *box;
	Evas_Object *selection_check;

	box = elm_box_add(eo);
	e1 = elm_entry_add(eo);
	lb1 = elm_label_add(eo);
	lb2 = elm_label_add(eo);
	lb3 = elm_label_add(eo);
	lb4 = elm_label_add(eo);
	spinner = elm_spinner_add(eo);
	spinner1 = elm_spinner_add(eo);
	spinner2 = elm_spinner_add(eo);
	separator = elm_separator_add(eo);
	selection_check = elm_check_add(eo);

	evas_object_size_hint_weight_set(e1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e1, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_scrollable_set(e1, EINA_TRUE);
	elm_entry_line_wrap_set(e1, EINA_TRUE);
	elm_box_pack_end(box, e1);

	elm_separator_horizontal_set(separator, EINA_FALSE);
	elm_object_part_content_set(eo, "elm.editor.name.box1", box);
	elm_object_part_content_set(eo, "elm.editor.name.label1", e1);
	elm_object_part_content_set(eo, "elm.editor.name.label2", lb1);
	elm_object_part_content_set(eo, "elm.editor.name.label3", lb2);
	elm_object_part_content_set(eo, "elm.editor.name.label4", lb3);
	elm_object_part_content_set(eo, "elm.editor.name.label5", lb4);
	elm_object_part_content_set(eo, "elm.editor.name.spinner", spinner);
	elm_object_part_content_set(eo, "elm.editor.name.spinner1", spinner1);
	elm_object_part_content_set(eo, "elm.editor.name.spinner2", spinner2);
	elm_object_part_content_set(eo, "elm.editor.name.button", selection_check);
	elm_object_part_content_set(eo, "elm.editor.name.separator", separator);

	elm_entry_editable_set(e1, EINA_FALSE);
	elm_object_part_text_set(e1, "guide", ENTER_VAL);

	elm_label_slide_duration_set(lb1, 3);
	elm_label_slide_duration_set(lb2, 3);
	elm_label_slide_duration_set(lb3, 3);
	elm_label_slide_duration_set(lb4, 3);

	elm_object_style_set(lb1, "slide_bounce");
	elm_object_style_set(lb2, "slide_bounce");
	elm_object_style_set(lb3, "slide_bounce");
	elm_object_style_set(lb4, "slide_bounce");

	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_object_part_text_set(eo, "text1", TEXT_VAL);
	elm_object_part_text_set(eo, "text2", CHAR_AT_OFFSET);
	elm_object_part_text_set(eo, "text3", CARET_OFFSET);
	elm_object_part_text_set(eo, "text4", CHAR_COUNT);
	elm_object_part_text_set(eo, "text5", NUM_OF_SEL_REGIONS);
	elm_object_part_text_set(eo, "text6", OFFSET);
	elm_object_part_text_set(eo, "text7", START_OFFSET);
	elm_object_part_text_set(eo, "text8", END_OFFSET);
	elm_object_part_text_set(eo, "text9", SET_SEL);
	elm_object_part_text_set(eo, "text10", USAGE);

	return eo;
}

/**
 * @brief set text
 *
 * Called when set text button in Editable text layout is clicked.
 * Allows to set text for labels, entries and all other object which implements that interface.
 *
 * @param data action_cb_stuct, which contains object with avaiable editable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_set_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}

	Evas_Object *eo1, *eo;
	eo1 = eina_list_nth(acs->list, 0);
	eo = eina_list_nth(acs->list, 3);

	atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(acs->atk_obj), elm_entry_entry_get(eo1));

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}
}

/**
 * @brief copy text
 *
 * Called when copy text button in Editable text layout is clicked.
 * Allows to copy text in labels, entries and all other object which implements that interface.
 *
 * @param data action_cb_stuct, which contains object with avaiable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_copy_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}

	Evas_Object *spin, *spin1, *eo;
	spin = eina_list_nth(acs->list, 1);
	spin1 = eina_list_nth(acs->list, 2);
	eo = eina_list_nth(acs->list, 3);

	if(!atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
		return;

	if((int)elm_spinner_value_get(spin) > strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)))
		return;

	if((int)elm_spinner_value_get(spin) > (int)elm_spinner_value_get(spin1))
		return;

	atk_editable_text_copy_text(ATK_EDITABLE_TEXT(acs->atk_obj),(int)elm_spinner_value_get(spin), (int)elm_spinner_value_get(spin1));

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}
}

/**
 * @brief paste text
 *
 * Called when paste text button in Editable text layout is clicked.
 * Allows to paste text for entries and all other object which implements that interface.
 *
 * @param data action_cb_stuct, which contains object with avaiable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_paste_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}

	Evas_Object *spin, *eo;
	spin = eina_list_nth(acs->list, 1);
	eo = eina_list_nth(acs->list, 3);

	if(!atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		atk_editable_text_paste_text(ATK_EDITABLE_TEXT(acs->atk_obj), 0);
		elm_entry_entry_set(eina_list_nth(acs->list, 0), atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1));
		return;
	}

	if((int)elm_spinner_value_get(spin) >= strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)))
	{
		atk_editable_text_paste_text(ATK_EDITABLE_TEXT(acs->atk_obj), strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_entry_entry_set(eina_list_nth(acs->list, 0), atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1));
		return;
	}

	atk_editable_text_paste_text(ATK_EDITABLE_TEXT(acs->atk_obj),(int)elm_spinner_value_get(spin));


	elm_entry_entry_set(eina_list_nth(acs->list, 0), atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1));

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}
}

/**
 * @brief cut text
 *
 * Called when cut text button in Editable text layout is clicked.
 * Allows to cut text for entries and all other object which implements that interface.
 *
 * @param data action_cb_stuct, which contains object with avaiable text interface
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _text_cut_cb(void *data, Evas_Object *obj, void *event_info)
{
	action_cb_struct *acs = (action_cb_struct*)data;
	if(!acs->atk_obj)
	{
		return;
	}

	Evas_Object *spin, *spin1, *eo;
	spin = eina_list_nth(acs->list, 1);
	spin1 = eina_list_nth(acs->list, 2);
	eo = eina_list_nth(acs->list, 3);

	if(!atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
		return;

	if((int)elm_spinner_value_get(spin) > (int)elm_spinner_value_get(spin1))
		return;

	else if((int)elm_spinner_value_get(spin) > strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)) ||
		(int)elm_spinner_value_get(spin1) > strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)))
			return;

	atk_editable_text_cut_text(ATK_EDITABLE_TEXT(acs->atk_obj), (int)elm_spinner_value_get(spin), (int)elm_spinner_value_get(spin1));
	elm_entry_entry_set(eina_list_nth(acs->list, 0), atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1));

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}
}

/**
 * @brief prints the Editable Text layout
 *
 * Main function to print selection layout, called when click on editable text
 * label in toolbar is clicked, that function fills all entries and labels
 * in that layout, register callbacks on objects
 *
 * @param eo Editable Text Layout object
 * @param obj AtkObject which has focus
 * @param parent Main Layout object
 */
void print_edit_text(Evas_Object *eo, AtkObject *obj, Evas_Object *parent)
{

	if(!acs)
	{
		acs = (action_cb_struct*)malloc(sizeof(action_cb_struct));
	}
	else
	{
		eina_list_free(acs->list);
	}

	acs->list = NULL;

	acs->atk_obj = obj;
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.e1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.spinner1"));
	acs->list = eina_list_append(acs->list, elm_object_part_content_get(eo, "elm.editor.name.spinner2"));
	acs->list = eina_list_append(acs->list, eo);

	if(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1))
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, strlen(atk_text_get_text(ATK_TEXT(acs->atk_obj), 0, -1)));
	}
	else
	{
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner1"), 0, 0);
		elm_spinner_min_max_set(elm_object_part_content_get(eo, "elm.editor.name.spinner2"), 0, 0);
	}

	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _text_set_cb);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _text_copy_cb);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _text_paste_cb);
	evas_object_smart_callback_del(elm_object_part_content_get(eo, "elm.editor.name.button3"), "clicked", _text_cut_cb);

	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button"), "clicked", _text_set_cb, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button1"), "clicked", _text_copy_cb, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button2"), "clicked", _text_paste_cb, acs);
	evas_object_smart_callback_add(elm_object_part_content_get(eo, "elm.editor.name.button3"), "clicked", _text_cut_cb, acs);

	elm_object_part_content_set(parent, "elm.layout", eo);
	evas_object_show(eo);
}

/**
 * @brief prepares Editable Text interface view
 *
 * Function prepares Editable Text Layout by adding all
 * the used objects to layout and setting object labels
 *
 * @param parent Parent Layout object
 *
 * @return Evas_Object representing Editable Text layout
 */
Evas_Object* prepare_layout_edit_text(Evas_Object *parent)
{
	Evas_Object *eo = elm_layout_add(parent);

	elm_layout_file_set(eo, EDJ_PATH_ETXT, "explorer/editable_text");

	Evas_Object *e1, *separator, *spinner1, *spinner2, *box;
	Evas_Object *set_text_button, *copy_text_button, *paste_text_button;
	Evas_Object *cut_text_button;

	box = elm_box_add(eo);
	e1 = elm_entry_add(eo);
	spinner1 = elm_spinner_add(eo);
	spinner2 = elm_spinner_add(eo);
	separator = elm_separator_add(eo);
	set_text_button = elm_button_add(eo);
	copy_text_button = elm_button_add(eo);
	paste_text_button = elm_button_add(eo);
	cut_text_button = elm_button_add(eo);

	evas_object_size_hint_weight_set(e1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(e1, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_entry_single_line_set(e1, EINA_TRUE);
	elm_entry_scrollable_set(e1, EINA_TRUE);
	elm_entry_line_wrap_set(e1, EINA_TRUE);
	elm_box_pack_end(box, e1);

	elm_object_part_text_set(e1, "guide", ENTER_TXT);

	elm_separator_horizontal_set(separator, EINA_FALSE);
	elm_object_part_content_set(eo, "elm.editor.name.box1", box);
	elm_object_part_content_set(eo, "elm.editor.name.e1", e1);
	elm_object_part_content_set(eo, "elm.editor.name.spinner1", spinner1);
	elm_object_part_content_set(eo, "elm.editor.name.spinner2", spinner2);
	elm_object_part_content_set(eo, "elm.editor.name.button", set_text_button);
	elm_object_part_content_set(eo, "elm.editor.name.button1", copy_text_button);
	elm_object_part_content_set(eo, "elm.editor.name.button2", paste_text_button);
	elm_object_part_content_set(eo, "elm.editor.name.button3", cut_text_button);
	elm_object_part_content_set(eo, "elm.editor.name.separator", separator);

	elm_object_text_set(set_text_button, SET);
	elm_object_text_set(copy_text_button, COPY);
	elm_object_text_set(paste_text_button, PASTE);
	elm_object_text_set(cut_text_button, CUT);

	evas_object_color_set(separator, 1, 1, 1, 255);

	elm_object_part_text_set(eo, "text1", SET_TEXT);
	elm_object_part_text_set(eo, "text2", COPY);
	elm_object_part_text_set(eo, "text3", COPY_TEXT_DESCRIPTION);
	elm_object_part_text_set(eo, "text4", PASTE_TEXT);
	elm_object_part_text_set(eo, "text5", CUT_TEXT);
	elm_object_part_text_set(eo, "text6", START_POINT);
	elm_object_part_text_set(eo, "text7", END_POINT);

	return eo;
}
