#include "Camera.h"
#include <math.h>
#include"SimpleMath.h"
#include "InputCommands.h"

Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 5.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camForward.x = 0.0f;
	m_camForward.y = 0.0f;
	m_camForward.z = 0.0f;

	m_camUp.x = 0.0f;
	m_camUp.y = 0.0f;
	m_camUp.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;

	m_pitch = 0;
	m_yaw = 0;
	m_roll = 0;

	m_toggleFirstPerson = false;

	m_prevMouseX = 0;
	m_prevMouseY = 0;

	RecalcDirection();
}

Camera::~Camera()
{
}

void Camera::Update(InputCommands* input_, float delta_time_)
{
	//camera motion is on a plane, so kill the 7 component of the look direction
	//DirectX::SimpleMath::Vector3 planarMotionVector = m_camLookDirection;
	//planarMotionVector.y = 0.0;

	if (input_->mouse_RB_Down)
	{
		int mouseXDist = (m_prevMouseX - input_->mouse_X);
		int mouseYDist = (m_prevMouseY - input_->mouse_Y);

		//move camera on mouse input
		RotateYYaw(mouseXDist  * m_camRotRate, delta_time_);
		RotateXPitch(-(mouseYDist * m_camRotRate), delta_time_);
	}
	else
	{
		m_prevMouseX = 0;
		m_prevMouseY = 0;

		if (input_->rotRight)
		{
			//m_camOrientation.y -= m_camRotRate;
			RotateYYaw(-m_camRotRate, delta_time_);
		}
		if (input_->rotLeft)
		{
			//m_camOrientation.y += m_camRotRate;
			RotateYYaw(m_camRotRate, delta_time_);
		}
	}

	m_prevMouseX = input_->mouse_X;
	m_prevMouseY = input_->mouse_Y;


	//process input and update stuff
	if (input_->forward)
	{
		m_camPosition += m_camForward * m_movespeed;
		RecalcDirection();
	}
	if (input_->back)
	{
		m_camPosition -= m_camForward * m_movespeed;
		RecalcDirection();
	}
	if (input_->right)
	{
		m_camPosition += m_camRight * m_movespeed;
		RecalcDirection();
	}
	if (input_->left)
	{
		m_camPosition -= m_camRight * m_movespeed;
		RecalcDirection();
	}
}

void Camera::RecalcDirection()
{
	//create look direction from Euler angles in m_camOrientation
	//m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180);
	//m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180);
	//m_camLookDirection.Normalize();

	float cosPitch, cosYaw, cosRoll;
	float sinPitch, sinYaw, sinRoll;

	cosPitch = cos(m_pitch * 3.1415 / 180);
	sinPitch = sin(m_pitch * 3.1415 / 180);
	cosYaw = cos(m_yaw * 3.1415 / 180);
	sinYaw = sin(m_yaw * 3.1415 / 180);
	cosRoll = cos(m_roll * 3.1415 / 180);
	sinRoll = sin(m_roll * 3.1415 / 180);

	m_camForward.x = sinYaw * cosPitch;
	m_camForward.y = sinPitch;
	m_camForward.z = cosPitch * -cosYaw;

	m_camUp.x = -cosYaw * sinRoll - sinYaw * sinPitch * cosRoll;
	m_camUp.y = cosPitch * cosRoll;
	m_camUp.z = -sinYaw * sinRoll - sinPitch * cosRoll * -cosYaw;

	m_camRight = m_camForward.Cross(m_camUp);

	//create right vector from look Direction
	//m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

	//update lookat point
	m_camLookAt = m_camPosition + m_camForward;

	//apply camera vectors
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camPosition, m_camLookAt, m_camUp);
}

DirectX::SimpleMath::Matrix Camera::GetView()
{
	return m_view;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition()
{
	return m_camPosition;
}

void Camera::SetWindowWidth(int width_)
{
	m_windowWidth = width_;
}

void Camera::SetWindowHeight(int height_)
{
	m_windowHeight = height_;
}

void Camera::RotateXPitch(float rot_val_, float delta_time_)
{
	m_pitch += rot_val_ * delta_time_;
	RecalcDirection();
}

void Camera::RotateYYaw(float rot_val_, float delta_time_)
{
	m_yaw += rot_val_ * delta_time_;
	RecalcDirection();
}

void Camera::RotateZRoll(float rot_val_, float delta_time_)
{
	m_roll += rot_val_ * delta_time_;
	RecalcDirection();
}


