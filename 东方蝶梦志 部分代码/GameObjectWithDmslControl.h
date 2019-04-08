#pragma once
#include <cstdint>
#include "GameObject.h"
#include "TaskList.h"
#include "Globals.h"
#include <vector>
#include "DataActor.h"
//#include <forward_list>
#include "Shooter.h"
#include "Dmsl.h"
#include <algorithm>

namespace Dmsl {
	struct GameObjectLibraryLinkTable;
}

class GameObjectWithDmslControl :public GameObject
{
public:
	struct Task {
		enum {
			MOVE,	//移动到目标位置
			SPEED_CHANGE,	//速度渐变（从old_x变到trg_x）
			ANGLE_CHANGE	//角度渐变（（从old_x变到trg_x）
		}type;
		double old_x, old_y;
		double trg_x;
		double trg_y;
		uint32_t frameTimes;	//总时间
		int slowMotion;	//缓动模式
	};

private:
	double m_x = 0;
	double m_y = 0;
	uint32_t m_frameLife = 0;
	double m_angle = 0;
	double m_speed = 0;

	double m_xSpeedMul = 1, m_ySpeedMul = 1;

	//bool m_taskDelayNoteMode = false;
protected:
	Basic::DataActor m_dataActor;
	Basic::TaskList m_taskList;
	Shooter::Shooter m_shooter;
	//std::forward_list<std::function<void()>> m_nowDelayedTask;
public:

	struct VMContext {
		std::shared_ptr<Dmsl::DmslVirtualMachine> dvm;
		std::shared_ptr<Dmsl::GameObjectLibraryLinkTable> table;
		uint32_t diedHandle;
	};
private:
	VMContext m_vmContext;
public:

	inline const VMContext& GetVMContext() const
	{
		return m_vmContext;
	}
	inline void SetVMContext(const VMContext& v)
	{
		m_vmContext = v;
	}

	virtual ~GameObjectWithDmslControl() {};

	/*inline bool GetTaskNoteModeEnabled() { return m_taskDelayNoteMode; }

	inline void AddTask(std::function<void()> f) {
		m_nowDelayedTask.push_front(f);
	}*/

	auto& GetShooter() {
		return m_shooter;
	}

	inline void SetTask(const Task& t) {
		//m_tasks.push_back(t);
		//m_tasks.back().beginTimes = m_frameLife;
		//std::function<void()> task = [this,t]() {
		switch (t.type) {
		case Task::MOVE:
			m_dataActor.Add(m_x, t.trg_x, t.frameTimes, t.slowMotion);
			m_dataActor.Add(m_y, t.trg_y, t.frameTimes, t.slowMotion);
			m_speed = 0;
			break;
		case Task::SPEED_CHANGE:
			m_dataActor.Add(m_speed, t.trg_x, t.frameTimes, t.slowMotion);
			break;
		case Task::ANGLE_CHANGE:
			m_dataActor.Add(m_angle, t.trg_x, t.frameTimes, t.slowMotion);
			break;
		};
		//};

		//if (GetTaskNoteModeEnabled())
			//AddTask(task);
		//else
		//task();
	}

	inline Basic::DataActor& GetDataActor() {
		return m_dataActor;
	}

	inline bool Died() const {
		return m_died;
	}
	inline void SetPos(double x, double y) {
		m_x = x;
		m_y = y;
	}
	inline void GetPos(double& x, double& y) const {
		x = m_x;
		y = m_y;
	}
	inline uint32_t GetLife() const {
		return m_frameLife;
	}
	inline double GetSpeed() const {
		return m_speed;
	}
	inline double GetAngle() const {
		return m_angle;
	}
	inline void SetSpeed(double speed) {
		m_speed = speed;
	}
	inline void SetAngle(double angle) {
		m_angle = angle;
	}
	inline void AddSpeed(double speed) {
		m_speed += speed;
	}
	inline void AddAngle(double angle) {
		m_angle += angle;
	}

	//inline void TaskNoteBegin() {
	//	m_taskDelayNoteMode = true;
	//}

	//inline void TaskNoteEnd(int frames) {
	//	for (auto& p : m_nowDelayedTask)
	//		m_taskList.AddTask(p,frames);
	//	m_nowDelayedTask.clear();
	//	m_taskDelayNoteMode = false;
	//}

	virtual void Update() {
		m_taskList.Update();
		m_dataActor.Update();
		++m_frameLife;

		m_x += m_speed * cos(m_angle) * m_xSpeedMul;
		m_y += m_speed * sin(m_angle) * m_ySpeedMul;
	}

	inline void SetSpeedMul(double x, double y)
	{
		m_xSpeedMul = x;
		m_ySpeedMul = y;
	}

	virtual void Draw() const = 0;

	virtual void Kill() = 0;
};
