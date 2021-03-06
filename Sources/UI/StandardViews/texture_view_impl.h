/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once

#include "API/Display/Window/display_window.h"
#include "API/Display/Render/blend_state.h"

namespace clan
{
	class KeyEvent;
	class PointerEvent;

	class TextureView_Impl
	{
	public:
		TextureView_Impl(TextureView *view, Canvas &canvas);

		void set_event_window(const DisplayWindow &event_window, const Mat4f &transform_mouse_matrix);

		void update();

		TextureView *window_view;
		Canvas canvas;

		bool needs_render = false;
		Rectf canvas_rect;
		DisplayWindow cursor_window;
		DisplayWindow event_window;
		SlotContainer slots;
		Mat4f transform_mouse_matrix;

		BlendState opaque_blend;

		void on_window_close();
		void on_lost_focus();
		void on_got_focus();
		void on_key_down(const clan::InputEvent &);
		void on_key_up(const clan::InputEvent &);
		void on_mouse_down(const clan::InputEvent &);
		void on_mouse_dblclk(const clan::InputEvent &);
		void on_mouse_up(const clan::InputEvent &);
		void on_mouse_move(const clan::InputEvent &);

	private:
		PointerButton decode_id(clan::InputCode ic) const;
		void window_key_event(KeyEvent &e);
		void window_pointer_event(PointerEvent &e);
		void transform_on_key_down(const clan::InputEvent &);
		void transform_on_key_up(const clan::InputEvent &);
		void transform_on_mouse_down(const clan::InputEvent &);
		void transform_on_mouse_dblclk(const clan::InputEvent &);
		void transform_on_mouse_up(const clan::InputEvent &);
		void transform_on_mouse_move(const clan::InputEvent &);

		clan::InputEvent transform_input_event(const clan::InputEvent &event) { InputEvent e = event; e.mouse_pos = Vec2f(transform_mouse_matrix.get_transformed_point(Vec3f(e.mouse_pos.x, e.mouse_pos.y, 0))); return e; }

		int capture_down_counter = 0;
		std::shared_ptr<View> captured_view;
		std::shared_ptr<View> hot_view;

	};
}
