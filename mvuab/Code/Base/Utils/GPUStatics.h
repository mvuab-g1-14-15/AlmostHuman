#ifndef _GPUSTATICS_H
#define _GPUSTATICS_H

#include "SingletonPattern.h"

class CGPUStatics : public CSingleton<CGPUStatics>
{
	private:
		unsigned int m_VertexCount;
		unsigned int m_FacesCount;
		unsigned int m_DrawCount;

        float m_Time;

	public:
		CGPUStatics()
		{
			m_VertexCount = 0;
			m_FacesCount = 0;
			m_DrawCount = 0;
            m_Time = 0.0f;
		}

		~CGPUStatics()
		{
		}

		void SetToZero()
		{
			m_VertexCount = 0;
			m_FacesCount = 0;
			m_DrawCount = 0;
            m_Time = 0.0f;
		}

		unsigned int GetVertexCount() { return m_VertexCount; }
		unsigned int GetFacesCount() { return m_FacesCount; }
		unsigned int GetDrawCount() { return m_DrawCount; }
        float GetTime(void) { return m_Time; }

		void AddVertexCount(unsigned int v) {  m_VertexCount += v; }
		void AddFacesCount(unsigned int f) {  m_FacesCount += f; }
		void AddDrawCount(unsigned int d) {  m_DrawCount += d; }
        void AddTime(float t) { m_Time += t; }
};

#endif