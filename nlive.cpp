#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "nDeviceSource.h"
#include <H264VideoStreamFramer.hh>
#include <H264VideoStreamDiscreteFramer.hh>
#include "nMediaSubsession.h"


UsageEnvironment* env;

// Just for printing Stream info
static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
//  env << "\n\"" << streamName << "\" stream, from the file \""
//      << inputFileName << "\"\n";
  env << "\n\e[1m\e[4mLive h264 Media Subclassed source \e[0m\n";
  env << "Compiled " __DATE__ " Naveen\n\n";
  env << "RTSP URL: \e[92m " << url << "\e[39m\n\n";
  delete[] url;
}


int main(int argc, char** argv) {

  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  // Create the RTSP server:
  RTSPServer* rtspServer = RTSPServer::createNew(*env, 8555, NULL);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  char const* descriptionString = "Session streamed by NvN";

  // A H.264 video elementary stream:
  {
    char const* streamName = "nlive";
    char const* inputFileName = "test.264";
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    //sms->addSubsession( H264VideoFileServerMediaSubsession ::createNew(*env, inputFileName, True)); //False=no reuse stream
    sms->addSubsession(nMediaSubsession::createNew(*env));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, inputFileName);	// not needed just for better diagnostic
  }

  env->taskScheduler().doEventLoop(); // does not return

  return 0;
}



