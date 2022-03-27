#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{

public:
	Studio();
    Studio(const std::string &configFilePath);
    virtual ~Studio();
    Studio(const Studio &other);
    Studio& operator=(const Studio &other);
    Studio(Studio&& other);
    Studio& operator=(Studio &&other);
    void start();
    int getCustomersCount() const;
    Trainer* getTrainer(int tid);
    std::vector<Trainer*> getTrainers();
    void closeStudio();
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void insertLog(BaseAction* toInsert);

private:
    bool open;
    int numOfTrainers;
    int customers_counter;
    std::vector<Trainer*> trainers;
    std::vector<BaseAction*> actionsLog;
    std::vector<Workout> workout_options;

};



#endif