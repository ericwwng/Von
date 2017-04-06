/*
Self-explanatory: look online if unsure of purpose
*/

#pragma once

template<typename C>
class Singleton {
public:
    inline static C& getInstance() 
	{
        static C I; //Gives the global variable property
        return I;
    }
protected:
    Singleton() {}
};
