//*************
// Golden Gears Robotics
// 1/11/15
// Robot MAIN
//*************

// Tyler Robbins - 1-11-15 - First Commit of Robot class. Wheels, compressor, and solenoid.
// Conlon Meek - 1-14-15 - Added individual Talon objects to pass to the RobotDrive classes.
// Tyler Robbins - 1-22-15 - Removed useless include statements. Added an Elevator talon. Added an encoder. Added Autonomous code. Added a method to check if the user is a goat. Added control methods for the elevator talon.
// Tyler Robbins - 1-23-15 - Added elevator methods and variables. Replaced SystemCheck contents with SmartDashboard calls. Fixed swugbruh.
// Tyler Robbins - 1-25-15 - Added a Hardware object. Replaced all method calls to hardware related methods with references to the hardware object. Added more SystemCheck calls.
// Tyler Robbins - 1-26-15 - Removed variables and methods that were transferred to Hardware. Changed all objects to pointers. Added new printf statement to display the height of the elevator. Removed commented out code. Added printf statements. Removed unnecessary include statement.
// Tyler Robbins - 1-29-15 - Added code to test, allowing individual buttons to call individual talons. Commented out possibly broken code. Changed autodrive instantiation. Added four new buttons. Removed unnecessary includes.

#define Autobots_Roll_Out TeleopInit

#include "WPILib.h"

#include "Camera.cpp"
//#include "HardwareMap.cpp"
#include "Hardware.cpp"
#include "AutonomousDrive.cpp"

class Robot: public IterativeRobot
{
	Joystick *stick; // only joystick
	LiveWindow *lw;
	Solenoid *gearShift;
	JoystickButton *solButt;
	JoystickButton *elevatorUpButt;
	JoystickButton *elevatorDownButt;
	JoystickButton *elevatorGoToButt;

	JoystickButton *testButtonFrontLeft;
	JoystickButton *testButtonFrontRight;
	JoystickButton *testButtonBackLeft;
	JoystickButton *testButtonBackRight;
//	RobotDrive* drive;

	AutonomousDrive *autodrive;
	Hardware *hw;

public:
	Compressor *c = new Compressor(0);
	const float ELEVATOR_DEADBAND = 100;
	Robot() :
		lw(NULL)
	{
		// cam = new Camera("10.44.13.10");
		hw = new Hardware();
//		drive = new RobotDrive(hw->GetFrontLeft(),hw->GetBackLeft(),hw->GetFrontRight(),hw->GetBackRight());
//		Talon* frontLeft = new Talon(0);
//		Talon* frontRight = new Talon(2);
//		Talon* backLeft = new Talon(1);
//		TalonSRX* backRight = new TalonSRX(3);
//		drive = new RobotDrive(frontLeft,backLeft,frontRight,backRight);
//		autodrive = new AutonomousDrive(drive,15.0,(float)-10); // 2 seconds, -1 power
		autodrive = new AutonomousDrive(hw->GetDrive(),15.0,(float)-10); // 2 seconds, -1 power
		stick = new Joystick(0);
		gearShift = new Solenoid(0);
		solButt = new JoystickButton(stick,2);
		elevatorUpButt = new JoystickButton(stick,5);
		elevatorDownButt = new JoystickButton(stick,3);
		elevatorGoToButt = new JoystickButton(stick,1);

		testButtonFrontLeft = new JoystickButton(stick,7);
		testButtonFrontRight = new JoystickButton(stick,8);
		testButtonBackLeft = new JoystickButton(stick,9);
		testButtonBackRight = new JoystickButton(stick,10);

//		drive->SetExpiration(0.1);
		c->Start();

//		drive->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor,true);
//		drive->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor,true);
		// drive->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor,true);
		// drive->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor,true);
	}

private:

	static Robot& instance() { return (Robot&) RobotBase::getInstance(); }

	void RobotInit()
	{
		printf("RobotInit()");
		lw = LiveWindow::GetInstance();
		if(IsUserAGoat()){
			printf("The user is a goat.\nTotes ma Goats!");
		}
		SystemCheck();
	}

	void AutonomousInit()
	{
		printf("AutonomousInit()");
		printf("Starting autonomous mode at %f power for %f seconds",
		 		autodrive->getPower(),autodrive->getSeconds());
//		autodrive->Initialize();
		autodrive->Start();
		SystemCheck();
	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance()->Run();
		SystemCheck();
	}

	void Autobots_Roll_Out()
	{
		printf("Autobots_Roll_Out()");
		printf((c->Enabled()?"Compressor On":"Compressor Off"));
		printf("\n");
//		printf("Potentiometer%f\n",hw->getElevatorHeight());
		SystemCheck();
	}

	void TeleopPeriodic()
	{
//		hw->move(stick);
		float x = stick->GetX();
		float y = stick->GetY();
		float z = stick->GetZ();
		float turn = stick->GetTwist();
		float throttle = stick->GetThrottle();

		throttle = (-throttle + 1.0)/2.0; // Shift the throttle value from -1-1 to 0-1

		x*=throttle;
		y*=throttle;
		z*=throttle;
		turn*=throttle;
		hw->GetDrive()->MecanumDrive_Cartesian(x,y,turn);
//		move();
//		hw->setSolenoid(gearShift,solButt->Get());

		SystemCheck();
//		hw->ElevatorPeriodic(elevatorUpButt->Get(),
//							 elevatorDownButt->Get(),
//							 elevatorGoToButt->Get());
	}

	void TestPeriodic()
	{
		SystemCheck();
		lw->Run();

		if(testButtonFrontLeft->Get())
			hw->GetFrontLeft()->Set(-0.5);
		else
			hw->GetFrontLeft()->Set(0);
		if(testButtonFrontRight->Get())
			hw->GetFrontRight()->Set(.5);
		else
			hw->GetFrontRight()->Set(0);
		if(testButtonBackLeft->Get())
			hw->GetBackLeft()->Set(-0.5);
		else
			hw->GetBackLeft()->Set(0);
		if(testButtonBackRight->Get())
			hw->GetBackRight()->Set(.5);
		else
			hw->GetBackRight()->Set(0);
	}

	void DisabledInit(){
		printf("DisabledInit()");
		SystemCheck();
	}

	void DisabledPeriodic(){
		SystemCheck();
	}

	void SystemCheck(){
//		SmartDashboard::PutNumber("Encoder Distance",hw->GetEncoder()->GetDistance());
//		SmartDashboard::PutNumber("Encoder Rate",hw->GetEncoder()->GetRate());
		SmartDashboard::PutNumber("Power Level",PowerDistributionPanel().GetTotalPower());
		SmartDashboard::PutNumber("Temperature Level",PowerDistributionPanel().GetTemperature());
		SmartDashboard::PutBoolean("Compressor",c->Enabled());
	}

	bool IsUserAGoat(){
		return true;
	}

//	void move(){
//		float x = stick->GetX();
//		float y = stick->GetY();
//		float z = stick->GetZ();
//		float turn = stick->GetTwist();
//		float throttle = stick->GetThrottle();
//
//		throttle = (-throttle + 1.0)/2.0; // Shift the throttle value from -1-1 to 0-1
//
//		x*=throttle;
//		y*=throttle;
//		z*=throttle;
//		turn*=throttle;
//
//		drive->MecanumDrive_Cartesian(x,y,turn,0.0);
//
//	}

};

START_ROBOT_CLASS(Robot);
