 #include <LangYa/SentryLib/TripleBuffer.hpp>

LangYa::SentryLib::TripleBuffer
::TripleBuffer(const LangYa::SentryLib::MemoryView::SizeType size) :
	InnerBuffer(size),
	Resources{
		ResourceType(
			ResourceFlag::Empty,
			MemoryView(
				InnerBuffer.GetView(), 
				0 * size / 3, 
				size / 3
			)
		),
		ResourceType(
			ResourceFlag::Empty,
			MemoryView(
				InnerBuffer.GetView(), 
				1 * size / 3, 
				size / 3
			)
		),
		ResourceType(
			ResourceFlag::Empty,
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
LangYa::SentryLib::TripleBuffer
::Pop(const MemoryView& destination)
{
	const char reader = ReaderIndex;
	for (auto i = 0; i < 3; ++i)
	{
		auto& resource = Resources[(i + reader) % 3];
		if (resource.Flag != ResourceFlag::Available) continue;

		resource.Flag = ResourceFlag::Busy;
		resource.Content.CopyTo(destination);
		resource.Flag = ResourceFlag::Empty;

		return true;
	}
	return false;
}

void
LangYa::SentryLib::TripleBuffer
::Push(const MemoryView& resource)
{
	if (Resources[WriterIndex].Flag == ResourceFlag::Busy)
	{
		const char index = WriterIndex;
		WriterIndex = index >= 2 ? 0 : index + 1;
	}

	auto& my_resource = Resources[WriterIndex];  // NOLINT(clang-diagnostic-char-subscripts)
	my_resource.Flag = ResourceFlag::Busy;
	my_resource.Content.ReadFrom(resource);
	my_resource.Flag = ResourceFlag::Available;
	const char index = WriterIndex;
	ReaderIndex = index;
	WriterIndex = WriterIndex >= 2 ? 0 : WriterIndex + 1;
}
