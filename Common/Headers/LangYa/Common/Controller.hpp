#pragma once

#include <spdlog/spdlog.h>

#include <LangYa/Common/IController.hpp>
#include <LangYa/Common/UniqueBuffer.hpp>

namespace LangYa::Common
{
	template <typename TSerializableData>
	class Controller final : public IController<TSerializableData>
	{
		TSerializableData DynamicData{};
		UniqueBuffer SerializationBuffer{};
		MemoryView BufferView{};
		std::shared_ptr<IOConnection> Connection{nullptr};

	public:
		explicit Controller(std::shared_ptr<IOConnection> connection) :
			SerializationBuffer(DynamicData.GetSerializationResultSize()),
			BufferView(SerializationBuffer.GetView()),
			Connection(std::move(connection))
		{
		}

		TSerializableData& GetControllerData() override
		{
			return DynamicData;
		}


		const TSerializableData& GetControllerData() const override
		{
			return DynamicData;
		}

		bool SendData() override
		{
			if (!DynamicData.Serialize(BufferView))
			{
				spdlog::warn("Controller> Failed to deserialize data, this send will be cancelled");
				return false;
			}

			if (const auto bytes = Connection->Write(BufferView); bytes != BufferView.Size)
			{
				spdlog::warn(
					"Controller> Failed to send full serialized data({}/{})",
					bytes,
					BufferView.Size
				);
				return false;
			}

			return true;
		}
	};
}
