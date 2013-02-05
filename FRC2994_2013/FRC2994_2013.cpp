#include "WPILib.h"
#include "BasicDefines.h"
#include "EGamepad.h"
#include "EAnalogTrigger.h"
#include "EDigitalInput.h"
#include "Ejoystick.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; 
	EJoystick stick2;
	EGamepad gamepad; //gamepad
	Talon collectorMotor;
	DoubleSolenoid shifter;
	Compressor compressor;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		stick2(2),
		gamepad(3),
		collectorMotor(5),
		shifter(7,8),
		compressor(5, 5)
	{
		myRobot.SetExpiration(0.1);
		shifter.Set(DoubleSolenoid::kReverse);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}
	
	void HandleDriverInputsManual(void)
	{
		myRobot.ArcadeDrive(stick);
		if(kEventClosed == stick2.GetEvent(BUTTON_SHIFT))
		{
			// Shift into high gear.
			shifter.Set(DoubleSolenoid::kForward);
		}
		else if(kEventOpened == stick2.GetEvent(BUTTON_SHIFT))
		{
			// Shift into low gear.
			shifter.Set(DoubleSolenoid::kReverse);
		}
	}
	// Shifts the super shifters into high gear if using manual transmission.

	void HandleDriverInputsAutomatic(void)
	{
		myRobot.ArcadeDrive(stick);
		if((DoubleSolenoid::kReverse) == shifter.Get())
		{
			if(stick.GetY() < -0.25)
			{
				shifter.Set(DoubleSolenoid::kForward);
			}
		}
		// If the robot is in low gear and is over 0.2 input,
		// then switch into high gear.
		else if(stick.GetY() > -0.2)
		{
			shifter.Set(DoubleSolenoid::kReverse);
		}
		// If the robot is in high gear and under 0.15 input,
		// switch the robot into low gear.
		
		//if (DoubleSolenoid::kReverse == shifter.Get())
		//{
		//	stick.GetY() * 5;
		//}
		// If robot is in low gear multiply the input by 5
		// to get full access of the low gear.
	}
	
	void HandlerDriverRPMAutomatic(void)
	{
		
	}
	
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		
		gamepad.EnableButton(BUTTON_FWD);
		gamepad.EnableButton(BUTTON_REV);
		gamepad.EnableButton(BUTTON_STOP);
		gamepad.Update();
		
		stick2.EnableButton(BUTTON_SHIFT);
		stick2.Update();
		
		compressor.Start ();
		
		while (IsOperatorControl())
		{
			gamepad.Update();
			stick2.Update();
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			HandleCollectorInputs();
			HandleDriverInputsAutomatic();
			Wait(0.005);				// wait for a motor update time
		}
	}
	void HandleCollectorInputs ()
	{
		bool m_shooterMotorRunning = false;
		
		if (false == m_shooterMotorRunning)
		{
			if (kEventClosed == gamepad.GetEvent(BUTTON_FWD))
			{
					collectorMotor.Set(1.0);
			}
			else if (kEventClosed == gamepad.GetEvent(BUTTON_REV))
			{
				collectorMotor.Set(-1.0);
			}
			else if (kEventClosed == gamepad.GetEvent(BUTTON_STOP))
			{
				collectorMotor.Set(0.0);
			}
		}
	}
	/**
	 * Runs during test mode
	 */
	void Test() 
	{
	}
};

START_ROBOT_CLASS(RobotDemo);

