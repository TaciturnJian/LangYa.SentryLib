#pragma once

namespace Sango {
	template <typename TID>
	class IHasID
	{
	public:
		using IDType = TID;
		virtual ~IHasID() = default;
		[[nodiscard]] virtual TID& GetID() = 0;
		[[nodiscard]] virtual const TID& GetID() const = 0;
	};

	template <typename TInfo>
	class IHasInfo
	{
	public:
		using InfoType = TInfo;
		virtual ~IHasInfo() = default;
		[[nodiscard]] virtual TInfo& GetInfo() = 0;
		[[nodiscard]] virtual const TInfo& GetInfo() const = 0;
	};

	template <typename TName>
	class IHasName
	{
	public:
		using NameType = TName;
		virtual ~IHasName() = default;
		[[nodiscard]] virtual TName& GetName() = 0;
		[[nodiscard]] virtual const TName& GetName() const = 0;
	};
}