#pragma once
class Animation {
public:
	Animation();
	Animation(int id, double duration, bool looping = false);
	void tick(double time);
	void setDuration(double time);
	void setLooping(bool looping);
	void enable(double time); // time is to update pastTick for time calculations
	void reset();
	void disable();
	int getId() const;
	double getTime() const;
	double getPercent() const;
	double getDuration() const;
	bool isLooping() const;
	bool isEnabled() const;

private:
	int m_id = 0;
	double m_time = 0.0;
	double m_duration = 0.0;
	bool m_looping = false;
	bool m_enabled = false;

	double m_pastTick = 0.0;
};