#include <glib.h>
#include <glib/gprintf.h>
#include <stdio.h>
#include "eail_clipboard.h"

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
 * @file eail_clipboard.c
 *
 * @brief EailClipboard implementation
 *
 * Widget that is being used to store clipboard data for ATK widgets
 */

/**
 * @brief Holds copied/cut text
 */
static char *clipboard;

/**
 * @brief Gets clipboard text
 * @returns clipboard text
 */
const char *eail_clipboard_get_text(void)
{
   return clipboard;
}

/**
 * @brief Sets clipboard text
 * @param text new clipboard text
 */
void eail_clipboard_set_text(const char *text)
{
   if (clipboard) g_free(clipboard);
   clipboard = g_strdup(text);
}

/**
 * @brief Frees memory allocated by clipboard
 */
void eail_clipboard_free(void)
{
   if (clipboard) g_free(clipboard);
}
