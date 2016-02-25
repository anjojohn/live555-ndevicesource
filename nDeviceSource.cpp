#include "nDeviceSource.h"
#include <GroupsockHelper.hh> // for "gettimeofday()"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//static uint8_t *buf = (uint8_t*)malloc(102400);
static uint8_t buf[640*480/2];
int fdstream;

nDeviceSource*
nDeviceSource::createNew(UsageEnvironment& env)
{
  return new nDeviceSource(env);
}

EventTriggerId nDeviceSource::eventTriggerId = 0;
unsigned nDeviceSource::referenceCount = 0;

nDeviceSource::nDeviceSource(UsageEnvironment& env):FramedSource(env) 
{ 

  if (referenceCount == 0) 
      fdstream = open("h264",O_RDWR); //remove

  ++referenceCount;

  if (eventTriggerId == 0) 
    eventTriggerId = envir().taskScheduler().createEventTrigger(deliverFrame0);
}

nDeviceSource::~nDeviceSource(void) {
  --referenceCount;
  envir().taskScheduler().deleteEventTrigger(eventTriggerId);
  eventTriggerId = 0;

  if (referenceCount == 0) 
  {
	//
  }
}

int loop_count;

void nDeviceSource::doGetNextFrame() 
{
	int ret;
	ret = read(fdstream,buf, sizeof(buf));
	//NVNTODO: copy encoded frame to buf here :)
	deliverFrame();

}

void nDeviceSource::deliverFrame0(void* clientData) 
{
  ((nDeviceSource*)clientData)->deliverFrame();
}

void nDeviceSource::deliverFrame() 
{
  if (!isCurrentlyAwaitingData()) return; // we're not ready for the data yet
  u_int8_t* newFrameDataStart = (u_int8_t*) buf;             //(u_int8_t*) buf; //%%% TO BE WRITTEN %%%
  unsigned newFrameSize = sizeof(buf); //%%% TO BE WRITTEN %%%
  // Deliver the data here:
  // NVNTODO: Make a variable to pass size also here instead of sizeof()
  if (newFrameSize > fMaxSize) {
    fFrameSize = fMaxSize;
    fNumTruncatedBytes = newFrameSize - fMaxSize;
  } else {
    fFrameSize = newFrameSize;
  }
  gettimeofday(&fPresentationTime, NULL); 
  memmove(fTo, newFrameDataStart, fFrameSize);
  FramedSource::afterGetting(this);
}
