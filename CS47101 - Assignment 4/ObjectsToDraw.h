#pragma once

#include "SceneObject.h"

class Axis
{
	public:
		Transformation transformation;

		Axis();

		void draw();

	private:
		
		Color xAxisColor_;
		Color yAxisColor_;
		Color zAxisColor_;

		Transformation xAxisTransform_;
		Transformation yAxisTransform_;
		Transformation zAxisTransform_;
};

class Robot
{
	public:
		Transformation transformation;

		Robot();

		void draw();

		void makeSolid();
		void makeWireframe();

		void animateWalking();

	private:
		SceneObject head_;
		SceneObject torso_;
		SceneObject leftUpperArm_;
		SceneObject leftLowerArm_;
		SceneObject rightUpperArm_;
		SceneObject rightLowerArm_;
		SceneObject leftUpperLeg_;
		SceneObject leftLowerLeg_;
		SceneObject rightUpperLeg_;
		SceneObject rightLowerLeg_;

		Transformation leftShoulder_;
		Transformation rightShoulder_;
		Transformation leftElbow_;
		Transformation rightElbow_;
		Transformation leftHip_;
		Transformation rightHip_;
		Transformation leftKnee_;
		Transformation rightKnee_;

		float leftShoulderVelocity_;
		float rightShoulderVelocity_;
		float leftElbowVelocity_;
		float rightElbowVelocity_;
		float leftHipVelocity_;
		float rightHipVelocity_;
		float leftKneeVelocity_;
		float rightKneeVelocity_;

		bool isSolid_;
};