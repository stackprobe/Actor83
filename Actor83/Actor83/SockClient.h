void SockSend(autoList<uchar> *sendData);
int IsSockSending(void);
autoList<uchar> *SockRecv(void);
char *GetSockInfoMessage(void);
char *GetSockErrorMessage(void);
void SetSockConnectWaitMillisMax(int millisMax);
int GetSockConnectWaitMillis(void);
