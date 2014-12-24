#include "CinematicPlayer.h"

void CCinematicPlayer::Update(float ElapsedTime)
{
	if (m_Playing)
		m_CurrentTime += ElapsedTime;
}

void CCinematicPlayer::Stop()
{
	m_Playing = false;
	m_CurrentTime = 0.0f;
}

void CCinematicPlayer::Play(bool Cycle)
{
	m_Playing = true;
	m_Cycle = Cycle;
}

void CCinematicPlayer::Pause()
{
	m_Playing = false;
}

void CCinematicPlayer::OnRestartCycle()
{
	m_CurrentTime = 0.0f;
}