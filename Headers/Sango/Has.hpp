#pragma once

#include <Sango/IHas.hpp>

namespace Sango {
	template <typename TID>
	struct InfoWithID : IHasID<TID>
	{
		using InterfaceType = IHasID<TID>;
		using IDType = typename InterfaceType::IDType;

	protected:
		IDType ID{};

	public:
		InfoWithID() = default;

		explicit InfoWithID(IDType id) : ID(std::move(id))
		{
		}

		[[nodiscard]] const IDType& GetID() const override
		{
			return ID;
		}

		[[nodiscard]] IDType& GetID() override
		{
			return ID;
		}
	};

	template <typename TName>
	struct InfoWithName : IHasName<TName>
	{
		using InterfaceType = IHasName<TName>;
		using NameType = typename InterfaceType::NameType;

	protected:
		NameType Name{};

	public:
		InfoWithName() = default;

		explicit InfoWithName(NameType name) : Name(std::move(name))
		{
		}

		[[nodiscard]] const NameType& GetName() const override
		{
			return Name;
		}

		[[nodiscard]] NameType& GetName() override
		{
			return Name;
		}
	};

	template <typename TInfo>
	struct WithInfo : IHasInfo<TInfo>
	{
		using InterfaceType = IHasInfo<TInfo>;
		using InfoType = typename InterfaceType::InfoType;

	protected:
		InfoType Info{};

	public:
		WithInfo() = default;

		explicit WithInfo(InfoType info) : Info(std::move(info))
		{
		}

		[[nodiscard]] const InfoType& GetInfo() const override
		{
			return Info;
		}

		[[nodiscard]] InfoType& GetInfo() override
		{
			return Info;
		}
	};
}