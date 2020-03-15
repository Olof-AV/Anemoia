#pragma once

namespace anemoia
{
	class Resource
	{
	public:
#pragma region Constructors
		Resource() = default;
		virtual ~Resource() = default;

		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator= (const Resource&) = delete;
		Resource& operator= (const Resource&&) = delete;
#pragma endregion Constructors
	};
}

