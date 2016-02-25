#include "nMediaSubsession.h"
#include "nDeviceSource.h"

#include <H264VideoRTPSink.hh>
#include <H264VideoStreamFramer.hh>
#include <H264VideoStreamDiscreteFramer.hh>

nMediaSubsession* nMediaSubsession::createNew(UsageEnvironment& env) {
		return new nMediaSubsession(env);
}

nMediaSubsession
	::nMediaSubsession(UsageEnvironment& env)
	: OnDemandServerMediaSubsession(env, True /*reuse the first source*/){
}

nMediaSubsession
	::~nMediaSubsession() {
}

FramedSource* nMediaSubsession ::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {
	estBitrate = 1024;

	nDeviceSource* liveSource = nDeviceSource::createNew(envir());

	// Create a framer for the Video Elementary Stream:
	return H264VideoStreamFramer::createNew(envir(), liveSource);
}

RTPSink* nMediaSubsession ::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char /*rtpPayloadTypeIfDynamic*/, FramedSource* /*inputSource*/) {
	return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
}





