#include "ObjectsToDraw.h"

Axis::Axis()
{
	transformation = Transformation();

	xAxisColor_ = Color::red();
	yAxisColor_ = Color::green();
	zAxisColor_ = Color::blue();

	xAxisTransform_ = Transformation(Vector3(25, 0, 0), Vector3(0, 0, 0), Vector3(50, 1, 1));
	yAxisTransform_ = Transformation(Vector3(0, 25, 0), Vector3(0, 0, 90), Vector3(1, 50, 1));
	zAxisTransform_ = Transformation(Vector3(0, 0, 25), Vector3(0, 90, 0), Vector3(1, 1, 50));
}

void Axis::draw()
{

	glPushMatrix();
	{
		transformation.transform();

		glPushMatrix();
		{
			xAxisTransform_.transform();
			xAxisColor_.glColor();
			standardLine();
		}
		glPopMatrix();

		glPushMatrix();
		{
			yAxisTransform_.transform();
			yAxisColor_.glColor();
			standardLine();
		}
		glPopMatrix();

		glPushMatrix();
		{
			zAxisTransform_.transform();
			zAxisColor_.glColor();
			standardLine();
		}
		glPopMatrix();
	}
	glPopMatrix();

}

Robot::Robot()
{
	// Body Parts
	head_ = SceneObject();
	head_.transformation.position = Vector3(0, 3, 0);
	head_.transformation.scale = Vector3(2, 2, 2);

	torso_ = SceneObject();
	torso_.transformation.position = Vector3(0, 0, 0);
	torso_.transformation.scale = Vector3(2, 4, 1);
	
	leftUpperArm_ = SceneObject();
	leftUpperArm_.transformation.position = Vector3(1, 0, 0);
	leftUpperArm_.transformation.scale = Vector3(2, 0.5, 1);

	leftLowerArm_ = SceneObject();
	leftLowerArm_.transformation.position = Vector3(1, 0, 0);
	leftLowerArm_.transformation.scale = Vector3(2, 0.5, 1);

	rightUpperArm_ = SceneObject();
	rightUpperArm_.transformation.position = Vector3(-1, 0, 0);
	rightUpperArm_.transformation.scale = Vector3(2, 0.5, 1);

	rightLowerArm_ = SceneObject();
	rightLowerArm_.transformation.position = Vector3(-1, 0, 0);
	rightLowerArm_.transformation.scale = Vector3(2, 0.5, 1);

	leftUpperLeg_ = SceneObject();
	leftUpperLeg_.transformation.position = Vector3(0, -1, 0);
	leftUpperLeg_.transformation.scale = Vector3(1, 2, 0.5);

	leftLowerLeg_ = SceneObject();
	leftLowerLeg_.transformation.position = Vector3(0, -1, 0);
	leftLowerLeg_.transformation.scale = Vector3(1, 2, 0.5);

	rightUpperLeg_ = SceneObject();
	rightUpperLeg_.transformation.position = Vector3(0, -1, 0);
	rightUpperLeg_.transformation.scale = Vector3(1, 2, 0.5);

	rightLowerLeg_ = SceneObject();
	rightLowerLeg_.transformation.position = Vector3(0, -1, 0);
	rightLowerLeg_.transformation.scale = Vector3(1, 2, 0.5);

	// Joints
	leftShoulder_ = Transformation(Vector3(1, 1.75, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	rightShoulder_ = Transformation(Vector3(-1, 1.75, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	leftElbow_ = Transformation(Vector3(2, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	rightElbow_ = Transformation(Vector3(-2, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	leftHip_ = Transformation(Vector3(0.5, -2, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	rightHip_ = Transformation(Vector3(-0.5, -2, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	leftKnee_ = Transformation(Vector3(0, -2, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	rightKnee_ = Transformation(Vector3(0, -2, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));

	isSolid_ = true;

	leftShoulderVelocity_ = 0;
	rightShoulderVelocity_ = 0;
	leftElbowVelocity_ = 0;
	rightElbowVelocity_ = 0;
	leftHipVelocity_ = 0;
	rightHipVelocity_ = 0;
	leftKneeVelocity_ = 0;
	rightKneeVelocity_ = 0;
}

void Robot::draw()
{
	glPushMatrix();
	{
		transformation.transform();
		glPushMatrix();
		{
			head_.glDraw();
			torso_.glDraw();

			// Left Arm
			glPushMatrix();
			{
				leftShoulder_.transform();
				leftUpperArm_.glDraw();
				glPushMatrix();
				{
					leftElbow_.transform();
					leftLowerArm_.glDraw();
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Right Arm
			glPushMatrix();
			{
				rightShoulder_.transform();
				rightUpperArm_.glDraw();
				glPushMatrix();
				{
					rightElbow_.transform();
					rightLowerArm_.glDraw();
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Left Leg
			glPushMatrix();
			{
				leftHip_.transform();
				leftUpperLeg_.glDraw();
				glPushMatrix();
				{
					leftKnee_.transform();
					leftLowerLeg_.glDraw();
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Right Leg
			glPushMatrix();
			{
				rightHip_.transform();
				rightUpperLeg_.glDraw();
				glPushMatrix();
				{
					rightKnee_.transform();
					rightLowerLeg_.glDraw();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Robot::animateWalking()
{
	const float standardVelocity = -0.4f;

	// Left Hip
	if (leftHipVelocity_ == 0)
		leftHipVelocity_ = standardVelocity;

	if (leftHip_.rotation.x < -44.7f && leftHip_.rotation.x >= -45.3f)
		leftHipVelocity_ = -standardVelocity;

	if (leftHip_.rotation.x > 44.7f && leftHip_.rotation.x <= 45.3f)
		leftHipVelocity_ = standardVelocity;

	// Right Hip
	if (rightHipVelocity_ == 0)
		rightHipVelocity_ = -standardVelocity;

	if (rightHip_.rotation.x < -44.7f && rightHip_.rotation.x >= -45.3f)
		rightHipVelocity_ = -standardVelocity;

	if (rightHip_.rotation.x > 44.7f && rightHip_.rotation.x <= 45.3f)
		rightHipVelocity_ = standardVelocity;

	// Left Knee
	if (leftKneeVelocity_ == 0)
		leftKneeVelocity_ = standardVelocity;

	if (leftKnee_.rotation.x < 0.3f && leftKnee_.rotation.x >= -0.3f)
		leftKneeVelocity_ = -standardVelocity;

	if (leftKnee_.rotation.x > 44.7f && leftKnee_.rotation.x <= 45.3f)
		leftKneeVelocity_ = standardVelocity;

	// Right Knee
	if (rightKneeVelocity_ == 0)
		rightKneeVelocity_ = -standardVelocity;

	if (rightKnee_.rotation.x < 0.3f && rightKnee_.rotation.x >= -0.3f)
		rightKneeVelocity_ = -standardVelocity;

	if (rightKnee_.rotation.x > 44.7f && rightKnee_.rotation.x <= 45.3f)
		rightKneeVelocity_ = standardVelocity;

	// Left Shoulder
	if (leftShoulder_.rotation.z != 285)
		leftShoulder_.rotation = Vector3(0, 0, 285);

	if (leftShoulderVelocity_ == 0)
		leftShoulderVelocity_ = -standardVelocity;

	if (leftShoulder_.rotation.y < -44.7f && leftShoulder_.rotation.y >= -45.3f)
		leftShoulderVelocity_ = -standardVelocity;

	if (leftShoulder_.rotation.y > 44.7f && leftShoulder_.rotation.y <= 45.3f)
		leftShoulderVelocity_ = standardVelocity;

	// Right Shoulder
	if (rightShoulder_.rotation.z != 75)
		rightShoulder_.rotation = Vector3(0, 0, 75);

	if (rightShoulderVelocity_ == 0)
		rightShoulderVelocity_ = -standardVelocity;

	if (rightShoulder_.rotation.y < -44.7f && rightShoulder_.rotation.y >= -45.3f)
		rightShoulderVelocity_ = -standardVelocity;

	if (rightShoulder_.rotation.y > 44.7f && rightShoulder_.rotation.y <= 45.3f)
		rightShoulderVelocity_ = standardVelocity;

	// Left Elbow
	if (leftElbowVelocity_ == 0)
		leftElbowVelocity_ = -standardVelocity;

	if (leftElbow_.rotation.y < 0.3f && leftElbow_.rotation.y >= -0.3f)
		leftElbowVelocity_ = standardVelocity;

	if (leftElbow_.rotation.y < -44.7f && leftElbow_.rotation.y >= -45.3f)
		leftElbowVelocity_ = -standardVelocity;

	// Right Elbow
	if (rightElbowVelocity_ == 0)
		rightElbowVelocity_ = -standardVelocity;

	if (rightElbow_.rotation.y < 0.3f && rightElbow_.rotation.y >= -0.3f)
		rightElbowVelocity_ = -standardVelocity;

	if (rightElbow_.rotation.y > 44.7f && rightElbow_.rotation.y <= 45.3f)
		rightElbowVelocity_ = standardVelocity;

	leftHip_.rotation += Vector3(leftHipVelocity_, 0, 0);
	rightHip_.rotation += Vector3(rightHipVelocity_, 0, 0);
	leftKnee_.rotation += Vector3(leftKneeVelocity_, 0, 0);
	rightKnee_.rotation += Vector3(rightKneeVelocity_, 0, 0);
	leftShoulder_.rotation += Vector3(0, leftShoulderVelocity_, 0);
	rightShoulder_.rotation += Vector3(0, rightShoulderVelocity_, 0);
	leftElbow_.rotation += Vector3(0, leftElbowVelocity_, 0);
	rightElbow_.rotation += Vector3(0, rightElbowVelocity_, 0);
}

void Robot::makeSolid()
{
	if (isSolid_ == false)
	{
		head_.mesh = standardTextureCube;
		torso_.mesh = standardTextureCube;
		leftUpperArm_.mesh = standardTextureCube;
		leftLowerArm_.mesh = standardTextureCube;
		rightUpperArm_.mesh = standardTextureCube;
		rightLowerArm_.mesh = standardTextureCube;
		leftUpperLeg_.mesh = standardTextureCube;
		leftLowerLeg_.mesh = standardTextureCube;
		rightUpperLeg_.mesh = standardTextureCube;
		rightLowerLeg_.mesh = standardTextureCube;

		isSolid_ = true;
	}
}

void Robot::makeWireframe()
{
	if (isSolid_ == true)
	{
		head_.mesh = standardWireCube;
		torso_.mesh = standardWireCube;
		leftUpperArm_.mesh = standardWireCube;
		leftLowerArm_.mesh = standardWireCube;
		rightUpperArm_.mesh = standardWireCube;
		rightLowerArm_.mesh = standardWireCube;
		leftUpperLeg_.mesh = standardWireCube;
		leftLowerLeg_.mesh = standardWireCube;
		rightUpperLeg_.mesh = standardWireCube;
		rightLowerLeg_.mesh = standardWireCube;

		isSolid_ = false;
	}
}