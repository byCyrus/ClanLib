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

#include <memory>
#include "../Image/buffer_usage.h"

namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class GraphicContext;
class TransferBuffer;
class StorageBufferProvider;
class StorageBuffer_Impl;
class ProgramObject;

/// \brief Storage Buffer
class StorageBuffer
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	StorageBuffer();

	/// \brief Constructs a buffer
	///
	/// \param gc = Graphic Context
	/// \param size = size - use ProgramObject::get_storage_buffer_size() to calcuate this
	/// \param usage = Buffer Usage
	StorageBuffer(GraphicContext &gc, int size, int stride, BufferUsage usage = usage_dynamic_draw);

	/// \brief Constructs a buffer
	///
	/// \param gc = Graphic Context
	/// \param data = void
	/// \param size = value
	/// \param usage = Buffer Usage
	StorageBuffer(GraphicContext &gc, const void *data, int size, int stride, BufferUsage usage = usage_dynamic_draw);
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Provider
	///
	/// \return provider
	StorageBufferProvider *get_provider() const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Handle comparison operator.
	bool operator==(const StorageBuffer &other) const;

	/// \brief Uploads data to storage buffer.
	///
	/// The size specified must match the size of the buffer and is only included to help guard against buffer overruns.
	void upload_data(GraphicContext &gc, const void *data, int size);

	/// \brief Copies data from transfer buffer
	void copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos = 0, int src_pos = 0, int size = -1);

	/// \brief Copies data to transfer buffer
	void copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos = 0, int src_pos = 0, int size = -1);
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<StorageBuffer_Impl> impl;
/// \}
};

}

/// \}
