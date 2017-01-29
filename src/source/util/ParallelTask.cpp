#include "../../headers/util/ParallelTask.hpp"

ParallelTask::ParallelTask() : mThread(&ParallelTask::runTask, this), mFinished(false)
{

}

void ParallelTask::execute(){
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}

bool ParallelTask::isFinished(){
	sf::Lock lock(mMutex);
	return mFinished;
}

float ParallelTask::getCompletion(){
	sf::Lock lock(mMutex);

	// 100% at 10 seconds of elapsed time
	return mElapsedTime.getElapsedTime().asSeconds() / 1.5f;
}

void ParallelTask::runTask(){

	// Dummy task - stall 10 seconds
	bool ended = false;

	while (!ended){
		sf::Lock lock(mMutex);
		//Protect the clock

		//printf("%f\n", mElapsedTime.getElapsedTime().asSeconds());

		if (mElapsedTime.getElapsedTime().asSeconds() > 1.5f)
			ended = true;
	}

	{
		// mFinished may be accessed from multiple threads, protect it
		sf::Lock lock(mMutex);
		mFinished = true;
	}
	
}