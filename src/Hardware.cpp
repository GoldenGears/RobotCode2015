//*************************
// Golden Gears Robotics
// 1/24/15
// Hardware MAIN
//*************************

// Tyler Robbins - 1-25-15 - Added a class to hold all methods and objects relating to the robot's hardware. Excluding joysticks.
// Tyler Robbins - 1-26-15 - Removed commented out code. Added accessor methods. Added potentiometer. Added try-catch statement to ElevatorPeriodic to check the height of the elevator. move() method now accepts pointer. Added method to return the height of the elevator.
// Tyler Robbins - 1/29/15 - Added inverters to the left motors (all motors are right-hand). Commented out a bunch of code that may or may not work. Made move() accept a pointer.

#include <math.h>
#include <exception>

#include "WPILib.h"

const float ELEVATOR_DEADBAND = 100;
const float MAX_ELEVATOR_HEIGHT = 78; // Height of elevator in inches.



class Hardware{
public:
//	Encoder *enc;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	TalonSRX *backRight;
//	Talon *elevatorMotor;
//	AnalogPotentiometer *poten;
	RobotDrive *drive;
	Compressor *c;

	bool height_error = false;

	Hardware(){
//		enc = new Encoder(8,9);
		frontLeft = new Talon(0);
		frontRight = new Talon(2);
		backLeft = new Talon(1);
		backRight = new TalonSRX(3);
//		elevatorMotor = new Talon(9);
//		poten = new AnalogPotentiometer(0);
		drive = new RobotDrive(frontLeft,backLeft,frontRight,backRight);
		c = new Compressor(0);
		drive->SetExpiration(0.1);
		drive->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor,true);
		drive->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor,true);
	}

	void toggleSolenoid(Solenoid *sol){
		//sol->Set(!sol->Get());
		setSolenoid(sol,!sol->Get());
	}

	void setSolenoid(Solenoid *sol,bool value){
		sol->Set(value);
	}

//	void moveElevator(float pwr){
//		elevatorMotor->Set(pwr);
//	}
//
//	void ElevatorUp(float pwr){
//		moveElevator(pwr/100);
//	}
//
//	void ElevatorDown(float pwr){
//		moveElevator(-pwr/100);
//	}
//
//	void ElevatorPeriodic(bool up=false,bool down=false,bool gotop=false){
////		try{
////			if(getElevatorHeight() >= MAX_ELEVATOR_HEIGHT+12)
////				// Throw an error if the elevator is too high
////				throw std::logic_error("Invalid elevator height error");
////
////			else if(getElevatorHeight() >= MAX_ELEVATOR_HEIGHT-0.5)
////				height_error = true;
////			else if(getElevatorHeight() < MAX_ELEVATOR_HEIGHT-8)
////				height_error = false;
////			else{}
//
//
//			if(up and not height_error)
//				ElevatorUp(30);
//			else if(down)
//				ElevatorDown(30);
//			else if(gotop and not height_error)
//				ElevatorGoToPeriodic(1000);
//			else
//				ElevatorDown(0);
////		}
////		catch(std::exception& e){
////			printf("Abandon all hope, ye who get this value:%f\n",getElevatorHeight());
////			printf("Something has gone horribly wrong, and an [%s] has occurred.\n",e.what());
////			printf("If you are seeing this error, then something is broken. Contact the programmer and check the elevator.\n");
////			height_error = true;
////			ElevatorDown(40);
////		}
//	}
//
//	void ElevatorGoToPeriodic(float poscmd){
//		float poserror = poscmd - enc->GetDistance();
//
//		if(std::abs(poserror)<(ELEVATOR_DEADBAND/2))
//			poserror = 0;
//
//		if(poserror > ELEVATOR_DEADBAND)
//			ElevatorUp(30);
//		else if(poserror < -ELEVATOR_DEADBAND)
//			ElevatorDown(30);
//		else
//			moveElevator(0);
//	}

//	bool swugbruh(float value){
//		/*Checks if the encoder has gone past a certain value.*/
//		return enc->GetDistance() >= value;
//	}

	void move(Joystick* stick){
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

		drive->MecanumDrive_Cartesian(x,y,turn,0.0);

	}

//	float getElevatorHeight(){
//		return (float)poten->Get();
//	}

//	AnalogPotentiometer* GetPotentiometer(){ return poten; }

	RobotDrive* GetDrive(){ return drive; }

//	Encoder* GetEncoder(){ return enc; }

	Talon* GetFrontLeft(){ return frontLeft; }
	Talon* GetFrontRight(){ return frontRight; }
	Talon* GetBackLeft(){ return backLeft; }
	TalonSRX* GetBackRight(){ return backRight; }

};


