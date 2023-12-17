#pragma once

namespace LangYa::SentryLib {
	template<typename TContent>
	class ContentProvider
	{
	public:
		virtual ~ContentProvider() = default;

		[[nodiscard]] virtual bool GetContent(TContent& content) = 0;
		[[nodiscard]] virtual const TContent& GetContent() const = 0;

		/// @brief 提供一种简单的方法访问其中的数据。
		/// 如果希望数据在读取时保持不变，请使用 GetContent 复制内容。
		///	如果只一次性读取少量数据，使用这个函数可以减少一次复制。
		[[nodiscard]] const TContent* operator->() const
		{
			return &GetContent();
		}
	};

	template<typename TContent>
	class ContentAcceptor
	{
	public:
		virtual ~ContentAcceptor() = default;

		virtual void SetContent(const TContent& content) = 0;
		virtual TContent& GetMutableContent() = 0;

		/// @brief 提供一种简单的方法修改其中的数据。
		TContent* operator->()
		{
			return &GetMutableContent();
		}
	};
}