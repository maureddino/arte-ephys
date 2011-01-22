#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <byteswap.h>

#define ntoh64(x) bswap_64(x);
#define hton64(x) bswap_64(x);

#define ntoh32(x) bswap_32(x);
#define hton32(x) bswap_32(x);

#define ntoh16(x) bswap_16(x);
#define hton16(x) bswap_16(x);


#define NETCOM_UDP_SPIKE 1;
#define NETCOM_UDP_LFP 2;
#define NETCOM_UDP_SYNC 3;

class NetCom{
  public:
	static int txSyncCount(char host[], int port, uint32_t count, int nTx);
	static uint32_t rxSyncCount(int port);
};
