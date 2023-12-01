#include <LangYa/SentryLib/TripleBuffer.hpp>

namespace LangYa::SentryLib
{
	TripleBuffer
	::TripleBuffer(const MemoryView::SizeType size) :
		InnerBuffer(size),
		Resources{	// 填充所有资源的视图，令其标志为 Empty
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

	TripleBuffer
	::TripleBuffer(UniqueBuffer&& uniqueBuffer) :
		InnerBuffer(std::move(uniqueBuffer))
	{
		const auto size = InnerBuffer.GetView().Size / 3;
		for (auto i = 0; i < 3; i++)
		{
			Resources[i].Content = MemoryView(
				InnerBuffer.GetView(),
				i * size,
				size
			);
		}
	}


	bool
	TripleBuffer
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
	TripleBuffer
	::Push(const MemoryView& resource)
	{
		if (Resources[WriterIndex].Flag == ResourceFlag::Busy) // NOLINT(clang-diagnostic-char-subscripts)
		{
			const char index = WriterIndex;
			WriterIndex = index >= 2 ? 0 : index + 1; // NOLINT(bugprone-narrowing-conversions)
		}

		auto& my_resource = Resources[WriterIndex]; // NOLINT(clang-diagnostic-char-subscripts)
		my_resource.Flag = ResourceFlag::Busy;
		my_resource.Content.ReadFrom(resource);
		my_resource.Flag = ResourceFlag::Available;
		const char index = WriterIndex;
		ReaderIndex = index;
		WriterIndex = WriterIndex >= 2 ? 0 : WriterIndex + 1; // NOLINT(bugprone-narrowing-conversions)
	}
}
