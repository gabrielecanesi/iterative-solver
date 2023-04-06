#ifndef UPDATE_STRATEGY_H
#define UPDATE_STRATEGY_H

template<typename T>
class UpdateStrategy {
    public:
    virtual void update() = 0;
    virtual ~UpdateStrategy(){}
};


#endif