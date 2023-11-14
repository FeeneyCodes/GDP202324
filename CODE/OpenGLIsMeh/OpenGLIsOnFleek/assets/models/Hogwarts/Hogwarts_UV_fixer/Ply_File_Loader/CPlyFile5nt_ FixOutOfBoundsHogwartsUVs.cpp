#include "CPlyFile5nt.h"

	// Added to fix the Hogwarts UV issue
	// Clamps them to -1.0f to 1.0f
void CPlyFile5nt::FixOutOfBoundsHogwartsUVs(void)
{
	for ( unsigned int index = 0; index != this->m_PlyHeaderInfo.numberOfVertices; index++ )
	{
		{
			int iU0 = (int)this->m_verticies[index].tex0u;
			this->m_verticies[index].tex0u -= (float)iU0;
		}

		{
			int iV0 = (int)this->m_verticies[index].tex0v;
			this->m_verticies[index].tex0v -= (float)iV0;
		}

		{
			int iU1 = (int)this->m_verticies[index].tex1u;
			this->m_verticies[index].tex1u -= (float)iU1;
		}

		{
			int iV1 = (int)this->m_verticies[index].tex1v;
			this->m_verticies[index].tex1v -= (float)iV1;
		}

	}//for ( unsigned int index...

	return;
}

void CPlyFile5nt::SetUniformColour(CVector3f RGB)
{
	for (unsigned int index = 0; index != this->m_PlyHeaderInfo.numberOfVertices; index++)
	{
		this->m_verticies[index].red = RGB.x;
		this->m_verticies[index].green = RGB.y;
		this->m_verticies[index].blue = RGB.z;
		this->m_verticies[index].alpha = 1.0f;
	}
	return;
}


