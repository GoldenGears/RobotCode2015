//*************
// Golden Gears Robotics
// 1/20/15
// Robot MAIN
//*************

// Tyler Robbins - 1-22-15 - Added an Autonomous class to hold Autonomous methods.
// Tyler Robbins - 1-23-15 - Removed m_timer2. Removed elevatorMove method. Replaced all references to m_timer2 with references to m_timer.

#include "WPILib.h"

class AutonomousDrive : public Command{
public:
	AutonomousDrive(RobotDrive *drive,double seconds,float pwr):
	Command("AutonomousDrive"),
	m_drive(drive),
	m_seconds(seconds),
	m_pwr(pwr/100),
	m_timer()
	{
	}

	~AutonomousDrive(){ }

	void Initialize(){
		m_timer.Start();
	}

	void Execute(){
		int forward_sec = 2;
		// m_drive->MecanumDrive_Cartesian(0,m_pwr,0);
		if(!m_timer.HasPeriodPassed(forward_sec))
			driveY(m_pwr);
	}

	bool IsFinished(){
		bool finished = m_timer.HasPeriodPassed(m_seconds);
		printf("Autonomous %s", (finished ? "finished":"not finished"));
		return finished;
	}
	void End(){
		m_timer.Stop();
		m_timer.Reset();
	}
	void Interrupted(){
		End();
	}

	void driveY(float pwr){
		m_drive->MecanumDrive_Cartesian(0,pwr,0);
	}

	void driveX(float pwr){
		m_drive->MecanumDrive_Cartesian(pwr,0,0);
	}

	void turn(float pwr){
		m_drive->MecanumDrive_Cartesian(0,0,pwr);
	}

	double getSeconds(){ return m_seconds; }

	float getPower(){ return m_pwr; }
private:
	RobotDrive *m_drive;
	double m_seconds;
	float m_pwr;
	Timer m_timer;
};


