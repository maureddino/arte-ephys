#include <NIDAQmx.h>
#include <stdint.h>
#include <string>
class Timer {

 private:

  //Methods
  int becomeMaster();
  int armTimer();

  int initDAQCounterTask();
  int initDAQSyncTask();
  int armCounterTask();
 
  //naming syntax required for the  NIDaqMx signalChange
//  int32 CVICALLBACK digitalChangeCallback(TaskHandle taskHandle, int32 signalID, void *callbackData);

 //Variables

  char niDev[256];
  char niCtr[256];
  char niArm[256];
  char niSync[256];

  int timerID;
 
  bool running;
  bool armed;
  bool master; 
  bool isSynced;

  uint32_t initCount;
  int countDir;
  int countEdge;

  int txSyncPort;
  int rxSyncPort;

  uint32_t syncCount;

  TaskHandle counterTask;
  TaskHandle diPulseTask;

 public:
  Timer();

  virtual ~Timer();

  int txSyncCount(uint32_t syncCount, int nPacket);
  uint32_t rxSyncCount();

  bool isMaster();
  int becomeMaster(bool isMaster);
  int setDevStrs(std::string const& dev, std::string const& ctr, 
std::string const& arm, std::string const& sync);
 
  int start();
  int stop();

  int setInitCount(uint32_t initCount);
  uint32_t getCount();
  uint64_t getTimestamp();

  int setTimerID(int newId);
  int getTimerID();

  uint32_t getSyncCount();
  int 	setSyncCount(uint32_t count);

};