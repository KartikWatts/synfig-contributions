/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/task.cpp
**	\brief Task
**
**	$Id$
**
**	\legal
**	......... ... 2015 Ivan Mahonin
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#include <synfig/rendering/task.h>

#endif

using namespace std;
using namespace synfig;
using namespace etl;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

bool
rendering::Task::draw(
	const Renderer::Handle &renderer,
	const Renderer::Params &params,
	const Surface::Handle &surface ) const
{
	if (!renderer) return false; // TODO: warning
	if (!surface) return false; // TODO: warning

	bool success = true;
	if (!renderer->draw(params, surface, get_transformation(), get_blending(), get_primitive()))
		success = false;
	if (get_branch() && !get_branch()->draw(renderer, params, surface))
		success = false;
	if (get_next() && !get_next()->draw(renderer, params, surface))
		success = false;

	return success;
}


/* === E N T R Y P O I N T ================================================= */