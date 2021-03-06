#pragma once
#include "Resource.h"

struct _TTF_Font;

	/**
	* Simple RAII wrapper for an _TTF_Font
	*/

namespace anemoia
{
	class Font final : public Resource
	{
	public:
#pragma region Constructors
		explicit Font(const std::string& fullPath, unsigned int size);
		virtual ~Font();

		Font(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) = delete;
#pragma endregion Constructors

		//Functions
		_TTF_Font* GetFont() const;

	private:
		_TTF_Font* m_Font;
		unsigned int m_Size;
	};
}