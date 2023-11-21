 #include <SangoC/Memory/Buffers/TripleBuffer.hpp>

 SangoC::Memory::Buffers::TripleBuffer
::TripleBuffer(const MemoryView::SizeType size) :
	InnerBuffer(size),
	Resources{
		ResourceType(
			Resources::ResourceFlag::Empty,
			MemoryView(
				InnerBuffer.GetView(), 
				0 * size / 3, 
				size / 3
			)
		),
		ResourceType(
			Resources::ResourceFlag::Empty,
			MemoryView(
				InnerBuffer.GetView(), 
				1 * size / 3, 
				size / 3
			)
		),
		ResourceType(
			Resources::ResourceFlag::Empty,
			MemoryView(
				InnerBuffer.GetView(), 
				2 * size / 3, 
				size / 3
			)
		),
	}
{
}


bool
SangoC::Memory::Buffers::TripleBuffer
::Pop(const MemoryView& destination)
{
	const char reader = ReaderIndex;
	for (auto i = 0; i < 3; ++i)
	{
		auto& resource = Resources[(i + reader) % 3];
		if (resource.Flag != Resources::ResourceFlag::Available) continue;

		resource.Lock();
		auto bytes = resource.Content.CopyTo(destination);
		resource.Unlock();
		return true;
	}
	return false;
}

void
SangoC::Memory::Buffers::TripleBuffer
::Push(const MemoryView& resource)
{
	if (Resources[WriterIndex].Flag == Resources::ResourceFlag::Busy)
	{
		const char index = WriterIndex;
		WriterIndex = index >= 2 ? 0 : index + 1;
	}

	auto& my_resource = Resources[WriterIndex];
	my_resource.Lock();
	my_resource.Content.ReadFrom(resource);
	my_resource.Unlock(Resources::ResourceFlag::Available);
	const char index = WriterIndex;
	ReaderIndex = index;
	WriterIndex = WriterIndex >= 2 ? 0 : WriterIndex + 1;
}
