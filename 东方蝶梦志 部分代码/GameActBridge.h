#pragma once

/* 用于游戏主逻辑和游戏活动间的交互 */
class GameActBridge {
public:

	//从主逻辑调用到游戏活动的回调
	virtual void OnPlayerAllDied() {}	//玩家全部死亡时
	virtual void OnStageClear() {}	//当关卡通过时
	virtual void FinishSpell(int spellHash, bool bonusGot) {}

	virtual ~GameActBridge() {}
};