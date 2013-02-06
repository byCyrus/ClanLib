/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#include "GL/precomp.h"
#include "gl3_buffer_object_provider.h"
#include "gl3_graphic_context_provider.h"
#include "gl3_transfer_buffer_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GL3BufferObjectProvider Construction:

GL3BufferObjectProvider::GL3BufferObjectProvider()
: handle(0), data_ptr(0)
{
	SharedGCData::add_disposable(this);
	OpenGL::set_active();

	glGenBuffers(1, &handle);
}

GL3BufferObjectProvider::~GL3BufferObjectProvider()
{
	dispose();
	SharedGCData::remove_disposable(this);
}

void GL3BufferObjectProvider::on_dispose()
{
	if (handle)
	{
		if (OpenGL::set_active())
		{
			glDeleteBuffers(1, &handle);
		}
	}
}

void GL3BufferObjectProvider::create(const void *data, int size, BufferUsage usage, GLenum new_binding, GLenum new_target)
{
	throw_if_disposed();

	binding = new_binding;
	target = new_target;

	OpenGL::set_active();

	GLint last_buffer = 0;
	if (binding)
		glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glBufferData(target, size, data, to_enum(usage));
	glBindBuffer(target, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// GL3BufferObjectProvider Attributes:

void *GL3BufferObjectProvider::get_data()
{
	if (data_ptr == NULL)
		throw Exception("PixelBuffer was not locked");
	return data_ptr;
}

/////////////////////////////////////////////////////////////////////////////
// GL3BufferObjectProvider Operations:

void GL3BufferObjectProvider::lock(GraphicContext &gc, BufferAccess access)
{
	throw_if_disposed();
	lock_gc = gc;
	OpenGL::set_active(lock_gc);
	GLint last_buffer = 0;
	if (binding)
		glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	data_ptr = (void *) glMapBuffer(target, to_enum(access));
	glBindBuffer(target, last_buffer);
}

void GL3BufferObjectProvider::unlock()
{
	throw_if_disposed();
	OpenGL::set_active(lock_gc);
	GLint last_buffer = 0;
	if (binding)
		glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glUnmapBuffer(target);
	glBindBuffer(target, last_buffer);
	data_ptr = 0;
	lock_gc = GraphicContext();
}

void GL3BufferObjectProvider::upload_data(GraphicContext &gc, int offset, const void *data, int size)
{
	throw_if_disposed();
	OpenGL::set_active(gc);
	GLint last_buffer = 0;
	if (binding)
		glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, last_buffer);
}

void GL3BufferObjectProvider::upload_data(GraphicContext &gc, const void *data, int size)
{
	upload_data(gc, 0, data, size);
}

void GL3BufferObjectProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
	throw_if_disposed();
	OpenGL::set_active(gc);

	glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
	glBindBuffer(GL_COPY_READ_BUFFER, static_cast<GL3TransferBufferProvider*>(buffer.get_provider())->get_handle());

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, src_pos, dest_pos, size);

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
}

void GL3BufferObjectProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
	throw_if_disposed();
	OpenGL::set_active(gc);

	glBindBuffer(GL_COPY_WRITE_BUFFER, static_cast<GL3TransferBufferProvider*>(buffer.get_provider())->get_handle());
	glBindBuffer(GL_COPY_READ_BUFFER, handle);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, dest_pos, src_pos, size);

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
}

/////////////////////////////////////////////////////////////////////////////
// GL3BufferObjectProvider Implementation:

GLenum GL3BufferObjectProvider::to_enum(BufferUsage usage) const
{
	switch (usage)
	{
	case usage_stream_draw:
		return GL_STREAM_DRAW;
	case usage_stream_read:
		return GL_STREAM_READ;
	case usage_stream_copy:
		return GL_STREAM_COPY;
	case usage_static_draw:
		return GL_STATIC_DRAW;
	case usage_static_read:
		return GL_STATIC_READ;
	case usage_static_copy:
		return GL_STATIC_COPY;
	case usage_dynamic_draw:
		return GL_DYNAMIC_DRAW;
	case usage_dynamic_read:
		return GL_DYNAMIC_READ;
	case usage_dynamic_copy:
		return GL_DYNAMIC_COPY;
	default:
		return GL_STATIC_DRAW;
	}
}

GLenum GL3BufferObjectProvider::to_enum(BufferAccess access) const
{
	switch (access)
	{
	case access_read_only:
		return GL_READ_ONLY;
	case access_write_only:
	case access_write_discard:
		return GL_WRITE_ONLY;
	case access_read_write:
		return GL_READ_WRITE;
	default:
		return GL_READ_WRITE;
	}
};

}