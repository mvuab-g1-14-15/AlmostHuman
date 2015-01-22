#ifndef _GPUSTATICS_H
#define _GPUSTATICS_H

#include "SingletonPattern.h"

class CGPUStatics : public CSingleton<CGPUStatics>
{
	private:
		unsigned int m_VertexCount;
		unsigned int m_FacesCount;
		unsigned int m_DrawCount;

	public:
		CGPUStatics()
		{
			m_VertexCount = 0;
			m_FacesCount = 0;
			m_DrawCount = 0;
		}

		~CGPUStatics()
		{
		}

		void SetToZero()
		{
			m_VertexCount = 0;
			m_FacesCount = 0;
			m_DrawCount = 0;
		}

		unsigned int GetVertexCount() { return m_VertexCount; }
		unsigned int GetFacesCount() { return m_FacesCount; }
		unsigned int GetDrawCount() { return m_DrawCount; }

		void AddVertexCount(unsigned int v) {  m_VertexCount += v; }
		void AddFacesCount(unsigned int f) {  m_FacesCount += f; }
		void AddDrawCount(unsigned int d) {  m_DrawCount += d; }
};

#endif