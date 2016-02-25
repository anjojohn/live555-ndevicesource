#ifndef _ANALYSING_SERVER_MEDIA_SUBSESSION_HH
#define _ANALYSING_SERVER_MEDIA_SUBSESSION_HH

#include <OnDemandServerMediaSubsession.hh>
//#include "Live_AnalyserInput.h"

class nMediaSubsession: public OnDemandServerMediaSubsession {

public:
  static nMediaSubsession*
  createNew(UsageEnvironment& env);


protected: // we're a virtual base class
  nMediaSubsession(UsageEnvironment& env);
  virtual ~nMediaSubsession();

protected:


private:
  // redefined virtual functions
  virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);

};

#endif
