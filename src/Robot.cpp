//*************
// Golden Gears Robotics
// 1/11/15
// Robot MAIN
//*************

// Tyler Robbins - 1-11-15 - First Commit of Robot class. Wheels, compressor, and solenoid.
// Conlon Meek - 1-14-15 - Added individual Talon objects to pass to the RobotDrive classes.
// Tyler Robbins - 1-22-15 - Removed useless include statements. Added an Elevator talon. Added an encoder. Added Autonomous code. Added a method to check if the user is a goat. Added control methods for the elevator talon.

#include "WPILib.h"

#include "Camera.cpp"
#include "HardwareMap.cpp"
#include "AutonomousDrive.cpp"

class Robot: public IterativeRobot
{
	Talon frontLeft;
	Talon frontRight;
	Talon backLeft;
	Talon backRight;
	Talon *elevatorMotor;
	RobotDrive *drive; // robot drive system
	//RobotDrive driveFront;
	//RobotDrive driveMiddle;
	//RobotDrive driveBack;
	Joystick stick; // only joystick
	LiveWindow *lw;
	Solenoid gearShift;
	JoystickButton solButt;
	// Camera *cam;
	Encoder *enc;
	AutonomousDrive *autodrive;
	// double last_encoder_value = 0;
	//Compressor *c;
	//int autoLoopCounter;

public:
	Compressor *c = new Compressor(0);
	Robot() :
		frontLeft(0),
		frontRight(2),
		//midLeft(4),
		//midRight(1),
		backLeft(1),
		backRight(3),
		//elevatorMotor(9),
		//driveFront(frontRight,frontLeft),
		//driveMiddle(midRight,midLeft),
		//driveBack(backRight,backLeft),
		//drive(frontLeft, backLeft, frontRight, backRight),
		stick(0),		// as they are declared above.
		lw(NULL),
		gearShift(0),
		solButt(&stick,2)
	{
		drive = new RobotDrive(frontLeft,backLeft,frontRight,backRight);
		// cam = new Camera("10.44.13.10");
		enc = new Encoder(8,9); // Needs real ports
		elevatorMotor = new Talon(9);
		autodrive = new AutonomousDrive(drive,15.0,(float)-1.0); // 2 seconds, -1 power

		drive->SetExpiration(0.1);
		c->Start();

		drive->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor,true);
		drive->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor,true);
		// drive->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor,true);
		// drive->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor,true);
	}

private:

	static Robot& instance() { return (Robot&) RobotBase::getInstance(); }

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		//c->Start();
		if(IsUserAGoat()){
			printf("The user is a goat.\nTotes ma Goats!");
		}
	}

	void AutonomousInit()
	{
		// autoLoopCounter = 0;
		//c->Stop();
		printf("Starting autonomous mode at %f power for %f seconds",
		 		autodrive->getPower(),autodrive->getSeconds());
		autodrive->Start();
	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance()->Run();
	}

	void TeleopInit()
	{
		//printf("Hello World 4413!");
		//c->Stop();
		//driveFront.SetInvertedMotor(RobotDrive::kFrontLeftMotor,true);
		//driveFront.SetInvertedMotor(RobotDrive::kRearRightMotor,true);
		//driveMiddle.SetInvertedMotor(RobotDrive::kFrontLeftMotor,true);
		//driveMiddle.SetInvertedMotor(RobotDrive::kFrontRightMotor,true);
		//driveMiddle.SetInvertedMotor(RobotDrive::kFrontRightMotor,true);
		printf((c->Enabled()?"Compressor On":"Compressor Off"));
		printf("\n");
	}

	void TeleopPeriodic()
	{
		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
		//driveFront.ArcadeDrive(stick);
		//driveMiddle.ArcadeDrive(stick);
		//driveBack.ArcadeDrive(stick);
		move();
		// if(swugbruh(180))
		//	moveElevator(0.5);
		printf("Encoder: %f",enc->GetDistance());

		if(solButt.Get() && false){
			//toggleSolenoid(&gearShift);
			setSolenoid(&gearShift,solButt.Get());
		}
		setSolenoid(&gearShift,solButt.Get());
	}

	void TestPeriodic()
	{
		lw->Run();
		// if (last_encoder_value != enc->GetPeriod()){
			// printf("Period:%f\nDirection:%f\nDistance:%f\nRate:%d",
			// 		enc->GetPeriod(),enc->GetDirection(),enc->GetDistance(),enc->GetRate());
			//last_encoder_value = enc->GetPeriod();
		// }
	}

	void DisabledInit(){
		//c->Start();
	}

	void move(){
		float x = stick.GetX();
		float y = stick.GetY();
		float z = stick.GetZ();
		float turn = stick.GetTwist();
		float throttle = stick.GetThrottle();

		throttle = (-throttle + 1.0)/2.0; // Shift the throttle value from -1-1 to 0-1

		x*=throttle;
		y*=throttle;
		z*=throttle;
		turn*=throttle;

		//driveFront.ArcadeDrive(y,turn);
		//driveMiddle.ArcadeDrive(y,turn);
		//driveBack.ArcadeDrive(y,turn);

		drive->MecanumDrive_Cartesian(x,y,turn);

	}

	void SystemCheck(){
		if(DriverStation::GetInstance()->GetBatteryVoltage()<=20){
			printf("[WARN] - Battery voltage low.");
			c->Stop();
		}
	}

	void toggleSolenoid(Solenoid *sol){
		//sol->Set(!sol->Get());
		setSolenoid(sol,!sol->Get());
	}

	void setSolenoid(Solenoid *sol,bool value){
		sol->Set(value);
	}

	void moveElevator(float pwr){
		elevatorMotor->Set(pwr);
	}

	bool swugbruh(float value){
		/*Checks if the encoder has gone past a certain value.*/
		return value >= 0;
	}

	bool IsUserAGoat(){
		return true;
	}
};

START_ROBOT_CLASS(Robot);
