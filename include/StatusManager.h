#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H


#include <direct.h>
#include "conio.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <windows.h>

template< class T>
inline T rotl( T v, int shift){
    std::size_t m = sizeof(v)*std::numeric_limits<T>::digits;
    T s = shift>=0? shift%m: -((-shift)%m);
    return (v<<s) | (v>>(m-s));
}

template< class T>
inline T rotr(T v, int shift){
    std::size_t m = sizeof(v)*std::numeric_limits<T>::digits;
    T s = shift>=0? shift%m: -((-shift)%m);
    return (v>>s) | (v<<(m-s));
}

class StatusManager
{
protected:
	StatusManager() : m_state(0x00){};

public:
	enum ST_STATUS {
					 ST_PLAYING = 0x00000001,
					 ST_PLAYING_WITH_CLUES = 0x00000002,
					 ST_EDITING = 0x00000004,
					 ST_DISPLAY_SOLUTION = 0x00000008
					

	};
	static std::string toString(ST_STATUS _p)
	{

		if (_p == ST_PLAYING) return std::string("ST_PLAYING");
		if (_p == ST_PLAYING_WITH_CLUES) return std::string("ST_PLAYING_WITH_CLUES");
		if (_p == ST_EDITING) return std::string("ST_EDITING");
		if (_p == ST_DISPLAY_SOLUTION) return std::string("ST_DISPLAY_SOLUTION");

	}

	static ST_STATUS fromString(std::string _s)
	{
		if (!_s.compare("ST_PLAYING")) return ST_PLAYING;
		if (!_s.compare("ST_PLAYING_WITH_CLUES")) return ST_PLAYING_WITH_CLUES;
		if (!_s.compare("ST_EDITING")) return ST_EDITING;
		if (!_s.compare("ST_DISPLAY_SOLUTION")) return ST_DISPLAY_SOLUTION;

	}
	static StatusManager& instance()
	{
		static StatusManager obj;
		return obj;
	}

	static  void setState(std::string _st) { StatusManager::instance().SetState(_st); }
	static  void setState(ST_STATUS _st) { StatusManager::instance().SetState(_st); }
	static  void setOn(std::string _p) { StatusManager::instance().SetOn(_p); }
	static void setOn(ST_STATUS _p) { StatusManager::instance().SetOn(_p); }
	static void unsetState(ST_STATUS _p) { StatusManager::instance().UnsetState(_p); }
	static 	void clearType() { StatusManager::instance().ClearType(); }
	static 	std::vector<ST_STATUS> getType() { return StatusManager::instance().GetType(); }
	static 	bool is(ST_STATUS _t) { return StatusManager::instance().Is(_t); }
	static 	void change(ST_STATUS _t) {  StatusManager::instance().Change(_t); }
	static 	void next() { StatusManager::instance().Next(); }
	static 	void prev() { StatusManager::instance().Prev(); }


protected:




	void SetState(std::string _st)
	{
		SetState(fromString(_st));

	}
	void SetState(ST_STATUS _p)
	{
		//activate ST_STATUS,  rest remains the same
		m_state = m_state | _p;
	}
	void SetOn(std::string _p)
	{
		SetOn(fromString(_p));

	}
	void SetOn(ST_STATUS _p)
	{
		//the rest of the states to 0
		m_state =  _p;
	}
	void UnsetState(ST_STATUS _p)
	{
		m_state = m_state & ~_p;
	}
	void ClearType() { m_state = 0x00; }

	std::vector<ST_STATUS> GetType()
	{
		std::vector<ST_STATUS> v;
		if (m_state & ST_PLAYING)
			v.push_back(ST_PLAYING);
		if (m_state & ST_PLAYING_WITH_CLUES)
			v.push_back(ST_PLAYING_WITH_CLUES);
		if (m_state & ST_EDITING)
			v.push_back(ST_EDITING);
		if (m_state & ST_DISPLAY_SOLUTION)
			v.push_back(ST_DISPLAY_SOLUTION);
		



		return v;
	}
	bool Is(ST_STATUS _t)
	{
		return m_state & _t;
	}
	void Change(ST_STATUS _t)
	{
		m_state = m_state^ _t;
	}
	
	void Next()
	{

		m_state = rotl<int>(m_state, 1);


	}
	void Prev()
	{
		m_state = rotr<int>(m_state, 1);

	}
	
	
protected:
	int m_state;
};

#endif
