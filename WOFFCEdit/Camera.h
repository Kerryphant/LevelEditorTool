#pragma once
#include "DirectXHelpers.h"
#include "pch.h"

class InputCommands;

class Camera
{
public:

	Camera();
	~Camera();

	void Update(InputCommands* input_, float delta_time_);

	DirectX::SimpleMath::Matrix GetView();
	DirectX::SimpleMath::Vector3 GetPosition();

	void SetWindowWidth(int width_);
	void SetWindowHeight(int height_);

private:

	//functionality
	float								m_movespeed;
	float								m_pitch;
	float								m_yaw;
	float								m_roll;

	bool								m_toggleFirstPerson;

	//camera
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	DirectX::SimpleMath::Vector3		m_camUp;
	DirectX::SimpleMath::Vector3		m_camForward;
	float m_camRotRate;

	DirectX::SimpleMath::Matrix                                             m_view;

	int									m_windowWidth;
	int									m_windowHeight;

	int									m_prevMouseX;
	int									m_prevMouseY;

	void RotateXPitch(float rot_val_, float delta_time_);
	void RotateYYaw(float rot_val_, float delta_time_);
	void RotateZRoll(float rot_val_, float delta_time_);

	void RecalcDirection();
protected:

};