// SPDX-FileCopyrightText: 2005 G-Truc Creation
//
// SPDX-License-Identifier: MIT

/// @ref gtx_perpendicular

namespace glm
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType perp(genType const& x, genType const& Normal)
	{
		return x - proj(x, Normal);
	}
}//namespace glm
